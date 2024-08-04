cmake_minimum_required (VERSION 3.15)
include(FetchContent)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(FETCHCONTENT_QUIET OFF)

#disable boringssl assembly language
set(OPENSSL_NO_ASM ON)

#disable boost assembly language
set(BOOST_NO_ASSEMBLY ON CACHE BOOL "Disable Boost Assembly")

#we have to disable this to prevent upb_alloc_global
set(protobuf_BUILD_LIBUPB OFF)

#build gRPC++ with OpenSSL instead of BoringSSL
set(gRPC_SSL_PROVIDER module)
set(ABSL_ENABLE_INSTALL ON)   #Very important!!!!

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT MSVC)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        message(STATUS "Found CCache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
    endif()
endif()

#if(MSVC)
#    enable_language(ASM_NASM)
#    enable_language(ASM_MASM)
#endif (MSVC)
#ASM_NASM ASM_MASM
project (ChattingServer VERSION 0.1 LANGUAGES CXX )

file(GLOB_RECURSE source_file CONFIGURE_DEPENDS src/*.cpp src/*.cc src/*.c)
file(GLOB_RECURSE header_file CONFIGURE_DEPENDS include/*.hpp  include/*.h)

add_executable(ChattingServer  ${source_file} ${header_file})
target_include_directories(ChattingServer PUBLIC include jsoncpp/include ada/include)

#import boost 1.84.0 library
FetchContent_Declare(
            boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG boost-1.84.0 
)
#import grpc1.50.2 library
#strong not recommend to use grpc 1.60.x+ versions
FetchContent_Declare(
            gRPC
            GIT_REPOSITORY https://github.com/grpc/grpc
            GIT_TAG        v1.50.2
)
FetchContent_MakeAvailable(boost gRPC)

add_subdirectory(ada)
add_subdirectory(jsoncpp)

target_link_libraries(ChattingServer PUBLIC 
        Boost::asio
        Boost::beast
        Boost::uuid
        ada-singleheader-lib
        jsoncpp_object
        grpc++
)