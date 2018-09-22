#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,        // vector length
      0x01, 0x61,  // module name
      0x01, 0x62,  // import name
      0x00, 0x12,  // import descriptor (function)
      0x01, 0x63,  // module name
      0x01, 0x64,  // import name
      0x00, 0x34,  // import descriptor (function)
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t importc;
  wasm_import_t *importv;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_import_section(parser, 0, &importc, &importv, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(importc == 2);

  assert(strcmp(importv[0].module, "a") == 0);
  assert(strcmp(importv[0].name, "b") == 0);
  assert(importv[0].type == WASM_IMPORT_FUNCTION);
  assert(importv[0].descriptor.function == 0x12);

  assert(strcmp(importv[1].module, "c") == 0);
  assert(strcmp(importv[1].name, "d") == 0);
  assert(importv[1].type == WASM_IMPORT_FUNCTION);
  assert(importv[1].descriptor.function == 0x34);
}
