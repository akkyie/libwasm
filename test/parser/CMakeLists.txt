foreach(name IN ITEMS
  code_section
  data_section
  element_section
  export_section
  function_section
  function_type
  global_section
  global_type
  import_section
  instruction
  limit
  memory_section
  module
  name
  result_type
  sint
  table_section
  type_section
  uint
  value_type
)
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} wasm)
  add_test(NAME ${name} COMMAND ${name})
endforeach()
