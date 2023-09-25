
add_subdirectory(HashCash)
add_subdirectory(RequestHandler)

target_link_libraries(request_handler PUBLIC hashcash)
