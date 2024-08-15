# FullStackChattingDemo
## Description

FullStackChattingDemo using C++17 standard

## Client

### Libraries

using QT6(Q::Widgets Qt::Network)

![](./assets/client_main.png)

![](./assets/register_page.png)

## Server

### Libraries

boost-1.84  (beast asio uuid), grpc-1.50.2, hiredis, jsoncpp, ada(url parsing), spdlog

### Functions

/get_test(GET method): system function test.

/get_verification(POST method): User sends get CPATCHA request to server. server using GRPC protocol to communicate with NodeJS server and generate and store uuid in Redis DB.

/post_registration(POST method): User post registration request to server. server store info into DB.

![](./assets/server.png)

### Submodule -- Verification Server(NodeJS)

import ioredis, grpc-js proto-loader, nodemailer, uuidv4 libraries to the project

Sending verification code to server
![](./assets/verification.png)

 ![](./assets/result.png)



## Requirements

The project is self-contained almost all dependencies on both Windows and Linux/Unix-like systems.

### Main Server(C++)

main server using config.ini to store parameters

```ini
[GateServer]
port =
[VerificationServer]
host=
port=
[MySQL]
host=
port=
password=
[Redis]
host=
port=
password=
```

### Verification Server(Nodejs)

verification server using verification-server/config.json to store parameters

```json
{
    "email":{
        "host": "please set to your email host name",
        "port": "please set to your email port",
        "username": "please set to your email address",
        "password": "please use your own authorized code"
    },
	"mysql": {
		"host": "your ip",
		  "port": "your port",
		  "password":  "your password"
	},
	"redis": {
		"host": "your ip",
		"port": "your port",
		"password": "your password"
	}
}
```

### Redis Server

1. For Windows -- redis-5.0.14.1

   URL: https://github.com/tporadowski/redis/releases/tag/v5.0.14.1

2. For Linux/MacOS

   ```bash
   #Pull the official docker image from Docker hub
   ```

### MySQL Server

1. For Windows -- redis-5.0.14.1

   URL: https://dev.mysql.com/get/Downloads/MySQL-9.0/mysql-9.0.1-winx64.msi

2. For Linux/MacOS

   ```bash
   #Pull the official docker image from Docker hub
   	docker pull mysql:8.0
   #create container(mapping inner port 3306 to host 3306)
   ```



## Developer Quick Start

### Platform Support
Windows, Linux, MacOS(Intel & Apple Silicon M)

### Download FullStackChattingDemo

```bash
git clone https://github.com/Liupeter01/FullStackChattingDemo
```

### Compile Client

1. For Windows

   ```bash
   #please import client dir to qt creator
   ```

2. For MacOS/Linux

   ```bash
   #It might take a long time to download dependency libraies!!!!
   cd FullStackChattingDemo/client
   cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
   cmake --build build --parallel [x]
   ```

### Compile Server

```bash
cd FullStackChattingDemo/server
git submodule update --init
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel [x]
```

### How to Execute

1. Execute Main Server Program

   ```bash
   ./server/build/ChattingServer
   ```

2. Activate Redis and MySQL service

3. Execute Verification Server(Nodejs)

   ```bash
   cd server/verification-server
   node index.js
   ```

4. Execute Client

   ```bash
   ./client/build/ChattingServer
   ```

### Error Handling
1. SyntaxError: Unexpected token  in JSON at position 0
   ```bash
   SyntaxError: Unexpected token  in JSON at position 0
       at JSON.parse (<anonymous>)
   ```

   Solving
   please change your encoding method to UTF-8, especially for VSCode user

   Referring Url
   https://stackoverflow.com/questions/55960919/nodejs-syntaxerror-unexpected-token-in-json-at-position-0

   
   
2. undefined symbol upb_alloc_global
   ```cmake
   set(protobuf_BUILD_LIBUPB OFF)
   ```

   Referring Url
   https://github.com/grpc/grpc/issues/35794

   
   
3. fatal error: 'unicode/locid.h' 'unicode/ucnv.h' file not found (usually happened on MacOS)
   Download icu 74.1
   ```bash
   wget https://github.com/unicode-org/icu/releases/download/release-74-1/icu4c-74_1-src.tgz
   ```

   Compile and Install
   ```bash
   git clone https://github.com/unicode-org/icu.git
   mkdir icu4c-build
   cd icu4c-build
   ./runConfigureICU Linux
   make check
   ```

   set cmake variable
   ```cmake
   cmake -Bbuild -DCMAKE_INCLUDE_PATH=/usr/local/include
   cmake --build build --parallel x
   ```

   Referring Url
   https://unicode-org.github.io/icu/userguide/icu4c/build.html

   
   
4. boringssl undefined win32
   ```cmake
   set(OPENSSL_NO_ASM ON)
   ```

   Referring Url
   https://github.com/grpc/grpc/issues/16376

   
   
5. Handling gRPC issue
   Issue description
   
   ```bash
   CMake Error: install(EXPORT "protobuf-targets" ...) includes target "libprotobuf-lite" which requires target "absl_node_hash_map" that is not in any export set.
   ```
   
   Problem Solving
   ```cmake
   set(ABSL_ENABLE_INSTALL ON)
   ```
   
   Referring Url
    https://github.com/protocolbuffers/protobuf/issues/12185
    https://github.com/protocolbuffers/protobuf/issues/12185#issuecomment-1594685860
   
   
   
6. E No address added out of total 1 resolved

   you have to start the main server first and then open nodejs service
