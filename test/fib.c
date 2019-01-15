#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  unsigned char input[] = {
      0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x09, 0x02,
      0x60, 0x00, 0x00, 0x60, 0x01, 0x7f, 0x01, 0x7f, 0x03, 0x02, 0x01,
      0x01, 0x0a, 0x3c, 0x01, 0x3a, 0x01, 0x01, 0x7f, 0x41, 0x01, 0x21,
      0x01, 0x02, 0x40, 0x20, 0x00, 0x41, 0x02, 0x48, 0x0d, 0x00, 0x20,
      0x00, 0x41, 0x02, 0x6a, 0x21, 0x00, 0x41, 0x01, 0x21, 0x01, 0x03,
      0x40, 0x20, 0x00, 0x41, 0x7d, 0x6a, 0x10, 0x00, 0x20, 0x01, 0x6a,
      0x21, 0x01, 0x20, 0x00, 0x41, 0x7e, 0x6a, 0x22, 0x00, 0x41, 0x03,
      0x4a, 0x0d, 0x00, 0x0b, 0x0b, 0x20, 0x01, 0x0b,
  };

  // Parse

  wasm_parser_t *parser = &(wasm_parser_t){input, sizeof input};

  wasm_module_t *module;
  size_t end = 0;
  wasm_parser_error_t error = wasm_parse_module(parser, 0, &module, &end);
  assert(error == WASM_PARSER_NO_ERROR);

  //  wasm_print_module(module);

  //  // Execution

  wasm_store_t *store = wasm_store_init();
  wasm_stack_t *stack = wasm_stack_init();

  wasm_module_instance_t *moduleinst = wasm_allocate_module(store, module);
  printf("Module allocated\n");

  for (uint32_t i = 0; i < 20; i++) {
    printf("\n\n");
    wasm_value_t *value = wasm_value_init_i32(i);
    wasm_value_t *resultv = NULL;
    uint32_t resultc = 0;

    printf("Invoking 0th function with:\n");
    wasm_print_value(value);

    wasm_invoke(store, stack, moduleinst, 0, value, 1, &resultv, &resultc);

    printf("Invocation succeeded: ");

    if (resultv == NULL) {
      printf("no value returned\n");
      return -1;
    }

    printf("%d value returned\n", resultc);
    for (uint32_t i = 0; i < resultc; i++) {
      wasm_print_value(&resultv[i]);
    }
  }
}
