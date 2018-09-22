#include <wasm.h>

#define P2(N) (2LL << (N - 1))

wasm_parser_error_t wasm_parse_uint(uint8_t bit,
                                    wasm_parser_t const parser[static 1],
                                    size_t start, uint32_t *result,
                                    size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  uint32_t n = (uint32_t)parser->input[start];

  if (n < P2(7) && n < P2(bit)) {
    *result = n;
    *end = start + 1;
  } else if (n >= P2(7) && bit > 7) {
    uint32_t m;
    wasm_parser_error_t error =
        wasm_parse_uint(bit - 1, parser, start + 1, &m, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    *result = (uint32_t)((m << 7) + (n - P2(7)));
  } else {
    return WASM_PARSER_INVALID_BIT;
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_sint(uint8_t bit,
                                    wasm_parser_t const parser[static 1],
                                    size_t start, int32_t *result,
                                    size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  int32_t n = (int32_t)parser->input[start];

  if (n < P2(6) && n < P2(bit - 1)) {
    *result = n;
    *end = start + 1;
  } else if (n >= P2(6) && n < P2(7) && n >= P2(7) - P2(bit - 1)) {
    *result = n - P2(7);
    *end = start + 1;
  } else if (n >= P2(7) && bit > 7) {
    int32_t m;
    wasm_parser_error_t error =
        wasm_parse_sint(bit - 1, parser, start + 1, &m, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    *result = (int32_t)((m << 7) + (n - P2(7)));
  } else {
    return WASM_PARSER_INVALID_BIT;
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_int(uint8_t bit,
                                   wasm_parser_t const parser[static 1],
                                   size_t start, int32_t *result, size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  int32_t r;
  wasm_parser_error_t error = wasm_parse_sint(bit, parser, start, &r, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  if (r < P2(bit - 1)) {
    *result = r;
  } else {
    *result = r - (int32_t)P2(bit);
  }

  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error_t wasm_parse_name(wasm_parser_t const parser[static 1],
                                    size_t start, wasm_utf8_t **result,
                                    size_t *end) {
  __wasm_parse_checkpoint(start, __func__);

  uint32_t length = 0;
  wasm_parser_error_t error = wasm_parse_uint(32, parser, start, &length, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  if (length < 1) return WASM_PARSER_INVALID_NAME_LENGTH;

  *result = (wasm_utf8_t *)calloc(length + 1, sizeof(wasm_utf8_t));
  if (*result == NULL) return WASM_PARSER_ALLOC_FAILED;

  for (size_t i = 0; i < length; i++) {
    if (start + 1 + i >= parser->length) return WASM_PARSER_REACHED_END;

    (*result)[i] = parser->input[start + 1 + i];
    *end += 1;
  }

  return WASM_PARSER_NO_ERROR;
}
