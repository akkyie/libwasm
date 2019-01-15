#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  wasm_store_t* store = wasm_store_init();
  wasm_stack_t* stack = wasm_stack_init();

  // wasm_invoke(store, stack, 0, NULL, 0);

  return 0;
}
