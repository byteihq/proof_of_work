
add_subdirectory(HashCash)
add_subdirectory(RequestHandler)
add_subdirectory(TypeDefs)

target_link_libraries(hashcash PUBLIC typedefs)
target_link_libraries(request_handler PRIVATE hashcash)
