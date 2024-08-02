cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

if(MSVC)
    enable_language(ASM_NASM)
    enable_language(ASM_MASM)
endif (MSVC)

project (ChattingServer VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE source_file CONFIGURE_DEPENDS src/*.cpp src/*.cc src/*.c)
file(GLOB_RECURSE header_file CONFIGURE_DEPENDS include/*.hpp  include/*.h)

add_executable(ChattingServer  ${source_file} ${header_file})
target_include_directories(ChattingServer PUBLIC include jsoncpp/include ada/include)

#target_compile_definitions(ChattingServer PRIVATE BOOST_ASIO_STANDALONE)

#import grpc 1.65.2 library
set(FETCHCONTENT_QUIET OFF)
set(ABSL_ENABLE_INSTALL ON)   #Very important!!!!
set(BOOST_NO_ASSEMBLY ON CACHE BOOL "Disable Boost Assembly")
include(FetchContent)
FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.84.0 
)
FetchContent_MakeAvailable(boost)

FetchContent_Declare(
    gRPC
    GIT_REPOSITORY https://github.com/grpc/grpc
    GIT_TAG  v1.65.2
)

FetchContent_MakeAvailable(
	boost
	gRPC
)

#set(_PROTOBUF_LIBPROTOBUF libprotobuf)
#set(_REFLECTION grpc++_reflection)
#set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
#set(_GRPC_GRPCPP grpc++)
#if(CMAKE_CROSSCOMPILING)
#   find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
#else()
#   set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
#endif()

add_subdirectory(ada)
add_subdirectory(jsoncpp)

target_link_libraries(ChattingServer PUBLIC 
        Boost::asio
        Boost::beast
        Boost::uuid
        ada-singleheader-lib
        jsoncpp_object
        #grpc 
        #grpc++
)