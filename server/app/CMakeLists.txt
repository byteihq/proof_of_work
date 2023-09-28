
add_subdirectory(HashCash)
add_subdirectory(RequestHandler)
add_subdirectory(Timer)
add_subdirectory(Weather)

target_link_libraries(request_handler PUBLIC hashcash)
