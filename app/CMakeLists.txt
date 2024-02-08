cmake_minimum_required(VERSION 3.16)

project(VizuApp LANGUAGES CXX)

set(SOURCE_FILE_LIST 
"src/Main.cpp"
"src/SoundProcessing.h"
"src/SoundProcessing.cpp"
)

add_executable(VizuApp ${SOURCE_FILE_LIST})

# Set C++ dialect version
target_compile_features(VizuApp PRIVATE cxx_std_20)

# Linking core library
target_link_libraries(VizuApp PRIVATE VizuCore)

# Fetching SFML from the official git repository
include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)

FetchContent_MakeAvailable(SFML)

# Linking SFML
target_link_libraries(VizuApp PRIVATE sfml-graphics)
target_link_libraries(VizuApp PRIVATE sfml-audio)

if(WIN32)
    add_custom_command(
        TARGET VizuApp
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:VizuApp>
        VERBATIM)
endif()