cmake_minimum_required(VERSION 3.16)

project(VizuCore LANGUAGES CXX)

set(SOURCE_FILE_LIST
"src/SimulationMap.cpp"
"src/Object.cpp"
"src/Platform.cpp"
"src/Ball.cpp"
)

add_library(VizuCore ${SOURCE_FILE_LIST})

target_include_directories(VizuCore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set C++ dialect version
target_compile_features(VizuCore PRIVATE cxx_std_17)