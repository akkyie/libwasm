#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x01,        // vector length
      0x01, 0x61,  // name
      0x00, 0x12,  // export descriptor
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t exportc;
  wasm_export_t *exportv;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_export_section(parser, 0, &exportc, &exportv, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(exportc == 1);
  assert(strcmp(exportv[0].name, "a") == 0);
  assert(exportv[0].type == WASM_EXPORT_FUNCTION);
  assert(exportv[0].index == 18);
}
