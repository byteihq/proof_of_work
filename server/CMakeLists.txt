cmake_minimum_required(VERSION 3.13)

project(server LANGUAGES CXX)
set(BUILD_FROM_SOURCE ON)

if (BUILD_FROM_SOURCE)
  include(FetchContent)
  FetchContent_Declare(
    drogon
    GIT_REPOSITORY https://github.com/drogonframework/drogon
    GIT_TAG        94ca651cbdbdf4038bc0392bcf2d897661996f7c # release-1.9.0
  )
  FetchContent_MakeAvailable(drogon)
  set(drogon_pack drogon)
else()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prebuild)
  get_property(drogon_lib GLOBAL PROPERTY drogon_lib_property)
  get_property(trantor_lib GLOBAL PROPERTY trantor_lib_property)
  find_package(jsoncpp REQUIRED)
  find_package(ZLIB REQUIRED)
  find_package(CURL REQUIRED)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(UUID REQUIRED uuid)

  set(drogon_pack drogon ${drogon_lib} ${trantor_lib} jsoncpp ZLIB::ZLIB uuid ${CURL_LIBRARIES})
endif()

add_executable(${PROJECT_NAME} main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

add_subdirectory(app)
target_link_libraries(${PROJECT_NAME} PUBLIC ${drogon_pack} api)
