#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  uint8_t input[] = {0x7F, 0x7E, 0x7D, 0x7C};
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  {
    wasm_value_type_t result;
    size_t end = 0;

    wasm_parser_error_t error = wasm_parse_value_type(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == WASM_VALUE_TYPE_I32);
    assert(end == 1);
  }

  {
    wasm_value_type_t result;
    size_t end = 0;

    wasm_parser_error_t error = wasm_parse_value_type(parser, 1, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == WASM_VALUE_TYPE_I64);
    assert(end == 2);
  }

  {
    wasm_value_type_t result;
    size_t end = 0;

    wasm_parser_error_t error = wasm_parse_value_type(parser, 2, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == WASM_VALUE_TYPE_F32);
    assert(end == 3);
  }

  {
    wasm_value_type_t result;
    size_t end = 0;

    wasm_parser_error_t error = wasm_parse_value_type(parser, 3, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == WASM_VALUE_TYPE_F64);
    assert(end == 4);
  }
}
