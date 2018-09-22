#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  {
    uint8_t input[] = {
        0x0F, 0x57, 0x65, 0x62, 0xF0, 0x9F, 0x8C, 0x8F,
        0x41, 0x73, 0x73, 0x65, 0x6D, 0x62, 0x6C, 0x79,
    };
    wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

    wasm_utf8_t *result = NULL;
    size_t end = 0;

    wasm_parser_error_t error = wasm_parse_name(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(strcmp(result, u8"Web🌏Assembly") == 0);
    assert(end == 16);
  }

  {
    uint8_t input[] = {0x00, 0x57, 0x65, 0x62};
    wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

    wasm_utf8_t *result = NULL;
    size_t end = 0;

    wasm_parser_error_t error = wasm_parse_name(parser, 0, &result, &end);
    assert(error == WASM_PARSER_INVALID_NAME_LENGTH);
  }
}
