#include <wasm.h>

wasm_parser_error_t wasm_parse_value_type(wasm_parser_t *parser, size_t start,
                                          wasm_value_type_t *result,
                                          size_t *end) {
  if (start >= parser->length)
    return WASM_PARSER_REACHED_END;

  switch (parser->input[start]) {
  case 0x7F:
    *result = WASM_VALUE_TYPE_I32;
    break;
  case 0x7E:
    *result = WASM_VALUE_TYPE_I64;
    break;
  case 0x7D:
    *result = WASM_VALUE_TYPE_F32;
    break;
  case 0x7C:
    *result = WASM_VALUE_TYPE_F64;
    break;
  default:
    return WASM_PARSER_UNEXPECTED;
  }
  *end = start + 1;

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_result_type(wasm_parser_t *parser, size_t start,
                                           wasm_result_type_t *result,
                                           size_t *end) {
  if (start >= parser->length)
    return WASM_PARSER_REACHED_END;

  switch (parser->input[start]) {
  case 0x40:
    *result = (wasm_result_type_t){NULL, 0};
    break;
  default: {
    wasm_value_type_t type;
    wasm_parser_error_t error =
        wasm_parse_value_type(parser, start, &type, end);
    if (error != WASM_PARSER_NO_ERROR)
      return error;

    wasm_result_type_t r = {&type, 1};
    *result = r;
    break;
  }
  }
  *end = start + 1;

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_function_type(wasm_parser_t *parser,
                                             size_t start,
                                             wasm_function_type_t *result,
                                             size_t *end) {
  if (start >= parser->length)
    return WASM_PARSER_REACHED_END;

  if (parser->input[start] != 0x60)
    return WASM_PARSER_UNEXPECTED;

  wasm_function_type_t _result = {NULL, 0, NULL, 0};

  wasm_parser_error_t error =
      wasm_parse_uint(32, parser, start + 1, &(_result.paramc), end);
  if (error != WASM_PARSER_NO_ERROR)
    return error;

  _result.params =
      (wasm_value_type_t *)calloc(_result.paramc, sizeof(wasm_value_type_t));

  if (_result.params == NULL) {
    return WASM_PARSER_CALLOC_FAILED;
  }

  size_t parameter_offset = *end;
  for (size_t i = 0; i < _result.paramc; i++) {
    error = wasm_parse_value_type(parser, parameter_offset,
                                  &(_result.params[i]), end);
    if (error != WASM_PARSER_NO_ERROR)
      return error;
  }

  error = wasm_parse_uint(32, parser, *end, &(_result.resultc), end);
  if (error != WASM_PARSER_NO_ERROR)
    return error;

  _result.results =
      (wasm_value_type_t *)calloc(_result.resultc, sizeof(wasm_value_type_t));

  if (_result.results == NULL) {
    return WASM_PARSER_CALLOC_FAILED;
  }

  size_t result_offset = *end;
  for (size_t i = 0; i < _result.resultc; i++) {
    error = wasm_parse_value_type(parser, result_offset, &(_result.results[i]),
                                  end);
    if (error != WASM_PARSER_NO_ERROR)
      return error;
  }

  *result = _result;

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_limit(wasm_parser_t *parser, size_t start,
                                     wasm_limit_t *result, size_t *end) {
  if (start >= parser->length)
    return WASM_PARSER_REACHED_END;

  switch (parser->input[start]) {
  case 0x00: {
    uint32_t min;
    wasm_parser_error_t error =
        wasm_parse_uint(32, parser, start + 1, &min, end);
    if (error != WASM_PARSER_NO_ERROR)
      return error;

    *result = (wasm_limit_t){.has_max = false, min, 0};
    return WASM_PARSER_NO_ERROR;
  }
  case 0x01: {
    uint32_t min, max;

    wasm_parser_error_t error =
        wasm_parse_uint(32, parser, start + 1, &min, end);
    if (error != WASM_PARSER_NO_ERROR)
      return error;

    error = wasm_parse_uint(32, parser, *end, &max, end);
    if (error != WASM_PARSER_NO_ERROR)
      return error;

    *result = (wasm_limit_t){.has_max = true, min, max};
    return WASM_PARSER_NO_ERROR;
  }
  default:
    return WASM_PARSER_UNEXPECTED;
  }
}

wasm_parser_error_t wasm_parse_memory_type(wasm_parser_t *parser, size_t start,
                                           wasm_memory_type_t *result,
                                           size_t *end) {
  return wasm_parse_limit(parser, start, result, end);
}

wasm_parser_error_t wasm_parse_table_type(wasm_parser_t *parser, size_t start,
                                          wasm_table_type_t *result,
                                          size_t *end) {
  if (start >= parser->length)
    return WASM_PARSER_REACHED_END;

  if (parser->input[start] != 0x70)
    return WASM_PARSER_UNEXPECTED;

  wasm_limit_t limit;
  wasm_parser_error_t error = wasm_parse_limit(parser, start + 1, &limit, end);
  if (error != WASM_PARSER_NO_ERROR)
    return error;

  *result = (wasm_table_type_t){limit};

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_global_type(wasm_parser_t *parser, size_t start,
                                           wasm_global_type_t *result,
                                           size_t *end) {
  if (start >= parser->length)
    return WASM_PARSER_REACHED_END;

  wasm_value_type_t value_type;
  wasm_parser_error_t error =
      wasm_parse_value_type(parser, start, &value_type, end);
  if (error != WASM_PARSER_NO_ERROR)
    return error;

  switch (parser->input[*end]) {
  case 0x00: {
    *result = (wasm_global_type_t){value_type, WASM_MUTABILITY_CONST};
    *end += 1;
    return WASM_PARSER_NO_ERROR;
  }
  case 0x01: {
    *result = (wasm_global_type_t){value_type, WASM_MUTABILITY_VARIABLE};
    *end += 1;
    return WASM_PARSER_NO_ERROR;
  }
  default:
    return WASM_PARSER_UNEXPECTED;
  }
}
