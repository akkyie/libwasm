cmake_minimum_required(VERSION 3.0)
project(wasm C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

include_directories(include)
set(SRCS
  src/execution/allocation.c
  src/parser/instructions.c
  src/parser/module.c
  src/parser/types.c
  src/parser/values.c
  src/print/print.c
)
add_library(wasm ${SRCS})

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()
