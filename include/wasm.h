#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Type Definitions
 */

typedef char wasm_utf8_t;

typedef enum {
  WASM_VALUE_TYPE_I32,
  WASM_VALUE_TYPE_I64,
  WASM_VALUE_TYPE_F32,
  WASM_VALUE_TYPE_F64,
} wasm_value_type_t;

typedef struct {
  wasm_value_type_t type;
  union {
    int32_t i32;
    int64_t i64;
    float f32;
    double f64;
  } value;
} wasm_value_t;

typedef uint32_t type_index;
typedef uint32_t function_index;
typedef uint32_t table_index;
typedef uint32_t memory_index;
typedef uint32_t global_index;
typedef uint32_t local_index;
typedef uint32_t label_index;

typedef struct {
  wasm_value_type_t* typev;
  uint32_t typec;
} wasm_result_type_t;

typedef struct {
  wasm_value_type_t* paramv;
  uint32_t paramc;
  wasm_value_type_t* resultv;
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

/*
 * Instruction Definitions
 */

typedef enum {
  WASM_OPCODE_UNREACHABLE = 0x00,
  WASM_OPCODE_NOP = 0x01,
  WASM_OPCODE_BLOCK = 0x02,
  WASM_OPCODE_LOOP = 0x03,
  WASM_OPCODE_IF = 0x04,
  WASM_OPCODE_ELSE = 0x05,
  // RESERVED = 0x06,
  // RESERVED = 0x07,
  // RESERVED = 0x08,
  // RESERVED = 0x09,
  // RESERVED = 0x0A,
  WASM_OPCODE_END = 0x0B,
  WASM_OPCODE_BR = 0x0C,
  WASM_OPCODE_BRIF = 0x0D,
  WASM_OPCODE_BRTABLE = 0x0E,
  WASM_OPCODE_RETURN = 0x0F,
  WASM_OPCODE_CALL = 0x10,
  WASM_OPCODE_CALLINDIRECT = 0x11,
  // RESERVED = 0x12,
  // RESERVED = 0x13,
  // RESERVED = 0x14,
  // RESERVED = 0x15,
  // RESERVED = 0x16,
  // RESERVED = 0x17,
  // RESERVED = 0x18,
  // RESERVED = 0x19,
  WASM_OPCODE_DROP = 0x1A,
  WASM_OPCODE_SELECT = 0x1B,
  // RESERVED = 0x1C,
  // RESERVED = 0x1D,
  // RESERVED = 0x1E,
  // RESERVED = 0x1F,
  WASM_OPCODE_GETLOCAL = 0x20,
  WASM_OPCODE_SETLOCAL = 0x21,
  WASM_OPCODE_TEELOCAL = 0x22,
  WASM_OPCODE_GETGLOBAL = 0x23,
  WASM_OPCODE_SETGLOBAL = 0x24,
  // RESERVED = 0x25,
  // RESERVED = 0x26,
  // RESERVED = 0x27,
  WASM_OPCODE_I32_LOAD = 0x28,
  WASM_OPCODE_I64_LOAD = 0x29,
  WASM_OPCODE_F32_LOAD = 0x2A,
  WASM_OPCODE_F64_LOAD = 0x2B,
  WASM_OPCODE_I32_LOAD_8_S = 0x2C,
  WASM_OPCODE_I32_LOAD_8_U = 0x2D,
  WASM_OPCODE_I32_LOAD_16_S = 0x2E,
  WASM_OPCODE_I32_LOAD_16_U = 0x2F,
  WASM_OPCODE_I64_LOAD_8_S = 0x30,
  WASM_OPCODE_I64_LOAD_8_U = 0x31,
  WASM_OPCODE_I64_LOAD_16_S = 0x32,
  WASM_OPCODE_I64_LOAD_16_U = 0x33,
  WASM_OPCODE_I64_LOAD_32_S = 0x34,
  WASM_OPCODE_I64_LOAD_32_U = 0x35,
  WASM_OPCODE_I32_STORE = 0x36,
  WASM_OPCODE_I64_STORE = 0x37,
  WASM_OPCODE_F32_STORE = 0x38,
  WASM_OPCODE_F64_STORE = 0x39,
  WASM_OPCODE_I32_STORE_8 = 0x3A,
  WASM_OPCODE_I32_STORE_16 = 0x3B,
  WASM_OPCODE_I64_STORE_8 = 0x3C,
  WASM_OPCODE_I64_STORE_16 = 0x3D,
  WASM_OPCODE_I64_STORE_32 = 0x3E,
  WASM_OPCODE_MEMORY_SIZE = 0x3F,
  WASM_OPCODE_MEMORY_GROW = 0x40,
  WASM_OPCODE_I32 = 0x41,
  WASM_OPCODE_I64 = 0x42,
  WASM_OPCODE_F32 = 0x43,
  WASM_OPCODE_F64 = 0x44,
  WASM_OPCODE_I32_EQZ = 0x45,
  WASM_OPCODE_I32_EQ = 0x46,
  WASM_OPCODE_I32_NE = 0x47,
  WASM_OPCODE_I32_LT_S = 0x48,
  WASM_OPCODE_I32_LT_U = 0x49,
  WASM_OPCODE_I32_GT_S = 0x4A,
  WASM_OPCODE_I32_GT_U = 0x4B,
  WASM_OPCODE_I32_LE_S = 0x4C,
  WASM_OPCODE_I32_LE_U = 0x4D,
  WASM_OPCODE_I32_GE_S = 0x4E,
  WASM_OPCODE_I32_GE_U = 0x4F,
  WASM_OPCODE_I64_EQZ = 0x50,
  WASM_OPCODE_I64_EQ = 0x51,
  WASM_OPCODE_I64_NE = 0x52,
  WASM_OPCODE_I64_LT_S = 0x53,
  WASM_OPCODE_I64_LT_U = 0x54,
  WASM_OPCODE_I64_GT_S = 0x55,
  WASM_OPCODE_I64_GT_U = 0x56,
  WASM_OPCODE_I64_LE_S = 0x57,
  WASM_OPCODE_I64_LE_U = 0x58,
  WASM_OPCODE_I64_GE_S = 0x59,
  WASM_OPCODE_I64_GE_U = 0x5A,
  WASM_OPCODE_F32_EQ = 0x5B,
  WASM_OPCODE_F32_NE = 0x5C,
  WASM_OPCODE_F32_LT = 0x5D,
  WASM_OPCODE_F32_GT = 0x5E,
  WASM_OPCODE_F32_LE = 0x5F,
  WASM_OPCODE_F32_GE = 0x60,
  WASM_OPCODE_F64_EQ = 0x61,
  WASM_OPCODE_F64_NE = 0x62,
  WASM_OPCODE_F64_LT = 0x63,
  WASM_OPCODE_F64_GT = 0x64,
  WASM_OPCODE_F64_LE = 0x65,
  WASM_OPCODE_F64_GE = 0x66,
  WASM_OPCODE_I32_CLZ = 0x67,
  WASM_OPCODE_I32_CTZ = 0x68,
  WASM_OPCODE_I32_POPCNT = 0x69,
  WASM_OPCODE_I32_ADD = 0x6A,
  WASM_OPCODE_I32_SUB = 0x6B,
  WASM_OPCODE_I32_MUL = 0x6C,
  WASM_OPCODE_I32_DIV_S = 0x6D,
  WASM_OPCODE_I32_DIV_U = 0x6E,
  WASM_OPCODE_I32_REM_S = 0x6F,
  WASM_OPCODE_I32_REM_U = 0x70,
  WASM_OPCODE_I32_AND = 0x71,
  WASM_OPCODE_I32_OR = 0x72,
  WASM_OPCODE_I32_XOR = 0x73,
  WASM_OPCODE_I32_SHL = 0x74,
  WASM_OPCODE_I32_SHR_S = 0x75,
  WASM_OPCODE_I32_SHR_U = 0x76,
  WASM_OPCODE_I32_ROTL = 0x77,
  WASM_OPCODE_I32_ROTR = 0x78,
  WASM_OPCODE_I64_CLZ = 0x79,
  WASM_OPCODE_I64_CTZ = 0x7A,
  WASM_OPCODE_I64_POPCNT = 0x7B,
  WASM_OPCODE_I64_ADD = 0x7C,
  WASM_OPCODE_I64_SUB = 0x7D,
  WASM_OPCODE_I64_MUL = 0x7E,
  WASM_OPCODE_I64_DIV_S = 0x7F,
  WASM_OPCODE_I64_DIV_U = 0x80,
  WASM_OPCODE_I64_REM_S = 0x81,
  WASM_OPCODE_I64_REM_U = 0x82,
  WASM_OPCODE_I64_AND = 0x83,
  WASM_OPCODE_I64_OR = 0x84,
  WASM_OPCODE_I64_XOR = 0x85,
  WASM_OPCODE_I64_SHL = 0x86,
  WASM_OPCODE_I64_SHR_S = 0x87,
  WASM_OPCODE_I64_SHR_U = 0x88,
  WASM_OPCODE_I64_ROTL = 0x89,
  WASM_OPCODE_I64_ROTR = 0x8A,
  WASM_OPCODE_F32_ABS = 0x8B,
  WASM_OPCODE_F32_NEG = 0x8C,
  WASM_OPCODE_F32_CEIL = 0x8D,
  WASM_OPCODE_F32_FLOOR = 0x8E,
  WASM_OPCODE_F32_TRUNC = 0x8F,
  WASM_OPCODE_F32_NEAREST = 0x90,
  WASM_OPCODE_F32_SQRT = 0x91,
  WASM_OPCODE_F32_ADD = 0x92,
  WASM_OPCODE_F32_SUB = 0x93,
  WASM_OPCODE_F32_MUL = 0x94,
  WASM_OPCODE_F32_DIV = 0x95,
  WASM_OPCODE_F32_FMIN = 0x96,
  WASM_OPCODE_F32_FMAX = 0x97,
  WASM_OPCODE_F32_COPYSIGN = 0x98,
  WASM_OPCODE_F64_ABS = 0x99,
  WASM_OPCODE_F64_NEG = 0x9A,
  WASM_OPCODE_F64_CEIL = 0x9B,
  WASM_OPCODE_F64_FLOOR = 0x9C,
  WASM_OPCODE_F64_TRUNC = 0x9D,
  WASM_OPCODE_F64_NEAREST = 0x9E,
  WASM_OPCODE_F64_SQRT = 0x9F,
  WASM_OPCODE_F64_ADD = 0xA0,
  WASM_OPCODE_F64_SUB = 0xA1,
  WASM_OPCODE_F64_MUL = 0xA2,
  WASM_OPCODE_F64_DIV = 0xA3,
  WASM_OPCODE_F64_FMIN = 0xA4,
  WASM_OPCODE_F64_FMAX = 0xA5,
  WASM_OPCODE_F64_COPYSIGN = 0xA6,
  WASM_OPCODE_I32_WRAP_I64 = 0xA7,
  WASM_OPCODE_I32_TRUNC_S_F32 = 0xA8,
  WASM_OPCODE_I32_TRUNC_U_F32 = 0xA9,
  WASM_OPCODE_I32_TRUNC_S_F64 = 0xAA,
  WASM_OPCODE_I32_TRUNC_U_F64 = 0xAB,
  WASM_OPCODE_I64_EXTEND_S_I32 = 0xAC,
  WASM_OPCODE_I64_EXTEND_U_I32 = 0xAD,
  WASM_OPCODE_I64_TRUNC_S_F32 = 0xAE,
  WASM_OPCODE_I64_TRUNC_U_F32 = 0xAF,
  WASM_OPCODE_I64_TRUNC_S_F64 = 0xB0,
  WASM_OPCODE_I64_TRUNC_U_F64 = 0xB1,
  WASM_OPCODE_F32_CONVERT_S_I32 = 0xB2,
  WASM_OPCODE_F32_CONVERT_U_I32 = 0xB3,
  WASM_OPCODE_F32_CONVERT_S_I64 = 0xB4,
  WASM_OPCODE_F32_CONVERT_U_I64 = 0xB5,
  WASM_OPCODE_F32_DEMOTE_F64 = 0xB6,
  WASM_OPCODE_F64_CONVERT_S_I32 = 0xB7,
  WASM_OPCODE_F64_CONVERT_U_I32 = 0xB8,
  WASM_OPCODE_F64_CONVERT_S_I64 = 0xB9,
  WASM_OPCODE_F64_CONVERT_U_I64 = 0xBA,
  WASM_OPCODE_F64_PROMOTE_F32 = 0xBB,
  WASM_OPCODE_I32_REINTERPRET_F32 = 0xBC,
  WASM_OPCODE_I64_REINTERPRET_F64 = 0xBD,
  WASM_OPCODE_F32_REINTERPRET_I32 = 0xBE,
  WASM_OPCODE_F64_REINTERPRET_I64 = 0xBF,
} wasm_opcode_t;

typedef struct {
  wasm_result_type_t type;
  /* wasm_expression_t* */ void* expression;
} wasm_block_argument_t;

typedef struct {
  wasm_result_type_t type;
  /* wasm_expression_t* */ void* expression1;
  /* wasm_expression_t* */ void* expression2;
} wasm_if_else_argument_t;

typedef struct {
  uint32_t* indexv;
  uint32_t indexc;
  uint32_t index;
} wasm_br_table_argument_t;

typedef struct {
  uint32_t alignment;
  uint32_t offset;
} wasm_memory_argument_t;

typedef union {
  uint32_t index;
  wasm_block_argument_t* block;
  wasm_if_else_argument_t* if_else;
  wasm_br_table_argument_t* br_table;
  wasm_memory_argument_t* memory;
  int32_t const_i32;
  int64_t const_i64;
  float const_f32;
  double const_f64;
} wasm_argument_t;

typedef struct {
  wasm_opcode_t opcode;
  wasm_argument_t argument;
} wasm_instruction_t;

typedef struct {
  wasm_instruction_t* instructionv;
  size_t instructionc;
} wasm_expression_t;

/*
 * Module Definitions
 */

typedef enum {
  WASM_EXPORT_FUNCTION = 0x00,
  WASM_EXPORT_TABLE = 0x01,
  WASM_EXPORT_MEMORY = 0x02,
  WASM_EXPORT_GLOBAL = 0x03,
} wasm_export_type_t;

typedef struct {
  wasm_utf8_t* name;
  wasm_export_type_t type;
  uint32_t index;
} wasm_export_t;

typedef enum {
  WASM_IMPORT_FUNCTION,
  WASM_IMPORT_TABLE,
  WASM_IMPORT_MEMORY,
  WASM_IMPORT_GLOBAL,
} wasm_import_type_t;

typedef union {
  uint32_t function;
  wasm_table_type_t* table;
  wasm_memory_type_t* memory;
  wasm_global_type_t* global;
} wasm_import_descriptor_t;

typedef struct {
  wasm_utf8_t* module;
  wasm_utf8_t* name;
  wasm_import_type_t type;
  wasm_import_descriptor_t descriptor;
} wasm_import_t;

typedef struct {
  uint32_t table;
  wasm_expression_t offset;
  uint32_t* initv;
  uint32_t initc;
} wasm_element_t;

typedef struct {
  uint32_t type;
  wasm_value_type_t* localv;
  uint32_t localc;

  wasm_expression_t expression;
} wasm_function_t;

typedef struct {
  uint32_t data;
  wasm_expression_t offset;
  uint8_t* initv;
  uint32_t initc;
} wasm_data_t;

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
} wasm_section_t;

static uint8_t const WASM_SECTION_ID_MAX = WASM_SECTION_ID_DATA;

typedef struct {
  wasm_function_type_t* typev;
  uint32_t typec;

  wasm_function_t* functionv;
  uint32_t functionc;

  wasm_table_type_t* tablev;
  uint32_t tablec;

  wasm_memory_type_t* memoryv;
  uint32_t memoryc;

  wasm_global_type_t* globalv;
  uint32_t globalc;

  wasm_element_t* elementv;
  uint32_t elementc;

  wasm_data_t* datav;
  uint32_t datac;

  uint32_t start;

  wasm_import_t* importv;
  uint32_t importc;

  wasm_export_t* exportv;
  uint32_t exportc;
} wasm_module_t;

/*
 * Parsers
 */

typedef enum {
  WASM_PARSER_NO_ERROR = 0,
  WASM_PARSER_REACHED_END,
  WASM_PARSER_UNEXPECTED,
  WASM_PARSER_INVALID_BIT,
  WASM_PARSER_INVALID_NAME_LENGTH,
  WASM_PARSER_ALLOC_FAILED,
  WASM_PARSER_INVALID_SECTION_ID,
} wasm_parser_error_t;

typedef struct {
  uint8_t* input;
  size_t length;
} wasm_parser_t;

// Value Parsers

wasm_parser_error_t wasm_parse_uint(uint8_t bit,
                                    wasm_parser_t const parser[static 1],
                                    size_t start, uint32_t* result,
                                    size_t* end);

wasm_parser_error_t wasm_parse_sint(uint8_t bit,
                                    wasm_parser_t const parser[static 1],
                                    size_t start, int32_t* result, size_t* end);

wasm_parser_error_t wasm_parse_int(uint8_t bit,
                                   wasm_parser_t const parser[static 1],
                                   size_t start, int32_t* result, size_t* end);

wasm_parser_error_t wasm_parse_name(wasm_parser_t const parser[static 1],
                                    size_t start, wasm_utf8_t** result,
                                    size_t* end);

// Type Parsers

wasm_parser_error_t wasm_parse_value_type(wasm_parser_t const parser[static 1],
                                          size_t start,
                                          wasm_value_type_t* result,
                                          size_t* end);

wasm_parser_error_t wasm_parse_result_type(wasm_parser_t const parser[static 1],
                                           size_t start,
                                           wasm_result_type_t* result,
                                           size_t* end);

wasm_parser_error_t wasm_parse_function_type(
    wasm_parser_t const parser[static 1], size_t start,
    wasm_function_type_t* result, size_t* end);

wasm_parser_error_t wasm_parse_limit(wasm_parser_t const parser[static 1],
                                     size_t start, wasm_limit_t* result,
                                     size_t* end);

wasm_parser_error_t wasm_parse_memory_type(wasm_parser_t const parser[static 1],
                                           size_t start,
                                           wasm_memory_type_t* result,
                                           size_t* end);

wasm_parser_error_t wasm_parse_table_type(wasm_parser_t const parser[static 1],
                                          size_t start,
                                          wasm_table_type_t* result,
                                          size_t* end);

wasm_parser_error_t wasm_parse_global_type(wasm_parser_t const parser[static 1],
                                           size_t start,
                                           wasm_global_type_t* result,
                                           size_t* end);

// Instruction Parsers
wasm_parser_error_t wasm_parse_instruction(wasm_parser_t const parser[static 1],
                                           size_t start,
                                           wasm_instruction_t* instruction,
                                           size_t* end);

wasm_parser_error_t wasm_parse_instructions(
    uint8_t end_marker, wasm_parser_t const parser[static 1], size_t start,
    wasm_instruction_t* instructionv, size_t* instructionc, size_t* end);

wasm_parser_error_t wasm_parse_expression_body(
    wasm_parser_t const parser[static 1], size_t start,
    wasm_expression_t* expression, size_t* end);

wasm_parser_error_t wasm_parse_expression(wasm_parser_t const parser[static 1],
                                          size_t start,
                                          wasm_expression_t* expression,
                                          size_t* end);

// Module Parsers

wasm_parser_error_t wasm_parse_custom_section(
    wasm_parser_t const parser[static 1], size_t start, wasm_module_t* module,
    size_t* end);

wasm_parser_error_t wasm_parse_type_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* typec,
    wasm_function_type_t** typev, size_t* end);

wasm_parser_error_t wasm_parse_import_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* importc,
    wasm_import_t** importv, size_t* end);

wasm_parser_error_t wasm_parse_function_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* indexc,
    uint32_t** indexv, size_t* end);

wasm_parser_error_t wasm_parse_table_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* tablec,
    wasm_table_type_t** tablev, size_t* end);

wasm_parser_error_t wasm_parse_memory_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* memoryc,
    wasm_memory_type_t** memoryv, size_t* end);

wasm_parser_error_t wasm_parse_global_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* globalc,
    wasm_global_type_t** globalv, size_t* end);

wasm_parser_error_t wasm_parse_export_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* exportc,
    wasm_export_t** exportv, size_t* end);

wasm_parser_error_t wasm_parse_start_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* index,
    size_t* end);

wasm_parser_error_t wasm_parse_element_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* elementc,
    wasm_element_t** elementv, size_t* end);

wasm_parser_error_t wasm_parse_code_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* codec,
    wasm_function_t** codev, size_t* end);

wasm_parser_error_t wasm_parse_data_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t* datac,
    wasm_data_t** datav, size_t* end);

wasm_parser_error_t wasm_parse_module(wasm_parser_t const parser[static 1],
                                      size_t start, wasm_module_t** module,
                                      size_t* end);

/*
 * Rumtime Structure
 */

// Addressess

typedef uint32_t wasm_address_t;
typedef wasm_address_t wasm_function_address_t;
typedef wasm_address_t wasm_table_address_t;
typedef wasm_address_t wasm_memory_address_t;
typedef wasm_address_t wasm_global_address_t;

typedef struct {
  wasm_function_type_t* typev;
  uint32_t typec;

  wasm_function_address_t* functionv;
  uint32_t functionc;

  wasm_table_address_t* tablev;
  uint32_t tablec;

  wasm_memory_address_t* memoryv;
  uint32_t memoryc;

  wasm_global_address_t* globalv;
  uint32_t globalc;
} wasm_module_instance_t;

typedef struct {
  wasm_function_type_t* type;
  wasm_module_instance_t* module;
  wasm_function_t* code;
} wasm_function_instance_t;

typedef struct {
  wasm_function_address_t** functionv;
  uint32_t functionc;
} wasm_table_instance_t;

typedef struct {
  uint8_t* data;
  uint8_t max;
  bool has_max;
} wasm_memory_instance_t;

typedef struct {
  wasm_value_t* value;
} wasm_global_instance_t;

typedef struct {
  wasm_function_instance_t** functionv;
  uint32_t functionc;

  wasm_table_instance_t** tablev;
  uint32_t tablec;

  wasm_memory_instance_t** memoryv;
  uint32_t memoryc;

  wasm_global_instance_t** globalv;
  uint32_t globalc;
} wasm_store_t;

wasm_function_address_t wasm_allocate_function(
    wasm_store_t* store, wasm_function_t* function,
    wasm_module_instance_t* moduleinst);

wasm_table_address_t wasm_allocate_table(wasm_store_t* store,
                                         wasm_table_type_t* table);

wasm_memory_address_t wasm_allocate_memory(wasm_store_t* store,
                                           wasm_memory_type_t* memory);

wasm_global_address_t wasm_allocate_global(wasm_store_t* store,
                                           wasm_global_type_t* global);

wasm_module_instance_t* wasm_allocate_module(wasm_store_t* store,
                                             wasm_module_t* module);

wasm_module_instance_t* wasm_instantiate_module(wasm_stack_t* stack,
                                                wasm_store_t* store,
                                                wasm_module_t* module);

/*
 * Utilities
 */

static inline void __wasm_parse_checkpoint(size_t start, const char* func) {
#ifdef DEBUG
  // printf("[0x%03zx] %s\n", start, func);
#endif
}

void wasm_print_module(wasm_module_t* module);
