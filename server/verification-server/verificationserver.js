const grpc = require('@grpc/grpc-js');
const uuidv4 = require('uuidv4')
const config = require('./config');
const def = require('./defs')
const { sendMail } = require('./email');
const message_proto = require('./proto');
const redis = require('./redis')

async function generateEmail(email, callback){

    /*generate verification code*/
    let id = uuidv4.uuid()
    console.log('uuid generator: ', id);

    let email_text = {
        from: config.email_username,
        to: email,
        subject:'verification code',
        text: 'verification code is ' + id + ' please register within three minutes!'
    }

    /*store <key, value> to redis*/
    let result = redis.setValueExpiracy(email, id, 180/*180s*/)
    if(!result){
        callback(null, 
        {
            email: email,
            error: def.Status.RedisError
        });
    }

    /*send email and wait for response*/
    result = await sendMail(email_text);
    console.log('send email status: ', result);

    callback(null, 
    {
        email: email,
        error: result ? def.Status.Success : def.Status.Exception
    });
}

async function GetVerificationCode(GetVerificationRequest, GetVerificationResponse) {
    console.log('requested email: ', GetVerificationRequest.request.email);
    try {
        
        /*this email is not requested within x minutes!*/
        let res = await redis.checkKey(GetVerificationRequest.request.email)
        if(res == null){
            /*generate verification code*/
            await generateEmail(
                GetVerificationRequest.request.email,
                GetVerificationResponse
            );
        }
        else{

        }
    } 
    catch (error) 
    {
        console.log('send email error! error is: ', error);

        GetVerificationResponse(null, {
            email: GetVerificationRequest.request.email,
            error: def.Status.Exception
        });
    }
}

function main() {
    //create a grpc server
    var server = new grpc.Server();
    server.addService(message_proto.VerificationService.service, {GetVerificationCode: GetVerificationCode});
    server.bindAsync('0.0.0.0:65500', grpc.ServerCredentials.createInsecure(), (error, port)=>{
        //server.start();
        console.log('verification server activated!');
    })
}

module.exports.main = main;