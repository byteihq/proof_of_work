cmake_minimum_required(VERSION 3.13)

project(proxy_app LANGUAGES CXX)

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
add_compile_definitions(
    ${CMAKE_BUILD_TYPE_UPPER}
    )

add_executable(${PROJECT_NAME} main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

add_subdirectory(${CMAKE_SOURCE_DIR}/app)
target_link_libraries(${PROJECT_NAME} PRIVATE proxy)

file(COPY config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY config_debug.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
