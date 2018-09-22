#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {0x00, 0x01, 0x01, 0x02, 0x03};
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  {
    wasm_limit_t result;
    size_t end = 0;

    wasm_parser_error_t error = wasm_parse_limit(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.has_max == false);
    assert(result.min == 1);
    assert(end == 2);
  }

  {
    wasm_limit_t result;
    size_t end = 0;

    wasm_parser_error_t error = wasm_parse_limit(parser, 2, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.has_max == true);
    assert(result.min == 2);
    assert(result.max == 3);
    assert(end == 5);
  }
}
