# Captcha-server
## 0x00 Description

Captcha-server imported `ioredis`, `grpc-js`, `pproto-loader`, `nodemailer`, `uuidv4` libraries to the project. 



## 0x01 Requirements

### Servers' Configurations

```json
{
      "email": {
                "host": "please set to your email host name",
                "port": "please set to your email port",
                "username": "please set to your email address",
                "password": "please use your own authorized code"
      },
      "mysql": {
                "host": "127.0.0.1",
                "port": 3307,
                "password": 123456
      },
      "redis": {
                "host": "127.0.0.1",
                "port": 16379,
                "password": 123456
      }
}
```



## 0x02 Developer Quick Start

### Platform Support
Windows, Linux, MacOS(Intel & Apple Silicon M)

### Download Captcha-server

```bash
git clone https://github.com/Liupeter01/captcha-server
```

### Compile Captcha-server

```bash
cd captcha-server
npm install
node index.js  # you could use nodemon
```
