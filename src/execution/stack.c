#include <wasm.h>

// FIXME:
#include <stdio.h>

wasm_stack_t* wasm_stack_init() { return calloc(1, sizeof(wasm_stack_t)); }

void wasm_stack_push(wasm_stack_t* stack, wasm_stack_entry_t* entry) {
  entry->next = stack->top;
  stack->top = entry;
}

wasm_stack_entry_t* wasm_stack_pop(wasm_stack_t* stack) {
  wasm_stack_entry_t* entry = stack->top;
  stack->top = entry->next;
  return entry;
}

void wasm_stack_push_value(wasm_stack_t* stack, wasm_value_t* value) {
  wasm_stack_entry_t* entry = calloc(1, sizeof(wasm_stack_entry_t));
  entry->kind = WASM_STACK_ENTRY_VALUE;
  entry->data.value = value;
  wasm_stack_push(stack, entry);
}

wasm_value_t* wasm_stack_pop_value(wasm_stack_t* stack) {
  wasm_value_t* value = NULL;
  wasm_stack_entry_t* entry = wasm_stack_pop(stack);
  assert(entry->kind == WASM_STACK_ENTRY_VALUE);
  value = entry->data.value;
  free(entry);
  return value;
}

void wasm_stack_push_frame(wasm_stack_t* stack, wasm_frame_t* frame) {
  wasm_stack_entry_t* entry = calloc(1, sizeof(wasm_stack_entry_t));
  entry->kind = WASM_STACK_ENTRY_FRAME;
  entry->data.frame = frame;
  wasm_stack_push(stack, entry);
}

wasm_frame_t* wasm_stack_pop_frame(wasm_stack_t* stack) {
  wasm_frame_t* frame = NULL;
  wasm_stack_entry_t* entry = wasm_stack_pop(stack);
  assert(entry->kind == WASM_STACK_ENTRY_FRAME);
  frame = entry->data.frame;
  free(entry);
  return frame;
}

void wasm_stack_push_label(wasm_stack_t* stack, wasm_label_t* label) {
  wasm_stack_entry_t* entry = calloc(1, sizeof(wasm_stack_entry_t));
  entry->kind = WASM_STACK_ENTRY_LABEL;
  entry->data.label = label;
  wasm_stack_push(stack, entry);
}

wasm_label_t* wasm_stack_pop_label(wasm_stack_t* stack) {
  wasm_label_t* label = NULL;
  wasm_stack_entry_t* entry = wasm_stack_pop(stack);
  assert(entry->kind == WASM_STACK_ENTRY_LABEL);
  label = entry->data.label;
  free(entry);
  return label;
}

wasm_frame_t* wasm_stack_get_current_frame(wasm_stack_t* stack) {
  wasm_stack_entry_t* entry = stack->top;
  for (; entry != NULL; entry = entry->next) {
    if (entry->kind == WASM_STACK_ENTRY_FRAME) return entry->data.frame;
  }
  return NULL;
}

wasm_label_t* wasm_stack_get_nth_label(wasm_stack_t* stack, uint32_t n) {
  wasm_stack_entry_t* entry = stack->top;

  int i = 0;
  for (; entry != NULL; entry = entry->next) {
    if (entry->kind == WASM_STACK_ENTRY_LABEL && ++i > n)
      return entry->data.label;
  }

  return NULL;
}

void wasm_frame_free(wasm_frame_t* frame) {
  free(frame->localv);
  free(frame);
}

void wasm_label_free(wasm_label_t* label) { free(label); }
