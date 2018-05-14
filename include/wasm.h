#pragma once

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Type Definitions

typedef enum {
  WASM_VALUE_TYPE_I32,
  WASM_VALUE_TYPE_I64,
  WASM_VALUE_TYPE_F32,
  WASM_VALUE_TYPE_F64,
} wasm_value_type_t;

typedef struct {
  wasm_value_type_t *types;
  uint32_t typec;
} wasm_result_type_t;

typedef struct {
  wasm_value_type_t *params;
  uint32_t paramc;
  wasm_value_type_t *results;
  uint32_t resultc;
} wasm_function_type_t;

typedef struct {
  bool has_max;
  uint32_t min, max;
} wasm_limit_t;

typedef wasm_limit_t wasm_memory_type_t;

typedef struct {
  wasm_limit_t limit;
  /* elemtype anyfunc */
} wasm_table_type_t;

typedef enum {
  WASM_MUTABILITY_CONST,
  WASM_MUTABILITY_VARIABLE,
} wasm_mutability_t;

typedef struct {
  wasm_value_type_t value_type;
  wasm_mutability_t mutablily;
} wasm_global_type_t;

// MARK: - Parser

typedef enum {
  WASM_PARSER_NO_ERROR = 0,
  WASM_PARSER_REACHED_END,
  WASM_PARSER_UNEXPECTED,
  WASM_PARSER_INVALID_BIT,
  WASM_PARSER_INVALID_NAME_LENGTH,
  WASM_PARSER_CALLOC_FAILED,
} wasm_parser_error_t;

typedef char wasm_utf8_t;

typedef struct {
  uint8_t *input;
  size_t length;
} wasm_parser_t;

// MARK: Value Parsers

wasm_parser_error_t wasm_parse_uint(uint8_t bit, wasm_parser_t *parser,
                                    size_t start, uint32_t *result,
                                    size_t *end);

wasm_parser_error_t wasm_parse_sint(uint8_t bit, wasm_parser_t *parser,
                                    size_t start, int32_t *result, size_t *end);

wasm_parser_error_t wasm_parse_int(uint8_t bit, wasm_parser_t *parser,
                                   size_t start, int32_t *result, size_t *end);

wasm_parser_error_t wasm_parse_name(wasm_parser_t *parser, size_t start,
                                    wasm_utf8_t **result, size_t *end);

// MARK: Type Parsers

wasm_parser_error_t wasm_parse_value_type(wasm_parser_t *parser, size_t start,
                                          wasm_value_type_t *result,
                                          size_t *end);

wasm_parser_error_t wasm_parse_result_type(wasm_parser_t *parser, size_t start,
                                           wasm_result_type_t *result,
                                           size_t *end);

wasm_parser_error_t wasm_parse_function_type(wasm_parser_t *parser,
                                             size_t start,
                                             wasm_function_type_t *result,
                                             size_t *end);

wasm_parser_error_t wasm_parse_limit(wasm_parser_t *parser, size_t start,
                                     wasm_limit_t *result, size_t *end);

wasm_parser_error_t wasm_parse_memory_type(wasm_parser_t *parser, size_t start,
                                           wasm_memory_type_t *result,
                                           size_t *end);

wasm_parser_error_t wasm_parse_table_type(wasm_parser_t *parser, size_t start,
                                          wasm_table_type_t *result,
                                          size_t *end);

wasm_parser_error_t wasm_parse_global_type(wasm_parser_t *parser, size_t start,
                                           wasm_global_type_t *result,
                                           size_t *end);
