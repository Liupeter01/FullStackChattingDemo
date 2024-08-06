const grpc = require('@grpc/grpc-js');
const uuidv4 = require('uuidv4')
const config = require('./config');
const def = require('./defs')
const { sendMail } = require('./email');
const message_proto = require('./proto');

async function GetVerificationCode(GetVerificationRequest, GetVerificationResponse) {
    console.log('requested email: ', GetVerificationRequest.request.email);
    try {
        /*generate verification code*/
        let id = uuidv4.uuid()
        console.log('uuid generator: ', id);

        let email_text = {
            from: config.email_username,
            to: GetVerificationRequest.request.email,
            subject:'verification code',
            text: 'verification code is ' + id
        }

        /*send email and wait for response*/
        let result = await sendMail(email_text);
        console.log('send email status: ', result);

        GetVerificationResponse(null, {
            email: GetVerificationRequest.request.email,
            error: def.Status.Success
        });
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