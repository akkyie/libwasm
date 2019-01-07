#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

size_t _count_stack(wasm_stack_t *stack) {
  size_t count = 0;
  for (wasm_stack_entry_t *entry = stack->top; entry != NULL;
       entry = entry->next)
    count++;
  return count;
}

int main() {
  wasm_stack_t *stack = wasm_stack_init();
  assert(_count_stack(stack) == 0);

  {
    wasm_value_t *value1 = wasm_value_init_i32(1234);
    wasm_stack_push_value(stack, value1);
    assert(_count_stack(stack) == 1);

    wasm_value_t *value2 = wasm_value_init_i32(5678);
    wasm_stack_push_value(stack, value2);
    assert(_count_stack(stack) == 2);

    wasm_value_t *popped1 = wasm_stack_pop_value(stack);
    assert(popped1 == value2);
    assert(popped1->type == WASM_VALUE_TYPE_I32);
    assert(popped1->value.i32 == 5678);
    assert(_count_stack(stack) == 1);
    wasm_value_free(popped1);

    wasm_value_t *popped2 = wasm_stack_pop_value(stack);
    assert(popped2 == value1);
    assert(popped2->type == WASM_VALUE_TYPE_I32);
    assert(popped2->value.i32 == 1234);
    assert(_count_stack(stack) == 0);
    wasm_value_free(popped2);
  }

  return 0;
}
