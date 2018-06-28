cmake_minimum_required(VERSION 2.8.10)
project(pbconf CXX)

#set(BOOST_INCLUDE "/usr/local/Cellar/boost\@1.60/1.60.0/")
#set(BOOST_LIB "/usr/local/Cellar/boost\@1.60/1.60.0/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BOOST_INCLUDE}")
#SET(CMAKE_LDPATH "${CMAKE_LDPATH} -L${BOOST_LIB}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)

include(FindProtobuf)

find_path(BRPC_INCLUDE_PATH NAMES brpc/server.h)
find_library(BRPC_LIB NAMES libbrpc.a brpc)
if ((NOT BRPC_INCLUDE_PATH) OR (NOT BRPC_LIB))
    message(FATAL_ERROR "Fail to find brpc")
endif()

find_path(YAML_CPP_INCLUDE_PATH NAMES yaml-cpp/yaml.h)
find_library(YAML_CPP_LIB NAMES libyaml-cpp.a yaml-cpp)
if ((NOT YAML_CPP_INCLUDE_PATH) OR (NOT YAML_CPP_LIB))
    message(FATAL_ERROR "Fail to find yaml-cpp")
endif()

find_path(HOCON_INCLUDE_PATH NAMES hocon/version.h)
find_library(HOCON_LIB NAMES libcpp-hocon.a cpp-hocon)
if ((NOT HOCON_INCLUDE_PATH) OR (NOT HOCON_LIB))
    message(FATAL_ERROR "Fail to find hocon")
endif()

#set(LEATHERMAN_COMPONENTS locale catch nowide util)
#find_package(Leatherman REQUIRED COMPONENTS ${LEATHERMAN_COMPONENTS})
#
#set(Boost_USE_STATIC_LIBS        ON) # only find static libs
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)

set(LEATHERMAN_COMPONENTS locale catch nowide util)
find_package(Leatherman REQUIRED COMPONENTS ${LEATHERMAN_COMPONENTS})

#set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH} ${BOOST_LIB}")
#set(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH} ${BOOST_INCLUDE}")
#find_path(BOOST_INCLUDE_VERSION NAMES boost/version.hpp)
#find_library(BOOST_TIMER_LIB NAMES libboost_timer.a boost_timer)
#if ((NOT BOOST_INCLUDE_VERSION))
#    message(FATAL_ERROR "Fail to find boost")
#endif()

find_package(Boost 1.54 COMPONENTS log locale thread date_time chrono system program_options)

# Publish headers
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/pbconf/
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/output/include/pbconf/
    FILES_MATCHING 
    PATTERN "*.h"
    )

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/include/
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
    )

# Publish libraries
file(GLOB_RECURSE PBCONF_LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/pbconf/*.cpp")
add_library(pbconf STATIC ${PBCONF_LIB_SOURCES})

# demo
file(GLOB DEMO_PROTOS "${CMAKE_SOURCE_DIR}/src/example/proto/*.proto")
foreach(PROTO ${DEMO_PROTOS})
    get_filename_component(PROTO_WE ${PROTO} NAME_WE)
    list(APPEND PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/example/proto/${PROTO_WE}.pb.cc")
    execute_process(
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS}
        --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
        --proto_path=${PROTOBUF_INCLUDE_DIR}
        --proto_path=${CMAKE_SOURCE_DIR}/src
        --proto_path=${CMAKE_SOURCE_DIR}/src/example/proto/ ${PROTO}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        ERROR_VARIABLE PROTO_ERROR
        RESULT_VARIABLE PROTO_RESULT
    )
    if (${PROTO_RESULT} EQUAL 0) 
    else ()
        message (FATAL_ERROR "Fail to generate cpp of ${PROTO} : ${PROTO_ERROR}")
    endif()
endforeach()

file(GLOB_RECURSE DEMO_SOURCES "${CMAKE_SOURCE_DIR}/src/example/*.cpp")
set(SOURCES
    ${DEMO_SOURCES}
    ${PROTO_SRCS}
    )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/bin)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/example/proto/")
include_directories(${Boost_INCLUDE_DIRS})

add_executable(demo ${SOURCES})
target_link_libraries(demo pbconf)
target_link_libraries(demo protobuf)
target_link_libraries(demo brpc)
target_link_libraries(demo yaml-cpp)
target_link_libraries(demo iconv)
target_link_libraries(demo ${LEATHERMAN_LIBRARIES})
target_link_libraries(demo ${Boost_LIBRARIES})
target_link_libraries(demo cpp-hocon)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/example/conf/
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/output/conf/
    FILES_MATCHING 
    PATTERN "*"
    )
# demo end
