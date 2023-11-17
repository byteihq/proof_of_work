
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        v1.12.0
)
FetchContent_MakeAvailable(spdlog)


add_subdirectory(HashCash)
add_subdirectory(RequestHandler)
add_subdirectory(Timer)
add_subdirectory(UserInfo)
add_subdirectory(TypeDefs)
add_subdirectory(Config)
add_subdirectory(Redis)
add_subdirectory(Proxy)

target_link_libraries(config PUBLIC spdlog::spdlog_header_only)
target_link_libraries(redis PUBLIC config)
target_link_libraries(user_info PUBLIC timer)
target_link_libraries(hashcash PUBLIC typedefs user_info)
target_link_libraries(request_handler PUBLIC hashcash)
target_link_libraries(proxy PUBLIC redis hashcash request_handler timer user_info config spdlog::spdlog_header_only)
