cmake_minimum_required(VERSION 3.16)

project(nbody LANGUAGES CXX)

# Source File Definitions
set(SOURCE_FILES
    src/main.cpp
    src/particle.cpp
    src/BHTree.cpp
    src/gravity.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

install(TARGETS ${PROJECT_NAME})

# SFML Setup
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)