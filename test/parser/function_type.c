#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {0x60, 0x00, 0x00, 0x60, 0x01, 0x7E, 0x01, 0x7D};
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  {
    wasm_function_type_t result;
    size_t end = 0;

    wasm_parser_error_t error =
        wasm_parse_function_type(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.paramc == 0);
    assert(result.resultc == 0);
    assert(end == 3);
  }

  {
    wasm_function_type_t result;
    size_t end = 0;

    wasm_parser_error_t error =
        wasm_parse_function_type(parser, 3, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.paramc == 1);
    assert(result.paramv[0] == WASM_VALUE_TYPE_I64);
    assert(result.resultc == 1);
    assert(result.resultv[0] == WASM_VALUE_TYPE_F32);
    assert(end == 8);
  }
}
