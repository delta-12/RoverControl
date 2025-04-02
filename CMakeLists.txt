cmake_minimum_required(VERSION 3.28)
project(RoverControl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_compile_definitions(
    $<$<CONFIG:Debug>:LOGGER>
)

option(ABSL_PROPAGATE_CXX_STD "Propagate C++ standard to abseil-cpp" ON)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(CAVE_TALK_DIR ${EXTERNAL_DIR}/CAVeTalk)
set(CAVE_TALK_BUILD_DIR ${CAVE_TALK_DIR}/build) 
set(PROTO_DIR ${CAVE_TALK_BUILD_DIR}/CAVeTalk-cpp_protos)
set(CAVEHEADER_DIR ${CAVE_TALK_DIR}/lib/c++/inc)
set(CAVECOMMON_DIR ${CAVE_TALK_DIR}/lib/common/inc)
# set(PROTOBUF_INSTALL_DIR ${EXTERNAL_DIR}/protobuf/_build/protobuf-install)
set(TOOLS_DIR ${CMAKE_SOURCE_DIR}/tools)

# set(PROTOBUF_INC_DIRS
    # ${CMAKE_CURRENT_BINARY_DIR}
    # ${PROTOBUF_INSTALL_DIR}/include
# )
set(INC_DIR ${CMAKE_SOURCE_DIR}/inc)

set(EXTERNAL_INC_DIRS
    # ${PROTOBUF_INC_DIRS}
    ${CAVE_TALK_DIR}/
)
set(INTERNAL_INC_DIRS
    ${INC_DIR}
)
set(INC_DIRS
    ${EXTERNAL_INC_DIRS}
    ${INTERNAL_INC_DIRS}
)

file(GLOB SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cc"
)

find_package(SDL2 REQUIRED)
find_package(serial REQUIRED)

include(${TOOLS_DIR}/uncrustify/uncrustify.cmake)

################################################################################
# RoverControl build
################################################################################
add_executable(RoverControl ${SRCS})
target_link_libraries(RoverControl PRIVATE SDL2::SDL2)
target_include_directories(RoverControl PRIVATE ${INC_DIRS})
# target_compile_options(RoverControl BEFORE PRIVATE -Wall -Wextra -Werror -Wpedantic -fdiagnostics-color=always)