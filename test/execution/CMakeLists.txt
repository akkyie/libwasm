foreach(name IN ITEMS
  invocation
  stack
)
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} wasm)
  add_test(NAME ${name} COMMAND ${name})
endforeach()
