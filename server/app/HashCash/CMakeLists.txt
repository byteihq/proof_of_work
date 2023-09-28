
project(hashcash)

add_library(${PROJECT_NAME} STATIC HashCash.cpp HashCash.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
        )

include(FetchContent)
FetchContent_Declare(
        picosha2
        GIT_REPOSITORY https://github.com/okdshin/PicoSHA2
        GIT_TAG        27fcf6979298949e8a462e16d09a0351c18fcaf2
)
FetchContent_Declare(
        redis++
        GIT_REPOSITORY https://github.com/sewenew/redis-plus-plus
        GIT_TAG        1.3.10
)
FetchContent_MakeAvailable(picosha2 redis++)

target_link_libraries(${PROJECT_NAME} PUBLIC picosha2 redis++::redis++)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
