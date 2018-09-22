#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {0x7F, 0x00, 0x7F, 0x01};
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  {
    wasm_global_type_t result;
    size_t end = 0;

    wasm_parser_error_t error =
        wasm_parse_global_type(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.value_type == WASM_VALUE_TYPE_I32);
    assert(result.mutablily == WASM_MUTABILITY_CONST);
    assert(end == 2);
  }

  {
    wasm_global_type_t result;
    size_t end = 0;

    wasm_parser_error_t error =
        wasm_parse_global_type(parser, 2, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.value_type == WASM_VALUE_TYPE_I32);
    assert(result.mutablily == WASM_MUTABILITY_VARIABLE);
    assert(end == 4);
  }
}
