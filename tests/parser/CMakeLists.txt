add_executable(test_parser main.c)
target_link_libraries(test_parser wasm)
add_test(NAME test_parser COMMAND $<TARGET_FILE:test_parser>)
