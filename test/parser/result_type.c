#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  uint8_t input[] = {0x40, 0x7F};
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  {
    wasm_result_type_t result;
    size_t end = 0;

    wasm_parser_error_t error =
        wasm_parse_result_type(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.typec == 0);
    assert(end == 1);
  }

  {
    wasm_result_type_t result;
    size_t end = 0;

    wasm_parser_error_t error =
        wasm_parse_result_type(parser, 1, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.typec == 1);
    assert(result.typev[0] == WASM_VALUE_TYPE_I32);
    assert(end == 2);
  }
}
