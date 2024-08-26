const fs = require('fs');

try {
    const config = JSON.parse(fs.readFileSync('./config.json','utf-8'))
    if (config) {
        console.log('Config loaded successfully:');
        console.log(config);
    
        // Access specific properties
        console.log('email host:', config.email.host);
        console.log('email port:', config.email.port);
        console.log('email username:', config.email.username);
    }
    else{
        console.log('Failed to load config.');
    } 

    /*load json configuration*/
    let email_host = config.email.host
    let email_port = config.email.port
    let email_username = config.email.username;
    let email_password = config.email.password;

    let mysql_host =config.mysql.host;
    let mysql_port =config.mysql.port;

    let redis_host = config.redis.host;
    let redis_port = config.redis.port;
    let redis_password = config.redis.password;

    module.exports = {
        email_host,
        email_port,
        email_username,
        email_password,
        mysql_host,
        mysql_port,
        redis_host,
        redis_port,
        redis_password
    };

} 
catch (error) {
    console.log('Parse Json Failed! error code: ', error);
}