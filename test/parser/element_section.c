#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,  // vector length
      0x12,  // table index
      0x0B,  // expression end
      0x01,  // vector length
      0x34,  // function index
      0x56,  // table index
      0x0B,  // expression end
      0x01,  // vector length
      0x78,  // function index
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t elementc;
  wasm_element_t *elementv;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_element_section(parser, 0, &elementc, &elementv, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(elementc == 2);

  assert(elementv[0].table == 18);
  assert(elementv[0].offset.instructionc == 0);
  assert(elementv[0].initc == 1);
  assert(elementv[0].initv[0] == 52);

  assert(elementv[1].table == 86);
  assert(elementv[1].offset.instructionc == 0);
  assert(elementv[1].initc == 1);
  assert(elementv[1].initv[0] == 120);
}
