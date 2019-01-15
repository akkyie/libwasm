#include <wasm.h>

wasm_module_instance_t* wasm_instantiate_module(wasm_stack_t* stack,
                                                wasm_store_t* store,
                                                wasm_module_t* module) {
  // TODO: externals

  wasm_module_instance_t* moduleinst = wasm_allocate_module(store, module);

  // TODO: initialize table and memory

  return moduleinst;
}
