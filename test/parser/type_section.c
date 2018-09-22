#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,                          // vector length
      0x60, 0x01, 0x7F, 0x01, 0x7E,  // function type
      0x60, 0x01, 0x7D, 0x01, 0x7C,  // function type
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t typec;
  wasm_function_type_t *typev;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_type_section(parser, 0, &typec, &typev, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(typec == 2);

  assert(typev[0].paramc == 1);
  assert(typev[0].paramv[0] == WASM_VALUE_TYPE_I32);
  assert(typev[0].resultc == 1);
  assert(typev[0].resultv[0] == WASM_VALUE_TYPE_I64);

  assert(typev[1].paramc == 1);
  assert(typev[1].paramv[0] == WASM_VALUE_TYPE_F32);
  assert(typev[1].resultc == 1);
  assert(typev[1].resultv[0] == WASM_VALUE_TYPE_F64);
}
