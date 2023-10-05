project(proxy)

add_library(${PROJECT_NAME} STATIC Proxy.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
        )

include(FetchContent)
FetchContent_Declare(
    drogon
    GIT_REPOSITORY https://github.com/drogonframework/drogon
    GIT_TAG        94ca651cbdbdf4038bc0392bcf2d897661996f7c # release-1.9.0
)

FetchContent_MakeAvailable(drogon)

target_link_libraries(${PROJECT_NAME} PUBLIC drogon)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
