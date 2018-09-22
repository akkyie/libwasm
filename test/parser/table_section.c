#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,              // vector length
      0x70,              // element type
      0x00, 0x12,        // limits
      0x70,              // element type
      0x01, 0x34, 0x56,  // limits
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t tablec;
  wasm_table_type_t *tablev;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_table_section(parser, 0, &tablec, &tablev, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(tablec == 2);
  assert(tablev[0].limit.has_max == false);
  assert(tablev[0].limit.min == 0x12);
  assert(tablev[1].limit.has_max == true);
  assert(tablev[1].limit.min == 0x34);
  assert(tablev[1].limit.max == 0x56);
}
