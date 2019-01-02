#include <assert.h>
#include <wasm.h>

wasm_function_address_t wasm_allocate_function(
    wasm_store_t* store, wasm_function_t* function,
    wasm_module_instance_t* moduleinst) {
  assert(function->type < moduleinst->typec);

  wasm_function_address_t addr = store->functionc;

  wasm_function_instance_t* funcinst =
      calloc(1, sizeof(wasm_function_instance_t));
  funcinst->type = moduleinst->typev + function->type;
  funcinst->module = moduleinst;
  funcinst->code = function;

  wasm_function_instance_t** functionv = realloc(store->functionv, addr);
  if (functionv == NULL) return -1;

  functionv[addr] = funcinst;
  store->functionv = functionv;
  store->functionc = addr + 1;
  return addr;
}

wasm_table_address_t wasm_allocate_table(wasm_store_t* store,
                                         wasm_table_type_t* tabletype) {
  wasm_table_address_t addr = store->tablec;

  wasm_table_instance_t* tableinst = calloc(1, sizeof(wasm_table_instance_t));

  uint32_t min = tabletype->limit.min;
  tableinst->functionc = min;
  tableinst->functionv = calloc(min, sizeof(wasm_function_address_t));

  wasm_table_instance_t** tablev = realloc(store->tablev, addr);
  if (tablev == NULL) return -1;

  tablev[addr] = tableinst;
  store->tablev = tablev;
  store->tablec = addr + 1;

  return addr;
}

wasm_memory_address_t wasm_allocate_memory(wasm_store_t* store,
                                           wasm_memory_type_t* memorytype) {
  wasm_memory_address_t addr = store->memoryc;

  wasm_memory_instance_t* memoryinst =
      calloc(1, sizeof(wasm_memory_instance_t));

  memoryinst->data = calloc(memorytype->min, 8 * 64 * 1024);
  memoryinst->has_max = memorytype->has_max;
  memoryinst->max = memorytype->max;

  wasm_memory_instance_t** memoryv = realloc(store->memoryv, addr);
  if (memoryv == NULL) return -1;

  memoryv[addr] = memoryinst;
  store->memoryv = memoryv;
  store->memoryc = addr + 1;

  return addr;
}

wasm_global_address_t wasm_allocate_global(wasm_store_t* store,
                                           wasm_global_type_t* globaltype) {
  wasm_global_address_t addr = store->memoryc;

  wasm_global_instance_t* globalinst =
      calloc(1, sizeof(wasm_global_instance_t));

  wasm_global_instance_t** globalv = realloc(store->globalv, addr);
  if (globalv == NULL) return -1;

  globalv[addr] = globalinst;
  store->globalv = globalv;
  store->globalc = addr + 1;

  return addr;
}

wasm_module_instance_t* wasm_allocate_module(wasm_store_t* store,
                                             wasm_module_t* module) {
  wasm_module_instance_t* moduleinst =
      calloc(1, sizeof(wasm_module_instance_t));

  if (!moduleinst) {
    assert(0);
  }

  moduleinst->typec = module->typec;
  moduleinst->typev = calloc(moduleinst->typec, sizeof(wasm_function_type_t));
  for (size_t i = 0; i < module->typec; i++) {
    moduleinst->typev[i] = module->typev[i];
  }

  moduleinst->functionc = module->functionc;
  moduleinst->functionv =
      calloc(module->functionc, sizeof(wasm_function_address_t));
  for (size_t i = 0; i < module->functionc; i++) {
    moduleinst->functionv[i] =
        wasm_allocate_function(store, module->functionv + i, moduleinst);
  }

  moduleinst->tablec = module->tablec;
  moduleinst->tablev = calloc(moduleinst->tablec, sizeof(wasm_table_address_t));
  for (size_t i = 0; i < module->tablec; i++) {
    moduleinst->tablev[i] = wasm_allocate_table(store, module->tablev + i);
  }

  moduleinst->memoryc = module->memoryc;
  moduleinst->memoryv =
      calloc(moduleinst->memoryc, sizeof(wasm_memory_address_t));
  for (size_t i = 0; i < module->memoryc; i++) {
    moduleinst->memoryv[i] = wasm_allocate_memory(store, module->memoryv + i);
  }

  moduleinst->globalc = module->globalc;
  moduleinst->globalv =
      calloc(moduleinst->globalc, sizeof(wasm_global_address_t));
  for (size_t i = 0; i < module->globalc; i++) {
    moduleinst->globalv[i] = wasm_allocate_global(store, module->globalv + i);
  }

  return moduleinst;
}
