#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,  // vector length
      0x01,
      0x02,  // function indices
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t indexc;
  uint32_t *indexv;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_function_section(parser, 0, &indexc, &indexv, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(indexc == 2);
  assert(indexv[0] == 1);
  assert(indexv[1] == 2);
}
