#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  {
    uint8_t input[] = {0x7E};
    wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};
    int32_t result = 0;
    size_t end = 0;
    wasm_parser_error_t error = wasm_parse_sint(32, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == -2);
  }

  {
    uint8_t input[] = {0xFE, 0x7F};
    wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

    int32_t result = 0;
    size_t end = 0;
    wasm_parser_error_t error = wasm_parse_sint(32, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == -2);
  }

  {
    uint8_t input[] = {0xFE, 0xFF, 0x7F};
    wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

    int32_t result = 0;
    size_t end = 0;
    wasm_parser_error_t error = wasm_parse_sint(32, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == -2);
  }
}
