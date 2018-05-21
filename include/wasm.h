#pragma once

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Type Definitions
 */

typedef enum {
  WASM_VALUE_TYPE_I32,
  WASM_VALUE_TYPE_I64,
  WASM_VALUE_TYPE_F32,
  WASM_VALUE_TYPE_F64,
} wasm_value_type;

typedef struct {
  wasm_value_type *typev;
  uint32_t typec;
} wasm_result_type;

typedef struct {
  wasm_value_type *paramv;
  uint32_t paramc;
  wasm_value_type *resultv;
  uint32_t resultc;
} wasm_function_type;

typedef struct {
  bool has_max;
  uint32_t min, max;
} wasm_limit;

typedef wasm_limit wasm_memory_type;

typedef struct {
  wasm_limit limit;
  /* elemtype anyfunc */
} wasm_table_type;

typedef enum {
  WASM_MUTABILITY_CONST,
  WASM_MUTABILITY_VARIABLE,
} wasm_mutability;

typedef struct {
  wasm_value_type value_type;
  wasm_mutability mutablily;
} wasm_global_type;

/*
 * Instruction Definitions
 */

typedef union {
} wasm_instruction;

typedef struct {
  wasm_instruction *instructionv;
  size_t instructionc;
} wasm_expression;

/*
 * Module Definitions
 */

typedef struct {
  uint32_t index;

  wasm_value_type *localv;
  uint32_t *localc;
} wasm_function;

typedef struct {
  char *name;

  enum {
    WASM_EXPORT_FUNCTION,
    WASM_EXPORT_TABLE,
    WASM_EXPORT_MEMORY,
    WASM_EXPORT_GLOBAL,
  } export_type;

  uint32_t index;
} wasm_export;

typedef struct {
  uint32_t table;
  wasm_expression offset;
  uint32_t *initv;
  uint32_t initc;
} wasm_element;

typedef enum {
  WASM_SECTION_ID_CUSTOM = 0,
  WASM_SECTION_ID_TYPE = 1,
  WASM_SECTION_ID_IMPORT = 2,
  WASM_SECTION_ID_FUNCTION = 3,
  WASM_SECTION_ID_TABLE = 4,
  WASM_SECTION_ID_MEMORY = 5,
  WASM_SECTION_ID_GLOBAL = 6,
  WASM_SECTION_ID_EXPORT = 7,
  WASM_SECTION_ID_START = 8,
  WASM_SECTION_ID_ELEMENT = 9,
  WASM_SECTION_ID_CODE = 10,
  WASM_SECTION_ID_DATA = 11,
} wasm_section;

static uint8_t const WASM_SECTION_ID_MAX = WASM_SECTION_ID_DATA;

typedef struct {
  wasm_function_type *typev;
  uint32_t typec;

  wasm_function *functionv;
  uint32_t functionc;

  wasm_table_type *tablev;
  uint32_t tablec;

  wasm_memory_type *memoryv;
  uint32_t memoryc;

  wasm_global_type *globalv;
  uint32_t globalc;

  wasm_export *exportv;
  uint32_t exportc;
} wasm_module;

/*
 * Parsers
 */

typedef enum {
  WASM_PARSER_NO_ERROR = 0,
  WASM_PARSER_REACHED_END,
  WASM_PARSER_UNEXPECTED,
  WASM_PARSER_INVALID_BIT,
  WASM_PARSER_INVALID_NAME_LENGTH,
  WASM_PARSER_CALLOC_FAILED,
  WASM_PARSER_INVALID_SECTION_ID,
} wasm_parser_error;

typedef char wasm_utf8;

typedef struct {
  uint8_t *input;
  size_t length;
} wasm_parser;

// Value Parsers

wasm_parser_error wasm_parse_uint(uint8_t bit,
                                  wasm_parser const parser[static 1],
                                  size_t start, uint32_t *result, size_t *end);

wasm_parser_error wasm_parse_sint(uint8_t bit,
                                  wasm_parser const parser[static 1],
                                  size_t start, int32_t *result, size_t *end);

wasm_parser_error wasm_parse_int(uint8_t bit,
                                 wasm_parser const parser[static 1],
                                 size_t start, int32_t *result, size_t *end);

wasm_parser_error wasm_parse_name(wasm_parser const parser[static 1],
                                  size_t start, wasm_utf8 **result,
                                  size_t *end);

// Type Parsers

wasm_parser_error wasm_parse_value_type(wasm_parser const parser[static 1],
                                        size_t start, wasm_value_type *result,
                                        size_t *end);

wasm_parser_error wasm_parse_result_type(wasm_parser const parser[static 1],
                                         size_t start, wasm_result_type *result,
                                         size_t *end);

wasm_parser_error wasm_parse_function_type(wasm_parser const parser[static 1],
                                           size_t start,
                                           wasm_function_type *result,
                                           size_t *end);

wasm_parser_error wasm_parse_limit(wasm_parser const parser[static 1],
                                   size_t start, wasm_limit *result,
                                   size_t *end);

wasm_parser_error wasm_parse_memory_type(wasm_parser const parser[static 1],
                                         size_t start, wasm_memory_type *result,
                                         size_t *end);

wasm_parser_error wasm_parse_table_type(wasm_parser const parser[static 1],
                                        size_t start, wasm_table_type *result,
                                        size_t *end);

wasm_parser_error wasm_parse_global_type(wasm_parser const parser[static 1],
                                         size_t start, wasm_global_type *result,
                                         size_t *end);

// Module Parsers
wasm_parser_error wasm_parse_module(wasm_parser const parser[static 1],
                                    size_t start, wasm_module **module,
                                    size_t *end);
