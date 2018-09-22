#include <wasm.h>

wasm_parser_error_t wasm_parse_instruction(wasm_parser_t const parser[static 1],
                                           size_t start,
                                           wasm_instruction_t *instruction,
                                           size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint8_t opcode = parser->input[start];
  *end = start + 1;

  if (opcode >= WASM_OPCODE_I32_EQZ &&
      opcode <= WASM_OPCODE_F64_REINTERPRET_I64) {
    *instruction = (wasm_instruction_t){.opcode = opcode};
    return WASM_PARSER_NO_ERROR;
  }

  wasm_parser_error_t error = WASM_PARSER_NO_ERROR;

  switch (opcode) {
    case WASM_OPCODE_UNREACHABLE:
    case WASM_OPCODE_NOP:
    case WASM_OPCODE_RETURN:
    case WASM_OPCODE_DROP:
    case WASM_OPCODE_SELECT: {
      *instruction = (wasm_instruction_t){.opcode = opcode};
      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_BLOCK:
    case WASM_OPCODE_LOOP: {
      wasm_result_type_t result;
      error = wasm_parse_result_type(parser, *end, &result, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      wasm_expression_t *expression =
          (wasm_expression_t *)calloc(1, sizeof(wasm_expression_t *));

      error = wasm_parse_expression_body(parser, *end, expression, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      *end += 1;  // consume end

      wasm_block_argument_t *argument =
          (wasm_block_argument_t *)calloc(1, sizeof(wasm_block_argument_t));
      argument->type = result;
      argument->expression = expression;

      *instruction =
          (wasm_instruction_t){.opcode = opcode, .argument.block = argument};
      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_IF: {
      wasm_result_type_t result;
      error = wasm_parse_result_type(parser, *end, &result, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      wasm_expression_t expression1;
      error = wasm_parse_expression_body(parser, *end, &expression1, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      wasm_if_else_argument_t argument;
      if (parser->input[*end] == WASM_OPCODE_ELSE) {
        *end += 1;  // consume else

        wasm_expression_t expression2;
        error = wasm_parse_expression_body(parser, *end, &expression2, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        *end += 1;  // consume end

        argument = (wasm_if_else_argument_t){.type = result,
                                             .expression1 = &expression1,
                                             .expression2 = &expression2};
      } else {
        *end += 1;  // consume end

        argument = (wasm_if_else_argument_t){
            .type = result, .expression1 = &expression1, .expression2 = NULL};
      }

      *instruction =
          (wasm_instruction_t){.opcode = opcode, .argument.if_else = &argument};
      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_BR:
    case WASM_OPCODE_BRIF:
    case WASM_OPCODE_CALL:
    case WASM_OPCODE_GETLOCAL:
    case WASM_OPCODE_SETLOCAL:
    case WASM_OPCODE_TEELOCAL:
    case WASM_OPCODE_GETGLOBAL:
    case WASM_OPCODE_SETGLOBAL: {
      uint32_t index;
      error = wasm_parse_uint(32, parser, *end, &index, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      *instruction =
          (wasm_instruction_t){.opcode = opcode, .argument.index = index};
      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_BRTABLE: {
      uint32_t length;
      error = wasm_parse_uint(32, parser, *end, &length, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      uint32_t indexv[length];

      for (uint32_t i = 0; i < length; i++) {
        error = wasm_parse_uint(32, parser, *end, indexv + i, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
      }

      uint32_t index;
      error = wasm_parse_uint(32, parser, *end, &index, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      wasm_br_table_argument_t argument = {
          .indexv = indexv, .indexc = length, .index = index};

      *instruction = (wasm_instruction_t){.opcode = opcode,
                                          .argument.br_table = &argument};
      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_CALLINDIRECT: {
      uint32_t index;
      error = wasm_parse_uint(32, parser, *end, &index, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      if (parser->input[*end] != 0x00) return WASM_PARSER_INVALID_BIT;
      *end += 1;

      *instruction =
          (wasm_instruction_t){.opcode = opcode, .argument.index = index};
      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_I32_LOAD:
    case WASM_OPCODE_I64_LOAD:
    case WASM_OPCODE_F32_LOAD:
    case WASM_OPCODE_F64_LOAD:
    case WASM_OPCODE_I32_LOAD_8_S:
    case WASM_OPCODE_I32_LOAD_8_U:
    case WASM_OPCODE_I32_LOAD_16_S:
    case WASM_OPCODE_I32_LOAD_16_U:
    case WASM_OPCODE_I64_LOAD_8_S:
    case WASM_OPCODE_I64_LOAD_8_U:
    case WASM_OPCODE_I64_LOAD_16_S:
    case WASM_OPCODE_I64_LOAD_16_U:
    case WASM_OPCODE_I64_LOAD_32_S:
    case WASM_OPCODE_I64_LOAD_32_U:
    case WASM_OPCODE_I32_STORE:
    case WASM_OPCODE_I64_STORE:
    case WASM_OPCODE_F32_STORE:
    case WASM_OPCODE_F64_STORE:
    case WASM_OPCODE_I32_STORE_8:
    case WASM_OPCODE_I32_STORE_16:
    case WASM_OPCODE_I64_STORE_8:
    case WASM_OPCODE_I64_STORE_16:
    case WASM_OPCODE_I64_STORE_32: {
      uint32_t alignment;
      error = wasm_parse_uint(32, parser, *end, &alignment, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      uint32_t offset;
      error = wasm_parse_uint(32, parser, *end, &offset, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      wasm_memory_argument_t argument = {alignment, offset};
      *instruction =
          (wasm_instruction_t){.opcode = opcode, .argument.memory = &argument};

      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_MEMORY_SIZE: {
      if (parser->input[*end] != 0x00) return WASM_PARSER_INVALID_BIT;
      *end += 1;  // consume 0x00
      *instruction = (wasm_instruction_t){.opcode = opcode};
      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_I32: {
      *instruction = (wasm_instruction_t){.opcode = opcode};
      int32_t value;
      error = wasm_parse_int(32, parser, *end, &value, end);
      if (error != WASM_PARSER_NO_ERROR) return error;
      *instruction =
          (wasm_instruction_t){.opcode = opcode, .argument.const_i32 = value};
      return WASM_PARSER_NO_ERROR;
    }

    case WASM_OPCODE_I64:
    case WASM_OPCODE_F32:
    case WASM_OPCODE_F64:
      abort();  // unimplemented
  }

  return WASM_PARSER_UNEXPECTED;
}

wasm_parser_error_t wasm_parse_next_instruction(
    wasm_parser_t const parser[static 1], size_t start,
    wasm_instruction_t **instructionv, size_t *instructionc, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  *instructionc += 1;
  wasm_instruction_t *tmp =
      realloc(*instructionv, *instructionc * sizeof(wasm_instruction_t));
  if (tmp == NULL) {
    free(*instructionv);
    return WASM_PARSER_ALLOC_FAILED;
  }
  *instructionv = tmp;
  return wasm_parse_instruction(parser, start,
                                *instructionv + (*instructionc - 1), end);
}

// last byte (end or else) not included
wasm_parser_error_t wasm_parse_expression_body(
    wasm_parser_t const parser[static 1], size_t start,
    wasm_expression_t *expression, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  size_t instructionc = 0;
  wasm_instruction_t *instructionv = NULL;

  *end = start;

  for (uint8_t c = parser->input[start];
       (c != WASM_OPCODE_END && c != WASM_OPCODE_ELSE);
       c = parser->input[*end]) {
    wasm_parser_error_t error = wasm_parse_next_instruction(
        parser, *end, &instructionv, &instructionc, end);
    if (error != WASM_PARSER_NO_ERROR) return error;
  }

  expression->instructionc = instructionc;
  expression->instructionv = instructionv;

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_expression(wasm_parser_t const parser[static 1],
                                          size_t start,
                                          wasm_expression_t *expression,
                                          size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  wasm_parser_error_t error =
      wasm_parse_expression_body(parser, start, expression, end);

  if (error != WASM_PARSER_NO_ERROR) return error;

  *end += 1;  // consume end or else;

  return WASM_PARSER_NO_ERROR;
}
