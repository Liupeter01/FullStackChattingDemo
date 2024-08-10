const Redis = require('ioredis')
const config = require('./config');

const redis = new Redis({
    port: config.redis_port, // Redis port
    host: config.redis_host, // Redis host
    password: config.redis_password,
});

redis.on("message", (channel, message) => {
    console.log(`Received ${message} from ${channel}`);
});

redis.on("error", (error)=>{
    console.log(`Redis error! Terminating... error code: ${error}`);

    /*came across issue, quiting!!*/
    quit();
});

function quit(){
    redis.quit();
}

/*set expiration configuration*/
async function setValueExpiracy(key, value, timeout){
    try {
        await redis.set(key, value);
        await redis.expire(key, timeout);
        return true;
    } 
    catch (error) {
        console.error(error);
        return false;
    }
}

async function checkKey(key){
    try {
        /**/
        const res = await redis.exists(key);
        console.log(0 === res ? 
            ('<', key, '>: ', `The value is not exist!`)
          : ('<', key, '>: ', `The value is ${res}`)
        );
        return (res === 0 ? null : res);
    } 
    catch (error) {
        console.error(error);
    }
}

/*async function to sync*/
async function getValue(key){
    try {
        /**/
        const res = await redis.get(key);
        console.log(null != res ? 
            ('<', key, '>: ', `The value is ${res}`)
          : ('<', key, '>: ', `The value cannot be found!`)
        );
        return res;
    } 
    catch (error) {
        console.error(error);
    }
}

module.exports = {
    checkKey,
    getValue,
    setValueExpiracy,
    quit
};