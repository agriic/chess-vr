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
        src/pch.hpp
        src/main.cpp
        src/App.cpp
        src/App.hpp
        src/VR.cpp
        src/VR.hpp
        src/VideoInput.cpp
        src/VideoInput.hpp
        src/ConcurrentQueue.hpp
        src/Utils.cpp
        src/Utils.hpp
        src/Log.cpp
        src/Log.hpp
        src/Chess.hpp src/Chess.cpp)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV COMPONENTS core features2d highgui imgproc imgcodecs objdetect video REQUIRED )

include_directories( 
    src 
    libs
    /usr/include
    /usr/local/include
)

if(EXISTS "/opt/local/lib")
    link_directories(/opt/local/lib)
endif()
if(EXISTS "/usr/local/lib")
    link_directories(/usr/local/lib)
endif()

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -DLWS_NO_EXTENSIONS")
set(CMAKE_CXX_FLAGS_DEBUG   "-DDEBUG -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#------------------------------------------------------

add_executable(chess-vr ${Sources})

set_target_properties(chess-vr PROPERTIES COMPILE_FLAGS "-include pch.hpp ")

target_link_libraries(chess-vr ${OpenCV_LIBS} pthread)

add_custom_target(
        generate_version
        COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/gen_version.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_dependencies(chess-vr generate_version)

set_target_properties(chess-vr PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
)
