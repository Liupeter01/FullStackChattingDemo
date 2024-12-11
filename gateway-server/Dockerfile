# Use a base image with C++ support
FROM gcc:latest

RUN apt-get update && apt-get install -y \
    net-tools \
    gdb \ 
    cmake \
    g++ 

# Set the working directory
WORKDIR /app

# Clone the repository and initialize submodules
RUN git clone https://github.com/Liupeter01/gateway-server --depth 1 && \
    cd gateway-server && \
    git submodule update --init

# Build the project with CMake
RUN cmake -Bbuild -S gateway-server -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --parallel $(nproc)