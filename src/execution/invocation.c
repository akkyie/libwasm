#include <wasm.h>

// FIXME:
#include <stdio.h>

void wasm_invoke(wasm_store_t *store, wasm_stack_t *stack,
                 wasm_module_instance_t *moduleinst,
                 wasm_function_address_t funcaddr, wasm_value_t *valuev,
                 uint32_t valuec, wasm_value_t **resultv, uint32_t *resultc) {
  for (uint32_t i = 0; i < valuec; i++) {
    wasm_stack_push_value(stack, &valuev[i]);
  }

  wasm_function_instance_t *funcinst = store->functionv[funcaddr];
  wasm_invoke_function(store, stack, moduleinst, funcinst);

  if (funcinst->type->resultc == 0) {
    *resultc = 0;
    resultv = NULL;
    return;
  }

  *resultc = funcinst->type->resultc;
  *resultv = calloc(*resultc, sizeof(wasm_value_t));
  for (uint32_t i = 0; i < *resultc; i++) {
    wasm_value_t *value = wasm_stack_pop_value(stack);
    (*resultv)[i] = *value;
    wasm_value_free(value);
  }
}

void wasm_invoke_function(wasm_store_t *store, wasm_stack_t *stack,
                          wasm_module_instance_t *moduleinst,
                          wasm_function_instance_t *funcinst) {
  // printf("=== invoking function\n");

  uint32_t paramc = funcinst->type->paramc;
  uint32_t localc = funcinst->code->localc;
  uint32_t framelocalc = paramc + localc;
  wasm_value_t *framelocalv = calloc(framelocalc, sizeof(wasm_value_t));

  for (uint32_t i = 0; i < funcinst->type->paramc; i++) {
    wasm_value_t *value = wasm_stack_pop_value(stack);
    framelocalv[i] = *value;
    wasm_value_free(value);
  }

  for (uint32_t i = 0; i < funcinst->code->localc; i++) {
    framelocalv[paramc + i].type = funcinst->code->localv[i];
  }

  wasm_frame_t *frame = calloc(1, sizeof(wasm_frame_t));
  frame->arity = funcinst->type->resultc;
  frame->localc = framelocalc;
  frame->localv = framelocalv;
  frame->moduleinst = moduleinst;
  wasm_stack_push_frame(stack, frame);

  wasm_result_type_t result_type = (wasm_result_type_t){
      .typev = funcinst->type->resultv, .typec = funcinst->type->resultc};
  wasm_expression_t expression = funcinst->code->expression;
  wasm_block_argument_t arg =
      (wasm_block_argument_t){.type = result_type, .expression = &expression};
  wasm_instruction_t block =
      (wasm_instruction_t){.opcode = WASM_OPCODE_BLOCK, .argument.block = &arg};

  int result = wasm_execute(store, stack, &block);

  // printf("=== returning from function\n");
  // printf("result: %d\n", result);
  // printf("stack: \n");
  // wasm_print_stack(stack);

  if (result != 0) return;

  wasm_value_t **valuev = NULL;
  if (frame->arity > 0) valuev = calloc(frame->arity, sizeof(wasm_value_t *));

  for (uint32_t i = 0; i < frame->arity; i++)
    valuev[i] = wasm_stack_pop_value(stack);

  // wasm_stack_pop_frame(stack);
  frame = wasm_stack_pop_frame(stack);

  for (uint32_t i = 0; i < frame->arity; i++)
    wasm_stack_push_value(stack, valuev[i]);

  wasm_frame_free(frame);

  free(valuev);
}
