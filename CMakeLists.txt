cmake_minimum_required(VERSION 3.0)

project(chess-vr CXX)

# Set a default build type if none was specified
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

#------------------------------------------------------

set(Sources 
        src/main.cpp
)

include_directories(
    src
    libs
)

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -DUSE_SYSLOG -I external/yaml-cpp/include")
set(CMAKE_CXX_FLAGS_DEBUG   "-DDEBUG -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#------------------------------------------------------

add_executable(chess-vr ${Sources})

target_link_libraries(chess-vr pthread)

add_custom_target(
        generate_version
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/gen_version.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_dependencies(chess-vr generate_version)
