#include <wasm.h>

wasm_parser_error_t wasm_parse_magic_number(
    wasm_parser_t const parser[static 1], size_t start, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  if (!(parser->input[start] == 0x00 && parser->input[start + 1] == 0x61 &&
        parser->input[start + 2] == 0x73 && parser->input[start + 3] == 0x6D)) {
    return WASM_PARSER_UNEXPECTED;
  }

  *end = start + 4;
  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_version(wasm_parser_t const parser[static 1],
                                       size_t start, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  if (!(parser->input[start] == 0x01 && parser->input[start + 1] == 0x00 &&
        parser->input[start + 2] == 0x00 && parser->input[start + 3] == 0x00)) {
    return WASM_PARSER_UNEXPECTED;
  }

  *end = start + 4;
  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_custom_section(
    wasm_parser_t const parser[static 1], size_t start, wasm_module_t *module,
    size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  abort();  // unimplemented
}

wasm_parser_error_t wasm_parse_type_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *typec,
    wasm_function_type_t **typev, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint32_t count;
  wasm_parser_error_t error = wasm_parse_uint(32, parser, *end, &count, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  *typec = count;
  *typev = calloc(count, sizeof(wasm_function_type_t));
  for (uint32_t i = 0; i < count; i++) {
    wasm_function_type_t result;
    wasm_parse_function_type(parser, *end, &result, end);
    (*typev)[i] = result;
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_import_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *importc,
    wasm_import_t **importv, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint32_t count;
  wasm_parser_error_t error = wasm_parse_uint(32, parser, *end, &count, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  *importc = count;
  *importv = calloc(count, sizeof(wasm_import_t));
  for (uint32_t i = 0; i < count; i++) {
    wasm_utf8_t *mod;
    wasm_parse_name(parser, *end, &mod, end);
    wasm_utf8_t *name;
    wasm_parse_name(parser, *end, &name, end);
    switch (parser->input[*end]) {
      case 0x00: {
        uint32_t result;
        wasm_parse_uint(32, parser, *end + 1, &result, end);
        (*importv)[i] = (wasm_import_t){
            mod, name, WASM_IMPORT_FUNCTION, {.function = result}};
        continue;
      }
      case 0x01: {
        wasm_table_type_t result;
        wasm_parse_table_type(parser, *end + 1, &result, end);
        (*importv)[i] =
            (wasm_import_t){mod, name, WASM_IMPORT_TABLE, {.table = &result}};
        continue;
      }
      case 0x02: {
        wasm_memory_type_t result;
        wasm_parse_memory_type(parser, *end + 1, &result, end);
        (*importv)[i] =
            (wasm_import_t){mod, name, WASM_IMPORT_MEMORY, {.memory = &result}};
        continue;
      }
      case 0x03: {
        wasm_global_type_t result;
        wasm_parse_global_type(parser, *end + 1, &result, end);
        (*importv)[i] =
            (wasm_import_t){mod, name, WASM_IMPORT_GLOBAL, {.global = &result}};
        continue;
      }
      default:
        return WASM_PARSER_UNEXPECTED;
    }
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_function_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *indexc,
    uint32_t **indexv, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint32_t count;
  wasm_parse_uint(32, parser, start, &count, end);

  *indexc = count;
  *indexv = (uint32_t *)calloc(count, sizeof(uint32_t));

  for (uint32_t i = 0; i < count; i++) {
    wasm_parse_uint(32, parser, *end, &((*indexv)[i]), end);
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_table_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *tablec,
    wasm_table_type_t **tablev, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint32_t count;
  wasm_parse_uint(32, parser, start, &count, end);

  *tablec = count;
  *tablev = (wasm_table_type_t *)calloc(count, sizeof(wasm_table_type_t));

  for (uint32_t i = 0; i < count; i++) {
    wasm_parse_table_type(parser, *end, &((*tablev)[i]), end);
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_memory_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *memoryc,
    wasm_memory_type_t **memoryv, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint32_t count;
  wasm_parse_uint(32, parser, start, &count, end);

  *memoryc = count;
  *memoryv = (wasm_memory_type_t *)calloc(count, sizeof(wasm_memory_type_t));

  for (uint32_t i = 0; i < count; i++) {
    wasm_parse_memory_type(parser, *end, &((*memoryv)[i]), end);
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_global_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *globalc,
    wasm_global_type_t **globalv, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint32_t count;
  wasm_parse_uint(32, parser, start, &count, end);

  *globalc = count;
  *globalv = (wasm_global_type_t *)calloc(count, sizeof(wasm_global_type_t));

  for (uint32_t i = 0; i < count; i++) {
    wasm_parse_global_type(parser, *end, &((*globalv)[i]), end);

    wasm_expression_t expression;
    wasm_parse_expression(parser, *end, &expression, end);
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_export_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *exportc,
    wasm_export_t **exportv, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint32_t count;
  wasm_parse_uint(32, parser, start, &count, end);

  *exportc = count;
  *exportv = (wasm_export_t *)calloc(count, sizeof(wasm_export_t));

  for (uint32_t i = 0; i < count; i++) {
    wasm_utf8_t *name;
    wasm_parse_name(parser, *end, &name, end);
    (*exportv)[i].name = name;

    wasm_parse_uint(32, parser, *end, &(*exportv)[i].type, end);
    wasm_parse_uint(32, parser, *end, &(*exportv)[i].index, end);
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_start_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *index,
    size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  return wasm_parse_uint(32, parser, start, index, end);
}

wasm_parser_error_t wasm_parse_element_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *elementc,
    wasm_element_t **elementv, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  wasm_parser_error_t error;

  uint32_t count;
  error = wasm_parse_uint(32, parser, start, &count, end);
  if (error != WASM_PARSER_NO_ERROR) return error;
  *elementc = count;
  *elementv = (wasm_element_t *)calloc(count, sizeof(wasm_element_t));

  for (uint32_t i = 0; i < count; i++) {
    wasm_parse_uint(32, parser, *end, &(*elementv)[i].table, end);

    wasm_expression_t expression = {0};
    error = wasm_parse_expression(parser, *end, &expression, end);
    if (error != WASM_PARSER_NO_ERROR) return error;
    (*elementv)[i].offset = expression;

    uint32_t initc;
    error = wasm_parse_uint(32, parser, *end, &initc, end);
    if (error != WASM_PARSER_NO_ERROR) return error;
    (*elementv)[i].initc = initc;

    (*elementv)[i].initv = calloc(initc, sizeof(uint32_t));
    for (uint32_t j = 0; j < initc; j++) {
      error = wasm_parse_uint(32, parser, *end, (*elementv)[i].initv + j, end);
      if (error != WASM_PARSER_NO_ERROR) return error;
    }
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_code_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *codec,
    wasm_function_t **codev, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  wasm_parser_error_t error;

  uint32_t count;
  error = wasm_parse_uint(32, parser, start, &count, end);
  if (error != WASM_PARSER_NO_ERROR) return error;
  *codec = count;
  *codev = calloc(count, sizeof(wasm_function_t));

  for (size_t i = 0; i < count; i++) {
    uint32_t size;
    error = wasm_parse_uint(32, parser, *end, &size, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    uint32_t localc;
    error = wasm_parse_uint(32, parser, *end, &localc, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    (*codev)[i].localc = 0;
    (*codev)[i].localv = NULL;

    for (size_t j = 0; j < localc; j++) {
      uint32_t typec;
      error = wasm_parse_uint(32, parser, *end, &typec, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      wasm_value_type_t type;
      error = wasm_parse_value_type(parser, *end, &type, end);
      if (error != WASM_PARSER_NO_ERROR) return error;

      (*codev)[i].localc += typec;
      (*codev)[i].localv = realloc((*codev)[i].localv, (*codev)[i].localc);
      for (size_t k = typec; k > 0; k--) {
        (*codev)[i].localv[(*codev)[i].localc - k] = type;
      }
    }

    error = wasm_parse_expression(parser, *end, &(*codev)[i].expression, end);
    if (error != WASM_PARSER_NO_ERROR) return error;
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_bytes(uint32_t length,
                                     wasm_parser_t const parser[static 1],
                                     size_t start, uint8_t *bytev,
                                     size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  for (size_t i = 0; i < length; i++) {
    if (start + i >= parser->length) return WASM_PARSER_REACHED_END;
    bytev[i] = parser->input[start + i];
    *end = start + i + 1;
  }
  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_data_section(
    wasm_parser_t const parser[static 1], size_t start, uint32_t *datac,
    wasm_data_t **datav, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  wasm_parser_error_t error;

  uint32_t count;
  error = wasm_parse_uint(32, parser, start, &count, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  *datac = count;
  *datav = calloc(count, sizeof(wasm_data_t));
  for (size_t i = 0; i < count; i++) {
    uint32_t index;
    error = wasm_parse_uint(32, parser, *end, &index, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    wasm_expression_t expression;
    error = wasm_parse_expression(parser, *end, &expression, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    uint32_t length;
    error = wasm_parse_uint(32, parser, *end, &length, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    (*datav)[i].data = index;
    (*datav)[i].offset = expression;
    (*datav)[i].initc = length;
    (*datav)[i].initv = calloc(length, sizeof(uint8_t));
    error = wasm_parse_bytes(length, parser, *end, (*datav)[i].initv, end);
    if (error != WASM_PARSER_NO_ERROR) return error;
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_module(wasm_parser_t const parser[static 1],
                                      size_t start, wasm_module_t **module,
                                      size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  wasm_parser_error_t error = WASM_PARSER_NO_ERROR;

  error = wasm_parse_magic_number(parser, start, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  error = wasm_parse_version(parser, *end, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  *module = (wasm_module_t *)calloc(1, sizeof(wasm_module_t));

  uint32_t *funcidxv = NULL;
  uint32_t funcidxc = 0;

  for (wasm_section_t id = 0; id <= WASM_SECTION_ID_MAX; /* nop */) {
    if (*end >= parser->length) break;

    // Parse sections in order, except for custom sections
    if (parser->input[*end] != WASM_SECTION_ID_CUSTOM) {
      if (parser->input[*end] > id) {
        id++;
        continue;
      } else if (parser->input[*end] < id) {
        return WASM_PARSER_INVALID_SECTION_ID;
      }
    }
    *end += 1;

    uint32_t size;
    error = wasm_parse_uint(32, parser, *end, &size, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    switch (id) {
      case WASM_SECTION_ID_CUSTOM:
        error = wasm_parse_custom_section(parser, *end, *module, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_TYPE:
        error = wasm_parse_type_section(parser, *end, &(*module)->typec,
                                        &(*module)->typev, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_IMPORT:
        error = wasm_parse_import_section(parser, *end, &(*module)->importc,
                                          &(*module)->importv, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_FUNCTION:
        error = wasm_parse_function_section(parser, *end, &funcidxc, &funcidxv,
                                            end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_TABLE:
        error = wasm_parse_table_section(parser, *end, &(*module)->tablec,
                                         &(*module)->tablev, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_MEMORY:
        error = wasm_parse_memory_section(parser, *end, &(*module)->memoryc,
                                          &(*module)->memoryv, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_GLOBAL:
        error = wasm_parse_global_section(parser, *end, &(*module)->globalc,
                                          &(*module)->globalv, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_EXPORT:
        error = wasm_parse_export_section(parser, *end, &(*module)->exportc,
                                          &(*module)->exportv, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_START:
        error = wasm_parse_start_section(parser, *end, &(*module)->start, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_ELEMENT:
        error = wasm_parse_element_section(parser, *end, &(*module)->elementc,
                                           &(*module)->elementv, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_CODE:
        error = wasm_parse_code_section(parser, *end, &(*module)->functionc,
                                        &(*module)->functionv, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      case WASM_SECTION_ID_DATA:
        error = wasm_parse_data_section(parser, *end, &(*module)->datac,
                                        &(*module)->datav, end);
        if (error != WASM_PARSER_NO_ERROR) return error;
        break;

      default:
        return WASM_PARSER_UNEXPECTED;
    }
  }

  for (size_t i = 0; i < funcidxc; i++) {
    (*module)->functionv[i].type = funcidxv[i];
  }

  return WASM_PARSER_NO_ERROR;
}
