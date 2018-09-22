#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,                    // vector length
      0x12,                    // memory index
      0x0B,                    // expression end
      0x04,                    // vector length (bytes)
      0x01, 0x02, 0x03, 0x04,  // bytes
      0x34,                    // memory index
      0x0B,                    // expression end
      0x02,                    // vector length (bytes)
      0x05, 0x06,              // bytes
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t datac;
  wasm_data_t *datav;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_data_section(parser, 0, &datac, &datav, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(datac == 2);
  assert(datav[0].data == 18);
  assert(datav[0].initc == 4);
  assert(datav[0].initv[0] == 0x01);
  assert(datav[0].initv[1] == 0x02);
  assert(datav[0].initv[2] == 0x03);
  assert(datav[0].initv[3] == 0x04);
  assert(datav[1].data == 52);
  assert(datav[1].initc == 2);
  assert(datav[1].initv[0] == 0x05);
  assert(datav[1].initv[1] == 0x06);
}
