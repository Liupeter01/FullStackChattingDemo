const nodemailer = require('nodemailer');

/*include config.js*/
const config = require('./config');

/* create email sending transport service*/
let transport = nodemailer.createTransport({
    host: config.email_host,
    port: config.email_port,
    secure: true,
    auth:{
        user: config.email_username,
        pass: config.email_password
    }
});

/*convert a async function to a sync function*/
function sendMail(params) {
    return new Promise(function(resolve, reject){
        /*async function*/
        transport.sendMail(params, (err, info) => {
            if(err){
                console.log(error);
                reject(err);
            }
            else{
                console.log('sending email successfully! response: ' + info.response);
                resolve(info.response);
            }
        });   
    })
}

module.exports.sendMail = sendMail;