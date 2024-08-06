# FullStackChattingDemo
## Description
### Client
1. Libraries: using QT6(Q::Widgets Qt::Network)
   ![](./client_main.png)

   ![](./register_page.png)


### Server
1. Libraries:
   boost-1.84 -> FetchContent (contains beast asio uuid)
   grpc-1.50.2 -> FetchContent
   jsoncpp
   ada(url parsing)

2. Functions: accept both GET and POST methods
   Handling GET: /get_test
   Handling POST:/get_verification, client sends a json form to the server and server will parse the form and return a json form back to the client
   ![](./server.png)

3. Verification Server
   Sending verification code to server
   ![](./verification.png)

    ![](./result.png)

## Requirements
The project is self-contained and it has no dependency on both Windows and Linux/Unix-like systems.
but on verification-server(server/verification-server) you have to set your own email and authorized code

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
		"port": "your port"
	},
	"redis": {
		"host": "your ip",
		"port": "your port",
		"password": "your password"
	}
}
```

## Developer Quick Start
### Platform Support
Windows, Linux, MacOS(Intel & Apple Silicon M)

### Building FullStackChattingDemo

Download FullStackChattingDemo Project
```bash
git clone https://github.com/Liupeter01/FullStackChattingDemo
```

1.client

For MacOS
```bash
cd FullStackChattingDemo/client
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel [x]
```

For Windows
```bash
#please import client dir to qt creator
```

2.server

We are going to simplified this in the future^_^!!!

Compile Main Server Code(c++)

It might take a long time to download dependency libraies!!!!


```bash
cd FullStackChattingDemo/server
git submodule update --init
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel [x]
```

Execute Main Server Program
```bash
./build/ChattingServer
```

Execute Verification Code(Nodejs)
```bash
cd verification-server
node index.js
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
