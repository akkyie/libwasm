#include <wasm.h>

wasm_store_t *wasm_store_init() { return calloc(1, sizeof(wasm_store_t)); }
