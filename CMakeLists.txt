cmake_minimum_required(VERSION 3.20)

project(plume)

set(CMAKE_CXX_STANDARD 20)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(plume
    src/main.cpp
    src/api.cpp
)

target_link_libraries(plume
   PRIVATE
   CURL::libcurl
   nlohmann_json::nlohmann_json
)
