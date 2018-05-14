cmake_minimum_required(VERSION 3.0)
project(wasm C)

include_directories(include)

add_library(wasm
  src/parser/types.c
  src/parser/values.c
)

enable_testing()

add_subdirectory(tests)
