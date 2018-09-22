#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,  // vector length
      0x7F,  // value type
      0x00,  // mutability.constant
      0x0B,  // expression end
      0x7E,  // value type
      0x01,  // mutability.variable
      0x0B,  // expression end
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t globalc;
  wasm_global_type_t *globalv;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_global_section(parser, 0, &globalc, &globalv, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(globalc == 2);
  assert(globalv[0].mutablily == WASM_MUTABILITY_CONST);
  assert(globalv[0].value_type == WASM_VALUE_TYPE_I32);
  assert(globalv[1].mutablily == WASM_MUTABILITY_VARIABLE);
  assert(globalv[1].value_type == WASM_VALUE_TYPE_I64);
}
