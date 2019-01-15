#include <wasm.h>

// FIXME:
#include <assert.h>
#include <stdio.h>

void wasm_enter_block(wasm_store_t* store, wasm_stack_t* stack,
                      wasm_label_t* label, wasm_instruction_t* instructionv,
                      size_t instructionc);

wasm_execution_result_t wasm_execute(wasm_store_t* store, wasm_stack_t* stack,
                                     wasm_instruction_t* instruction) {
  // printf("---\n");
  // printf("> executing opcode: 0x%02x\n", instruction->opcode);
  // printf("stack:\n");
  // wasm_print_stack(stack);
  // printf("---\n");

  switch (instruction->opcode) {
      // control

    case WASM_OPCODE_BLOCK: {
      wasm_label_t* label = calloc(1, sizeof(wasm_label_t));
      label->arity = instruction->argument.block->type.typec;
      wasm_expression_t* expression = instruction->argument.block->expression;
      wasm_enter_block(store, stack, label, expression->instructionv,
                       expression->instructionc);
      wasm_label_free(label);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

    case WASM_OPCODE_LOOP: {
      wasm_label_t* label = calloc(1, sizeof(wasm_label_t));
      wasm_expression_t* expression = instruction->argument.block->expression;
      label->instructionv = instruction;
      label->instructionc = 1;
      wasm_enter_block(store, stack, label, expression->instructionv,
                       expression->instructionc);

      wasm_label_free(label);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

    case WASM_OPCODE_BR: {
      uint32_t index = instruction->argument.index;
      wasm_label_t* label = wasm_stack_get_nth_label(stack, index);

      wasm_value_t** valuev = NULL;

      if (label->arity > 0)
        valuev = calloc(label->arity, sizeof(wasm_value_t*));

      for (uint32_t i = 0; i < label->arity; i++)
        valuev[i] = wasm_stack_pop_value(stack);

      for (uint32_t i = 0; i <= label->arity; i++) {
        while (stack->top->kind == WASM_STACK_ENTRY_VALUE)
          wasm_value_free(wasm_stack_pop_value(stack));

        wasm_label_t* _label = wasm_stack_pop_label(stack);
        // don't free current label
        if (label != _label) wasm_label_free(_label);
      }

      for (uint32_t i = 0; i < label->arity; i++)
        wasm_stack_push_value(stack, valuev[i]);

      free(valuev);

      return WAMS_EXECUTION_RESULT_BR;
    }

    case WASM_OPCODE_BRIF: {
      wasm_value_t* value = wasm_stack_pop_value(stack);
      assert(value->type == WASM_VALUE_TYPE_I32);

      if (value->value.i32 == 0) {
        wasm_value_free(value);
        return WAMS_EXECUTION_RESULT_NEXT;
      }

      assert(value->value.i32 == 1);
      wasm_value_free(value);

      wasm_instruction_t ins = (wasm_instruction_t){
          .opcode = WASM_OPCODE_BR, .argument = instruction->argument};
      return wasm_execute(store, stack, &ins);
    }

    case WASM_OPCODE_CALL: {
      uint32_t index = instruction->argument.index;
      wasm_frame_t* frame = wasm_stack_get_current_frame(stack);
      assert(index < frame->moduleinst->functionc);

      wasm_module_instance_t* moduleinst = frame->moduleinst;
      wasm_function_address_t funcaddr = moduleinst->functionv[index];
      wasm_function_instance_t* funcinst = store->functionv[funcaddr];
      wasm_invoke_function(store, stack, moduleinst, funcinst);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

      // local

    case WASM_OPCODE_GETLOCAL: {
      uint32_t index = instruction->argument.index;
      wasm_frame_t* frame = wasm_stack_get_current_frame(stack);
      wasm_value_t* value = wasm_value_copy(&frame->localv[index]);
      wasm_stack_push_value(stack, value);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

    case WASM_OPCODE_SETLOCAL: {
      uint32_t index = instruction->argument.index;
      wasm_frame_t* frame = wasm_stack_get_current_frame(stack);
      wasm_value_t* value = wasm_stack_pop_value(stack);
      assert(value->type == frame->localv[index].type);
      frame->localv[index].value = value->value;
      wasm_value_free(value);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

    case WASM_OPCODE_TEELOCAL: {
      wasm_value_t* value1 = wasm_stack_pop_value(stack);
      wasm_value_t* value2 = wasm_value_copy(value1);

      wasm_stack_push_value(stack, value1);
      wasm_stack_push_value(stack, value2);

      wasm_instruction_t ins = (wasm_instruction_t){
          .opcode = WASM_OPCODE_SETLOCAL, .argument = instruction->argument};
      return wasm_execute(store, stack, &ins);
    }

      // constants

    case WASM_OPCODE_I32: {
      wasm_value_t* value =
          wasm_value_init_i32(instruction->argument.const_i32);
      wasm_stack_push_value(stack, value);
      return WAMS_EXECUTION_RESULT_NEXT;
    }
    case WASM_OPCODE_I64: {
      wasm_value_t* value =
          wasm_value_init_i64(instruction->argument.const_i64);
      wasm_stack_push_value(stack, value);
      return WAMS_EXECUTION_RESULT_NEXT;
    }
    case WASM_OPCODE_F32: {
      wasm_value_t* value =
          wasm_value_init_f32(instruction->argument.const_f32);
      wasm_stack_push_value(stack, value);
      return WAMS_EXECUTION_RESULT_NEXT;
    }
    case WASM_OPCODE_F64: {
      wasm_value_t* value =
          wasm_value_init_f64(instruction->argument.const_f64);
      wasm_stack_push_value(stack, value);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

      // numeric

    case WASM_OPCODE_I32_ADD: {
      wasm_value_t* rvalue = wasm_stack_pop_value(stack);
      wasm_value_t* lvalue = wasm_stack_pop_value(stack);

      assert(rvalue->type == WASM_VALUE_TYPE_I32 &&
             lvalue->type == WASM_VALUE_TYPE_I32);

      wasm_value_t* value =
          wasm_value_init_i32(lvalue->value.i32 + rvalue->value.i32);
      wasm_stack_push_value(stack, value);

      wasm_value_free(rvalue);
      wasm_value_free(lvalue);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

    case WASM_OPCODE_I32_SUB: {
      wasm_value_t* rvalue = wasm_stack_pop_value(stack);
      wasm_value_t* lvalue = wasm_stack_pop_value(stack);

      assert(rvalue->type == WASM_VALUE_TYPE_I32 &&
             lvalue->type == WASM_VALUE_TYPE_I32);

      wasm_value_t* value =
          wasm_value_init_i32(lvalue->value.i32 - rvalue->value.i32);
      wasm_stack_push_value(stack, value);

      wasm_value_free(rvalue);
      wasm_value_free(lvalue);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

    case WASM_OPCODE_I32_LT_S: {
      wasm_value_t* rvalue = wasm_stack_pop_value(stack);
      wasm_value_t* lvalue = wasm_stack_pop_value(stack);

      assert(rvalue->type == WASM_VALUE_TYPE_I32 &&
             lvalue->type == WASM_VALUE_TYPE_I32);

      wasm_value_t* value =
          wasm_value_init_i32((lvalue->value.i32 < rvalue->value.i32) ? 1 : 0);
      wasm_stack_push_value(stack, value);

      wasm_value_free(rvalue);
      wasm_value_free(lvalue);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

    case WASM_OPCODE_I32_GT_S: {
      wasm_value_t* rvalue = wasm_stack_pop_value(stack);
      wasm_value_t* lvalue = wasm_stack_pop_value(stack);

      assert(rvalue->type == WASM_VALUE_TYPE_I32 &&
             lvalue->type == WASM_VALUE_TYPE_I32);

      wasm_value_t* value =
          wasm_value_init_i32((lvalue->value.i32 > rvalue->value.i32) ? 1 : 0);
      wasm_stack_push_value(stack, value);

      wasm_value_free(rvalue);
      wasm_value_free(lvalue);
      return WAMS_EXECUTION_RESULT_NEXT;
    }

    default: {
      printf("unimplemented");
      break;
    }
  }

  return WAMS_EXECUTION_RESULT_UNKNOWN;
}

wasm_execution_result_t wasm_execute_instructions(
    wasm_store_t* store, wasm_stack_t* stack, wasm_label_t* label,
    wasm_instruction_t* instructionv, size_t instructionc) {
  wasm_execution_result_t result = WAMS_EXECUTION_RESULT_NEXT;
  for (size_t i = 0; i < instructionc; i++) {
    result = wasm_execute(store, stack, &instructionv[i]);
    if (result != WAMS_EXECUTION_RESULT_NEXT) {
      // printf("block terminated with result: %d\n", result);
      return result;
    }
  }
  return result;
}

void wasm_enter_block(wasm_store_t* store, wasm_stack_t* stack,
                      wasm_label_t* label, wasm_instruction_t* instructionv,
                      size_t instructionc) {
  // printf("entering a block\n");
  wasm_stack_push_label(stack, label);

  wasm_execution_result_t result = wasm_execute_instructions(
      store, stack, label, instructionv, instructionc);

  switch (result) {
    case WAMS_EXECUTION_RESULT_NEXT: {
      // printf("ending a block\n");

      assert(stack->top != NULL);

      // FIXME: support multiple values
      wasm_value_t* value = NULL;
      if (stack->top->kind == WASM_STACK_ENTRY_VALUE) {
        value = wasm_stack_pop_value(stack);
      }

      assert(stack->top->kind == WASM_STACK_ENTRY_LABEL);
      wasm_stack_pop_label(stack);

      if (value != NULL) {
        wasm_stack_push_value(stack, value);
      }
      break;
    }

    case WAMS_EXECUTION_RESULT_BR: {
      if (label->instructionc > 0)
        wasm_execute_instructions(store, stack, label, label->instructionv,
                                  label->instructionc);
      break;
    }
    default:
      assert(false);
      break;
  }
}
