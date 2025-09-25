cmake_minimum_required(VERSION 3.28)
project(Rummage LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # Link time optimisation

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

file(GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*")

add_executable("${CMAKE_PROJECT_NAME}" ${SRC})

# target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${SRC})
# target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

target_compile_features("${CMAKE_PROJECT_NAME}" PRIVATE cxx_std_17)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SFML::Graphics)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SFML::Network)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE nlohmann_json::nlohmann_json)