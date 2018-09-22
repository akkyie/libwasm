#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  uint8_t input[] = {
      0x00, 0x61, 0x73, 0x6D,  // magic number
      0x01, 0x00, 0x00, 0x00,  // wasm version
  };
  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  wasm_module_t *module;
  size_t end = 0;
  wasm_parser_error_t error = wasm_parse_module(parser, 0, &module, &end);
  assert(error == WASM_PARSER_NO_ERROR);
  assert(module->typec == 0);
  assert(module->functionc == 0);
  assert(module->tablec == 0);
  assert(module->memoryc == 0);
  assert(module->globalc == 0);
  assert(module->elementc == 0);
  assert(module->datac == 0);
  assert(module->start == 0);
  assert(module->importc == 0);
  assert(module->exportc == 0);
}
