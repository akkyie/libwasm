#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x02,              // vector length
      0x00, 0x12,        // limits
      0x01, 0x34, 0x56,  // limits
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  uint32_t memoryc;
  wasm_memory_type_t *memoryv;
  size_t end = 0;
  wasm_parser_error_t error =
      wasm_parse_memory_section(parser, 0, &memoryc, &memoryv, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  assert(memoryc == 2);
  assert(memoryv[0].has_max == false);
  assert(memoryv[0].min == 0x12);
  assert(memoryv[1].has_max == true);
  assert(memoryv[1].min == 0x34);
  assert(memoryv[1].max == 0x56);
}
