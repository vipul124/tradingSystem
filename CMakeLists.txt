cmake_minimum_required(VERSION 3.10)
project(tradingApp)

find_package(CURL REQUIRED)
find_package(websocketpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(main main.cpp)
add_executable(webServer webServer.cpp)

target_link_libraries(main PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
target_link_libraries(webServer PRIVATE CURL::libcurl websocketpp::websocketpp OpenSSL::SSL OpenSSL::Crypto nlohmann_json::nlohmann_json)