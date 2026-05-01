cmake_minimum_required(VERSION 3.16)

project(CurrencyConverter VERSION 0.8.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(converter main.cpp)

# Link curl
find_package(CURL REQUIRED)
target_link_libraries(converter PRIVATE CURL::libcurl)

# nlohmann/json (header-only, no linking needed usually)
find_package(nlohmann_json REQUIRED)
target_link_libraries(converter PRIVATE nlohmann_json::nlohmann_json)