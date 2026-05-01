cmake_minimum_required(VERSION 3.16)

project(CurrencyConverter VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_add_executable(converter
    main.cpp
)

qt_add_qml_module(converter
    URI CurrencyConverter
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_link_libraries(converter PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)