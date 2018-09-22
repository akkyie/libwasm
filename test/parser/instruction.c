#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x00,                                // unreachable
      0x01,                                // nop
      0x02, 0x40, 0x45, 0x46, 0x47, 0x0B,  // block[] i32.eqz i32.eq i32.ne end
      0x04, 0x40, 0x45, 0x46, 0x47, 0x0B,  // if[] i32.eqz i32.eq i32.ne end
      0x04, 0x40, 0x45, 0x46, 0x47,        // if[] i32.eqz i32.eq i32.ne
      0x05, 0x48, 0x49, 0x4A, 0x0B,  // else i32.lt_s i32.lt_u i32.gt_s end
      0x0C, 0x2A,                    // br 42
      0x0E, 0x03, 0x01, 0x02, 0x03, 0x04,  // br_table [1 2 3] 4
      0x11, 0x2A, 0x00,                    // call_indirect 42
      0x28, 0x2A, 0x18,                    // i32.load {42 24}

      0x03, 0x40,  // loop
      0x20, 0x00, 0x41, 0x7d, 0x6a, 0x10, 0x80, 0x80, 0x80, 0x80,
      0x00, 0x20, 0x01, 0x6a, 0x21, 0x01, 0x20, 0x00, 0x41, 0x7e,
      0x6a, 0x22, 0x00, 0x41, 0x03, 0x4a, 0x0d, 0x00,
      0x0b,  // end
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 0, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 1);
    assert(instruction.opcode == WASM_OPCODE_UNREACHABLE);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 1, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 2);
    assert(instruction.opcode == WASM_OPCODE_NOP);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 2, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 8);
    assert(instruction.opcode == WASM_OPCODE_BLOCK);
    assert(instruction.argument.block->type.typec == 0);

    wasm_expression_t *expression =
        (wasm_expression_t *)instruction.argument.block->expression;
    assert(expression != NULL);
    assert(expression->instructionc == 3);
    assert(expression->instructionv[0].opcode == WASM_OPCODE_I32_EQZ);
    assert(expression->instructionv[1].opcode == WASM_OPCODE_I32_EQ);
    assert(expression->instructionv[2].opcode == WASM_OPCODE_I32_NE);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 8, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 14);
    assert(instruction.opcode == WASM_OPCODE_IF);
    assert(instruction.argument.block->type.typec == 0);

    wasm_expression_t *expression1 =
        (wasm_expression_t *)instruction.argument.if_else->expression1;
    assert(expression1 != NULL);
    assert(expression1->instructionc == 3);
    assert(expression1->instructionv[0].opcode == WASM_OPCODE_I32_EQZ);
    assert(expression1->instructionv[1].opcode == WASM_OPCODE_I32_EQ);
    assert(expression1->instructionv[2].opcode == WASM_OPCODE_I32_NE);

    wasm_expression_t *expression2 =
        (wasm_expression_t *)instruction.argument.if_else->expression2;
    assert(expression2 == NULL);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 14, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 24);
    assert(instruction.opcode == WASM_OPCODE_IF);
    assert(instruction.argument.block->type.typec == 0);

    wasm_expression_t *expression1 =
        (wasm_expression_t *)instruction.argument.if_else->expression1;
    assert(expression1 != NULL);
    assert(expression1->instructionc == 3);
    assert(expression1->instructionv[0].opcode == WASM_OPCODE_I32_EQZ);
    assert(expression1->instructionv[1].opcode == WASM_OPCODE_I32_EQ);
    assert(expression1->instructionv[2].opcode == WASM_OPCODE_I32_NE);

    wasm_expression_t *expression2 =
        (wasm_expression_t *)instruction.argument.if_else->expression2;
    assert(expression2 != NULL);
    assert(expression2->instructionc == 3);
    assert(expression2->instructionv[0].opcode == WASM_OPCODE_I32_LT_S);
    assert(expression2->instructionv[1].opcode == WASM_OPCODE_I32_LT_U);
    assert(expression2->instructionv[2].opcode == WASM_OPCODE_I32_GT_S);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 24, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 26);
    assert(instruction.opcode == WASM_OPCODE_BR);
    assert(instruction.argument.index == 42);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 26, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 32);
    assert(instruction.opcode == WASM_OPCODE_BRTABLE);
    assert(instruction.argument.br_table->indexc == 3);
    assert(instruction.argument.br_table->indexv[0] == 1);
    assert(instruction.argument.br_table->indexv[1] == 2);
    assert(instruction.argument.br_table->indexv[2] == 3);
    assert(instruction.argument.br_table->index == 4);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 32, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 35);
    assert(instruction.opcode == WASM_OPCODE_CALLINDIRECT);
    assert(instruction.argument.index == 42);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 35, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 38);
    assert(instruction.opcode == WASM_OPCODE_I32_LOAD);
    assert(instruction.argument.memory->alignment == 42);
    assert(instruction.argument.memory->offset == 24);
  }
  {
    wasm_instruction_t instruction;
    size_t end = 0;
    wasm_parser_error_t error =
        wasm_parse_instruction(parser, 38, &instruction, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(end == 69);
    assert(instruction.opcode == WASM_OPCODE_LOOP);
    // assert(instruction.argument.block->);
  }
}
