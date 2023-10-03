
add_subdirectory(HashCash)
add_subdirectory(RequestHandler)
add_subdirectory(Timer)
add_subdirectory(UserInfo)
add_subdirectory(TypeDefs)

target_link_libraries(user_info PUBLIC timer)
target_link_libraries(hashcash PUBLIC typedefs user_info)
target_link_libraries(request_handler PRIVATE hashcash)
