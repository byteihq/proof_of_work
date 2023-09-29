cmake_minimum_required(VERSION 3.13)

project(proxy LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  drogon
  GIT_REPOSITORY https://github.com/drogonframework/drogon
  GIT_TAG        94ca651cbdbdf4038bc0392bcf2d897661996f7c # release-1.9.0
)
FetchContent_MakeAvailable(drogon)

add_executable(${PROJECT_NAME} main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

add_subdirectory(${CMAKE_SOURCE_DIR}/app)

target_link_libraries(${PROJECT_NAME} PRIVATE drogon hashcash request_handler)
