
project(hashcash)

add_library(${PROJECT_NAME} STATIC HashCash.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
        )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/app/Proxy)
include(FetchContent)
FetchContent_Declare(
        picosha2
        GIT_REPOSITORY https://github.com/okdshin/PicoSHA2
        GIT_TAG        27fcf6979298949e8a462e16d09a0351c18fcaf2
)
FetchContent_MakeAvailable(picosha2)

target_link_libraries(${PROJECT_NAME} PRIVATE picosha2)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
