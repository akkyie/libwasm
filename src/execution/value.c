#include <string.h>
#include <wasm.h>

wasm_value_t* wasm_value_init_i32(int32_t v) {
  wasm_value_t* value = calloc(1, sizeof(wasm_value_t));
  value->type = WASM_VALUE_TYPE_I32;
  value->value.i32 = v;
  return value;
}

wasm_value_t* wasm_value_init_i64(int64_t v) {
  wasm_value_t* value = calloc(1, sizeof(wasm_value_t));
  value->type = WASM_VALUE_TYPE_I64;
  value->value.i64 = v;
  return value;
}

wasm_value_t* wasm_value_init_f32(float v) {
  wasm_value_t* value = calloc(1, sizeof(wasm_value_t));
  value->type = WASM_VALUE_TYPE_F32;
  value->value.f32 = v;
  return value;
}

wasm_value_t* wasm_value_init_f64(double v) {
  wasm_value_t* value = calloc(1, sizeof(wasm_value_t));
  value->type = WASM_VALUE_TYPE_F64;
  value->value.f64 = v;
  return value;
}

wasm_value_t* wasm_value_copy(wasm_value_t* v) {
  wasm_value_t* value = malloc(sizeof(wasm_value_t));
  memcpy(value, v, sizeof(wasm_value_t));
  return value;
}

void wasm_value_free(wasm_value_t* value) { free(value); }
