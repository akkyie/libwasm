#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  {
    uint8_t input[] = {0x03};
    wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};
    uint32_t result = 0;
    size_t end = 0;
    wasm_parser_error_t error = wasm_parse_uint(8, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == 3);
    assert(end == 1);
  }

  {
    uint8_t input[] = {0x83, 0x00};
    wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};
    uint32_t result = 0;
    size_t end = 0;
    wasm_parser_error_t error = wasm_parse_uint(8, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == 3);
    assert(end == 2);
  }
  return 0;
}
