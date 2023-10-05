project(redis)

add_library(${PROJECT_NAME} STATIC Redis.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
        )

include(FetchContent)
FetchContent_Declare(
    redis++
    GIT_REPOSITORY https://github.com/sewenew/redis-plus-plus
    GIT_TAG        1.3.10
)
FetchContent_MakeAvailable(redis++)

target_link_libraries(${PROJECT_NAME} PUBLIC redis++::redis++)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
