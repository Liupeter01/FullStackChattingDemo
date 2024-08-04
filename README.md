# FullStackChattingDemo
## Description
### Client
1. Libraries: using QT6(Q::Widgets Qt::Network)

### Server
1. Libraries: 
   boost-1.84 -> FetchContent (contains beast asio uuid)
   grpc-1.50.2 -> FetchContent
   jsoncpp
   ada(url parsing)

2. Functions: accept both GET and POST methods
   Handling GET: /get_test
   Handling POST:/get_verification, client sends a json form to the server and server will parse the form and return a json form back to the client

## Requirements
The project is self-contained and it has no dependency on both Windows and Linux/Unix-like systems.

## Developer Quick Start
### Platform Support
~~Windows~~, Linux, ~~MacOS~~
Currently, MacOS with APPLE SILLICON is not supported yet

### Building  FullStackChattingDemo
```bash
git clone https://github.com/Liupeter01/FullStackChattingDemo
cd FullStackChattingDemo/server
git submodule update --init
cmake -B build
cmake --build build --parallel x
```

### Error Handling
1. undefined symbol
   ```cmake
   set(protobuf_BUILD_LIBUPB OFF)
   ```

   Referring Url
   https://github.com/grpc/grpc/issues/35794

   
   
2. fatal error: 'unicode/locid.h' 'unicode/ucnv.h' file not found (usually happened on MacOS)
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

   Referring Url
   https://unicode-org.github.io/icu/userguide/icu4c/build.html

   
   
3. Handling missing CMAKE_ASM_MASM_COMPILE_OBJECT issue on windows
   ```bash
   add that folder to the PATH and set another env variable ASM_NASM with the name of nasm.exe.
   ```
   ```cmake
   if(MSVC)
       enable_language(ASM_NASM)
       enable_language(ASM_MASM)
   endif (MSVC)
   project (ChattingServer VERSION 0.1 LANGUAGES CXX ASM_NASM ASM_MASM)
   ```

   Referring Url
   https://stackoverflow.com/questions/73214824/missing-cmake-asm-nasm-compiler-when-compiling-grpc-with-ms-visual-studio

   
   
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