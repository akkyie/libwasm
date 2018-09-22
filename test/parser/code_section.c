#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,  // vector length (code)
      0x04,  // code size
      0x01,  // vector length (locals)
      0x03,  // n
      0x7F,  // Int32
      0x0B,  // expression end
      0x06,  // code size
      0x02,  // vector length (locals)
      0x01,  // n
      0x7E,  // Int64
      0x02,  // n
      0x7D,  // Float32
      0x0B,  // expression end
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t codec;
  wasm_function_t *codev;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_code_section(parser, 0, &codec, &codev, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(codec == 2);
}
