add_subdirectory(parser)

foreach(name IN ITEMS
  fib
)
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} wasm)
  add_test(NAME ${name} COMMAND ${name})
endforeach()
