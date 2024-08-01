cmake_minimum_required (VERSION 3.15)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

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

project (ChattingServer VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE source_file CONFIGURE_DEPENDS src/*.cpp src/*.cc src/*.c)
file(GLOB_RECURSE header_file CONFIGURE_DEPENDS include/*.hpp  include/*.h)

add_executable(ChattingServer  ${source_file} ${header_file})
target_include_directories(ChattingServer PUBLIC include jsoncpp/include ada/include)

include(FetchContent)
FetchContent_Declare(
          Boost
          URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
          URL_MD5 893b5203b862eb9bbd08553e24ff146a
          DOWNLOAD_EXTRACT_TIMESTAMP ON
)
#set(BOOST_INCLUDE_LIBRARIES timer filesystem regex)
FetchContent_MakeAvailable(Boost)

target_link_libraries(ChattingServer  PRIVATE 
            Boost::system 
            Boost::thread   
            Boost::asio
            Boost::beast
            Boost::uuid
            Boost::filesystem
            Boost::program_options
)

include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(jsoncpp)
target_link_libraries(ChattingServer PUBLIC jsoncpp_object)

add_subdirectory(ada)
target_link_libraries(ChattingServer PUBLIC ada-singleheader-lib)