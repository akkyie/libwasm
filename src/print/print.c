#include <wasm.h>

#include <stdio.h>

#define INDENT(n) \
  for (uint8_t i = 0; i < n; i++) printf(" ");

void wasm_print_value_type(wasm_value_type_t type, uint8_t indent);
void wasm_print_function_type(wasm_function_type_t type, uint8_t indent);
void wasm_print_result_type(wasm_result_type_t type, uint8_t indent);
void wasm_print_instruction(wasm_instruction_t instruction, uint8_t indent);
void wasm_print_expression(wasm_expression_t expression, uint8_t indent);
void wasm_print_function(wasm_function_t function, uint8_t indent);
void wasm_print_module(wasm_module_t* module);

void wasm_print_value_type(wasm_value_type_t type, uint8_t indent) {
  INDENT(indent);

  switch (type) {
    case WASM_VALUE_TYPE_I32:
      printf("WASM_VALUE_TYPE_I32\n");
      break;
    case WASM_VALUE_TYPE_I64:
      printf("WASM_VALUE_TYPE_I64\n");
      break;
    case WASM_VALUE_TYPE_F32:
      printf("WASM_VALUE_TYPE_F32\n");
      break;
    case WASM_VALUE_TYPE_F64:
      printf("WASM_VALUE_TYPE_F64\n");
      break;
  }
}

void wasm_print_function_type(wasm_function_type_t type, uint8_t indent) {
  INDENT(indent);
  printf("%u params: \n", type.paramc);

  for (size_t i = 0; i < type.paramc; i++) {
    wasm_print_value_type(type.paramv[i], indent + 1);
  }

  INDENT(indent);
  printf("%u results: \n", type.resultc);

  for (size_t i = 0; i < type.resultc; i++) {
    wasm_print_value_type(type.resultv[i], indent + 1);
  }
}

void wasm_print_result_type(wasm_result_type_t type, uint8_t indent) {
  INDENT(indent);
  printf("%u types: \n", type.typec);

  for (size_t i = 0; i < type.typec; i++) {
    wasm_print_value_type(type.typev[i], indent + 1);
  }
}

void wasm_print_instruction(wasm_instruction_t instruction, uint8_t indent) {
  INDENT(indent);

  switch (instruction.opcode) {
    case WASM_OPCODE_UNREACHABLE:
      printf("unreachable\n");
      break;
    case WASM_OPCODE_NOP:
      printf("nop\n");
      break;
    case WASM_OPCODE_BLOCK:
      printf("block\n");
      wasm_print_result_type(instruction.argument.block->type, indent);
      wasm_print_expression(
          *(wasm_expression_t*)instruction.argument.block->expression, indent);
      break;
    case WASM_OPCODE_LOOP:
      printf("block\n");
      wasm_print_result_type(instruction.argument.block->type, indent);
      wasm_print_expression(
          *(wasm_expression_t*)instruction.argument.block->expression, indent);
      break;
    case WASM_OPCODE_IF:
      printf("if\n");
      break;
    case WASM_OPCODE_ELSE:
      printf("else\n");
      break;
    case WASM_OPCODE_END:
      printf("end\n");
      break;
    case WASM_OPCODE_BR:
      printf("br\n");
      break;
    case WASM_OPCODE_BRIF:
      printf("brif\n");
      break;
    case WASM_OPCODE_BRTABLE:
      printf("brtable\n");
      break;
    case WASM_OPCODE_RETURN:
      printf("return\n");
      break;
    case WASM_OPCODE_CALL:
      printf("call\n");
      break;
    case WASM_OPCODE_CALLINDIRECT:
      printf("callindirect\n");
      break;
    case WASM_OPCODE_DROP:
      printf("drop\n");
      break;
    case WASM_OPCODE_SELECT:
      printf("select\n");
      break;
    case WASM_OPCODE_GETLOCAL:
      printf("getlocal\n");
      break;
    case WASM_OPCODE_SETLOCAL:
      printf("setlocal\n");
      break;
    case WASM_OPCODE_TEELOCAL:
      printf("teelocal\n");
      break;
    case WASM_OPCODE_GETGLOBAL:
      printf("getglobal\n");
      break;
    case WASM_OPCODE_SETGLOBAL:
      printf("setglobal\n");
      break;
    case WASM_OPCODE_I32_LOAD:
      printf("i32.load\n");
      break;
    case WASM_OPCODE_I64_LOAD:
      printf("i64.load\n");
      break;
    case WASM_OPCODE_F32_LOAD:
      printf("f32.load\n");
      break;
    case WASM_OPCODE_F64_LOAD:
      printf("f64.load\n");
      break;
    case WASM_OPCODE_I32_LOAD_8_S:
      printf("i32.load_8_s\n");
      break;
    case WASM_OPCODE_I32_LOAD_8_U:
      printf("i32.load_8_u\n");
      break;
    case WASM_OPCODE_I32_LOAD_16_S:
      printf("i32.load_16_s\n");
      break;
    case WASM_OPCODE_I32_LOAD_16_U:
      printf("i32.load_16_u\n");
      break;
    case WASM_OPCODE_I64_LOAD_8_S:
      printf("i64.load_8_s\n");
      break;
    case WASM_OPCODE_I64_LOAD_8_U:
      printf("i64.load_8_u\n");
      break;
    case WASM_OPCODE_I64_LOAD_16_S:
      printf("i64.load_16_s\n");
      break;
    case WASM_OPCODE_I64_LOAD_16_U:
      printf("i64.load_16_u\n");
      break;
    case WASM_OPCODE_I64_LOAD_32_S:
      printf("i64.load_32_s\n");
      break;
    case WASM_OPCODE_I64_LOAD_32_U:
      printf("i64.load_32_u\n");
      break;
    case WASM_OPCODE_I32_STORE:
      printf("i32.store\n");
      break;
    case WASM_OPCODE_I64_STORE:
      printf("i64.store\n");
      break;
    case WASM_OPCODE_F32_STORE:
      printf("f32.store\n");
      break;
    case WASM_OPCODE_F64_STORE:
      printf("f64.store\n");
      break;
    case WASM_OPCODE_I32_STORE_8:
      printf("i32.store_8\n");
      break;
    case WASM_OPCODE_I32_STORE_16:
      printf("i32.store_16\n");
      break;
    case WASM_OPCODE_I64_STORE_8:
      printf("i64.store_8\n");
      break;
    case WASM_OPCODE_I64_STORE_16:
      printf("i64.store_16\n");
      break;
    case WASM_OPCODE_I64_STORE_32:
      printf("i64.store_32\n");
      break;
    case WASM_OPCODE_MEMORY_SIZE:
      printf("memory_size\n");
      break;
    case WASM_OPCODE_MEMORY_GROW:
      printf("memory_grow\n");
      break;
    case WASM_OPCODE_I32:
      printf("i32.const %d\n", instruction.argument.const_i32);
      break;
    case WASM_OPCODE_I64:
      printf("i32.const %lld\n", instruction.argument.const_i64);
      break;
    case WASM_OPCODE_F32:
      printf("i32.const %f\n", instruction.argument.const_f32);
      break;
    case WASM_OPCODE_F64:
      printf("i32.const %f\n", instruction.argument.const_f64);
      break;
    case WASM_OPCODE_I32_EQZ:
      printf("i32.eqz\n");
      break;
    case WASM_OPCODE_I32_EQ:
      printf("i32.eq\n");
      break;
    case WASM_OPCODE_I32_NE:
      printf("i32.ne\n");
      break;
    case WASM_OPCODE_I32_LT_S:
      printf("i32.lt_s\n");
      break;
    case WASM_OPCODE_I32_LT_U:
      printf("i32.lt_u\n");
      break;
    case WASM_OPCODE_I32_GT_S:
      printf("i32.gt_s\n");
      break;
    case WASM_OPCODE_I32_GT_U:
      printf("i32.gt_u\n");
      break;
    case WASM_OPCODE_I32_LE_S:
      printf("i32.le_s\n");
      break;
    case WASM_OPCODE_I32_LE_U:
      printf("i32.le_u\n");
      break;
    case WASM_OPCODE_I32_GE_S:
      printf("i32.ge_s\n");
      break;
    case WASM_OPCODE_I32_GE_U:
      printf("i32.ge_u\n");
      break;
    case WASM_OPCODE_I64_EQZ:
      printf("i64.eqz\n");
      break;
    case WASM_OPCODE_I64_EQ:
      printf("i64.eq\n");
      break;
    case WASM_OPCODE_I64_NE:
      printf("i64.ne\n");
      break;
    case WASM_OPCODE_I64_LT_S:
      printf("i64.lt_s\n");
      break;
    case WASM_OPCODE_I64_LT_U:
      printf("i64.lt_u\n");
      break;
    case WASM_OPCODE_I64_GT_S:
      printf("i64.gt_s\n");
      break;
    case WASM_OPCODE_I64_GT_U:
      printf("i64.gt_u\n");
      break;
    case WASM_OPCODE_I64_LE_S:
      printf("i64.le_s\n");
      break;
    case WASM_OPCODE_I64_LE_U:
      printf("i64.le_u\n");
      break;
    case WASM_OPCODE_I64_GE_S:
      printf("i64.ge_s\n");
      break;
    case WASM_OPCODE_I64_GE_U:
      printf("i64.ge_u\n");
      break;
    case WASM_OPCODE_F32_EQ:
      printf("f32.eq\n");
      break;
    case WASM_OPCODE_F32_NE:
      printf("f32.ne\n");
      break;
    case WASM_OPCODE_F32_LT:
      printf("f32.lt\n");
      break;
    case WASM_OPCODE_F32_GT:
      printf("f32.gt\n");
      break;
    case WASM_OPCODE_F32_LE:
      printf("f32.le\n");
      break;
    case WASM_OPCODE_F32_GE:
      printf("f32.ge\n");
      break;
    case WASM_OPCODE_F64_EQ:
      printf("f64.eq\n");
      break;
    case WASM_OPCODE_F64_NE:
      printf("f64.ne\n");
      break;
    case WASM_OPCODE_F64_LT:
      printf("f64.lt\n");
      break;
    case WASM_OPCODE_F64_GT:
      printf("f64.gt\n");
      break;
    case WASM_OPCODE_F64_LE:
      printf("f64.le\n");
      break;
    case WASM_OPCODE_F64_GE:
      printf("f64.ge\n");
      break;
    case WASM_OPCODE_I32_CLZ:
      printf("i32.clz\n");
      break;
    case WASM_OPCODE_I32_CTZ:
      printf("i32.ctz\n");
      break;
    case WASM_OPCODE_I32_POPCNT:
      printf("i32.popcnt\n");
      break;
    case WASM_OPCODE_I32_ADD:
      printf("i32.add\n");
      break;
    case WASM_OPCODE_I32_SUB:
      printf("i32.sub\n");
      break;
    case WASM_OPCODE_I32_MUL:
      printf("i32.mul\n");
      break;
    case WASM_OPCODE_I32_DIV_S:
      printf("i32.div_s\n");
      break;
    case WASM_OPCODE_I32_DIV_U:
      printf("i32.div_u\n");
      break;
    case WASM_OPCODE_I32_REM_S:
      printf("i32.rem_s\n");
      break;
    case WASM_OPCODE_I32_REM_U:
      printf("i32.rem_u\n");
      break;
    case WASM_OPCODE_I32_AND:
      printf("i32.and\n");
      break;
    case WASM_OPCODE_I32_OR:
      printf("i32.or\n");
      break;
    case WASM_OPCODE_I32_XOR:
      printf("i32.xor\n");
      break;
    case WASM_OPCODE_I32_SHL:
      printf("i32.shl\n");
      break;
    case WASM_OPCODE_I32_SHR_S:
      printf("i32.shr_s\n");
      break;
    case WASM_OPCODE_I32_SHR_U:
      printf("i32.shr_u\n");
      break;
    case WASM_OPCODE_I32_ROTL:
      printf("i32.rotl\n");
      break;
    case WASM_OPCODE_I32_ROTR:
      printf("i32.rotr\n");
      break;
    case WASM_OPCODE_I64_CLZ:
      printf("i64.clz\n");
      break;
    case WASM_OPCODE_I64_CTZ:
      printf("i64.ctz\n");
      break;
    case WASM_OPCODE_I64_POPCNT:
      printf("i64.popcnt\n");
      break;
    case WASM_OPCODE_I64_ADD:
      printf("i64.add\n");
      break;
    case WASM_OPCODE_I64_SUB:
      printf("i64.sub\n");
      break;
    case WASM_OPCODE_I64_MUL:
      printf("i64.mul\n");
      break;
    case WASM_OPCODE_I64_DIV_S:
      printf("i64.div_s\n");
      break;
    case WASM_OPCODE_I64_DIV_U:
      printf("i64.div_u\n");
      break;
    case WASM_OPCODE_I64_REM_S:
      printf("i64.rem_s\n");
      break;
    case WASM_OPCODE_I64_REM_U:
      printf("i64.rem_u\n");
      break;
    case WASM_OPCODE_I64_AND:
      printf("i64.and\n");
      break;
    case WASM_OPCODE_I64_OR:
      printf("i64.or\n");
      break;
    case WASM_OPCODE_I64_XOR:
      printf("i64.xor\n");
      break;
    case WASM_OPCODE_I64_SHL:
      printf("i64.shl\n");
      break;
    case WASM_OPCODE_I64_SHR_S:
      printf("i64.shr_s\n");
      break;
    case WASM_OPCODE_I64_SHR_U:
      printf("i64.shr_u\n");
      break;
    case WASM_OPCODE_I64_ROTL:
      printf("i64.rotl\n");
      break;
    case WASM_OPCODE_I64_ROTR:
      printf("i64.rotr\n");
      break;
    case WASM_OPCODE_F32_ABS:
      printf("f32.abs\n");
      break;
    case WASM_OPCODE_F32_NEG:
      printf("f32.neg\n");
      break;
    case WASM_OPCODE_F32_CEIL:
      printf("f32.ceil\n");
      break;
    case WASM_OPCODE_F32_FLOOR:
      printf("f32.floor\n");
      break;
    case WASM_OPCODE_F32_TRUNC:
      printf("f32.trunc\n");
      break;
    case WASM_OPCODE_F32_NEAREST:
      printf("f32.nearest\n");
      break;
    case WASM_OPCODE_F32_SQRT:
      printf("f32.sqrt\n");
      break;
    case WASM_OPCODE_F32_ADD:
      printf("f32.add\n");
      break;
    case WASM_OPCODE_F32_SUB:
      printf("f32.sub\n");
      break;
    case WASM_OPCODE_F32_MUL:
      printf("f32.mul\n");
      break;
    case WASM_OPCODE_F32_DIV:
      printf("f32.div\n");
      break;
    case WASM_OPCODE_F32_FMIN:
      printf("f32.fmin\n");
      break;
    case WASM_OPCODE_F32_FMAX:
      printf("f32.fmax\n");
      break;
    case WASM_OPCODE_F32_COPYSIGN:
      printf("f32.copysign\n");
      break;
    case WASM_OPCODE_F64_ABS:
      printf("f64.abs\n");
      break;
    case WASM_OPCODE_F64_NEG:
      printf("f64.neg\n");
      break;
    case WASM_OPCODE_F64_CEIL:
      printf("f64.ceil\n");
      break;
    case WASM_OPCODE_F64_FLOOR:
      printf("f64.floor\n");
      break;
    case WASM_OPCODE_F64_TRUNC:
      printf("f64.trunc\n");
      break;
    case WASM_OPCODE_F64_NEAREST:
      printf("f64.nearest\n");
      break;
    case WASM_OPCODE_F64_SQRT:
      printf("f64.sqrt\n");
      break;
    case WASM_OPCODE_F64_ADD:
      printf("f64.add\n");
      break;
    case WASM_OPCODE_F64_SUB:
      printf("f64.sub\n");
      break;
    case WASM_OPCODE_F64_MUL:
      printf("f64.mul\n");
      break;
    case WASM_OPCODE_F64_DIV:
      printf("f64.div\n");
      break;
    case WASM_OPCODE_F64_FMIN:
      printf("f64.fmin\n");
      break;
    case WASM_OPCODE_F64_FMAX:
      printf("f64.fmax\n");
      break;
    case WASM_OPCODE_F64_COPYSIGN:
      printf("f64.copysign\n");
      break;
    case WASM_OPCODE_I32_WRAP_I64:
      printf("i32.wrap_i64\n");
      break;
    case WASM_OPCODE_I32_TRUNC_S_F32:
      printf("i32.trunc_s_f32\n");
      break;
    case WASM_OPCODE_I32_TRUNC_U_F32:
      printf("i32.trunc_u_f32\n");
      break;
    case WASM_OPCODE_I32_TRUNC_S_F64:
      printf("i32.trunc_s_f64\n");
      break;
    case WASM_OPCODE_I32_TRUNC_U_F64:
      printf("i32.trunc_u_f64\n");
      break;
    case WASM_OPCODE_I64_EXTEND_S_I32:
      printf("i64.extend_s_i32\n");
      break;
    case WASM_OPCODE_I64_EXTEND_U_I32:
      printf("i64.extend_u_i32\n");
      break;
    case WASM_OPCODE_I64_TRUNC_S_F32:
      printf("i64.trunc_s_f32\n");
      break;
    case WASM_OPCODE_I64_TRUNC_U_F32:
      printf("i64.trunc_u_f32\n");
      break;
    case WASM_OPCODE_I64_TRUNC_S_F64:
      printf("i64.trunc_s_f64\n");
      break;
    case WASM_OPCODE_I64_TRUNC_U_F64:
      printf("i64.trunc_u_f64\n");
      break;
    case WASM_OPCODE_F32_CONVERT_S_I32:
      printf("f32.convert_s_i32\n");
      break;
    case WASM_OPCODE_F32_CONVERT_U_I32:
      printf("f32.convert_u_i32\n");
      break;
    case WASM_OPCODE_F32_CONVERT_S_I64:
      printf("f32.convert_s_i64\n");
      break;
    case WASM_OPCODE_F32_CONVERT_U_I64:
      printf("f32.convert_u_i64\n");
      break;
    case WASM_OPCODE_F32_DEMOTE_F64:
      printf("f32.demote_f64\n");
      break;
    case WASM_OPCODE_F64_CONVERT_S_I32:
      printf("f64.convert_s_i32\n");
      break;
    case WASM_OPCODE_F64_CONVERT_U_I32:
      printf("f64.convert_u_i32\n");
      break;
    case WASM_OPCODE_F64_CONVERT_S_I64:
      printf("f64.convert_s_i64\n");
      break;
    case WASM_OPCODE_F64_CONVERT_U_I64:
      printf("f64.convert_u_i64\n");
      break;
    case WASM_OPCODE_F64_PROMOTE_F32:
      printf("f64.promote_f32\n");
      break;
    case WASM_OPCODE_I32_REINTERPRET_F32:
      printf("i32.reinterpret_f32\n");
      break;
    case WASM_OPCODE_I64_REINTERPRET_F64:
      printf("i64.reinterpret_f64\n");
      break;
    case WASM_OPCODE_F32_REINTERPRET_I32:
      printf("f32.reinterpret_i32\n");
      break;
    case WASM_OPCODE_F64_REINTERPRET_I64:
      printf("f64.reinterpret_i64\n");
      break;
    default:
      break;
  }
}

void wasm_print_expression(wasm_expression_t expression, uint8_t indent) {
  INDENT(indent);
  printf("%zu instructions: \n", expression.instructionc);

  for (size_t i = 0; i < expression.instructionc; i++) {
    wasm_print_instruction(expression.instructionv[i], indent + 1);
  }
}

void wasm_print_function(wasm_function_t function, uint8_t indent) {
  INDENT(indent);
  printf("type: %d\n", function.type);

  INDENT(indent);
  printf("%d locals: \n", function.localc);

  for (size_t i = 0; i < function.localc; i++) {
    wasm_print_value_type(function.localv[i], indent + 1);
  }

  wasm_print_expression(function.expression, indent);
}

void wasm_print_module(wasm_module_t* module) {
  printf("%u types: \n", module->typec);
  for (size_t i = 0; i < module->typec; i++) {
    wasm_print_function_type(module->typev[i], 1);
  }

  printf("%u functions:\n", module->functionc);
  for (size_t i = 0; i < module->functionc; i++) {
    wasm_print_function(module->functionv[i], 1);
  }

  // FIXME: Implementation not completed

  printf("%u tables\n", module->tablec);
  printf("%u memories\n", module->memoryc);
  printf("%u globals\n", module->globalc);
  printf("%u elements\n", module->elementc);
  printf("%u data\n", module->datac);
  printf("%u imports\n", module->importc);
  printf("%u exports\n", module->exportc);
}

void wasm_print_value(wasm_value_t* value) {
  printf(" value: ");

  switch (value->type) {
    case WASM_VALUE_TYPE_I32:
      printf("(i32) %d\n", value->value.i32);
      break;
    case WASM_VALUE_TYPE_I64:
      printf("(i64) %lld\n", value->value.i64);
      break;
    case WASM_VALUE_TYPE_F32:
      printf("(f32) %f\n", value->value.f32);
      break;
    case WASM_VALUE_TYPE_F64:
      printf("(f64) %lf\n", value->value.f64);
      break;
    default:
      break;
  }
}

void wasm_print_stack(wasm_stack_t* stack) {
  for (wasm_stack_entry_t* entry = stack->top; entry != NULL;
       entry = entry->next) {
    switch (entry->kind) {
      case WASM_STACK_ENTRY_VALUE: {
        wasm_print_value(entry->data.value);
      } break;
      case WASM_STACK_ENTRY_FRAME: {
        printf(" frame [%u]\n", entry->data.frame->arity);
        for (size_t i = 0; i < entry->data.frame->localc; i++) {
          printf(" ");
          wasm_print_value(&entry->data.frame->localv[i]);
        }
      } break;

      case WASM_STACK_ENTRY_LABEL: {
        printf(" label [%u]\n", entry->data.label->arity);
        printf("   %zu instructions\n", entry->data.label->instructionc);
      } break;
      default:
        break;
    }
  }
}
