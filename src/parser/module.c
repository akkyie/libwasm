#include <wasm.h>

wasm_parser_error wasm_parse_magic_number(wasm_parser const parser[static 1],
                                          size_t start, size_t *end) {
  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  if (!(parser->input[start] == 0x00 && parser->input[start + 1] == 0x61 &&
        parser->input[start + 2] == 0x73 && parser->input[start + 3] == 0x6D)) {
    return WASM_PARSER_UNEXPECTED;
  }

  *end = start + 4;
  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error wasm_parse_version(wasm_parser const parser[static 1],
                                     size_t start, size_t *end) {
  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  if (!(parser->input[start] == 0x01 && parser->input[start + 1] == 0x00 &&
        parser->input[start + 2] == 0x00 && parser->input[start + 3] == 0x00)) {
    return WASM_PARSER_UNEXPECTED;
  }

  *end = start + 4;
  return WASM_PARSER_NO_ERROR;
}

wasm_parser_error wasm_parse_section_header(wasm_parser const parser[static 1],
                                            size_t start,
                                            wasm_section *section_id,
                                            uint32_t *size, size_t *end) {
  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  *section_id = parser->input[start];
  if (*section_id > WASM_SECTION_ID_MAX) return WASM_PARSER_UNEXPECTED;

  *end += 1;

  return wasm_parse_uint(32, parser, *end, size, end);
}

wasm_parser_error wasm_parse_module(wasm_parser const parser[static 1],
                                    size_t start, wasm_module **module,
                                    size_t *end) {
  if (start >= parser->length) return WASM_PARSER_REACHED_END;

  wasm_parser_error error = WASM_PARSER_NO_ERROR;

  error = wasm_parse_magic_number(parser, start, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  error = wasm_parse_version(parser, *end, end);
  if (error != WASM_PARSER_NO_ERROR) return error;

  *module = (wasm_module *)calloc(1, sizeof(wasm_module));

  if (*end >= parser->length) return WASM_PARSER_NO_ERROR;

  for (wasm_section id = 0; id <= WASM_SECTION_ID_MAX; /* nop */) {
    if (parser->input[*end] == 0) {
      // parse custom section
      continue;
    } else if (parser->input[*end] > id) {
      id++;
      continue;
    } else if (parser->input[*end] < id) {
      return WASM_PARSER_INVALID_SECTION_ID;
    }
    *end += 1;

    uint32_t size;
    error = wasm_parse_uint(32, parser, *end, &size, end);
    if (error != WASM_PARSER_NO_ERROR) return error;

    switch (id) {
      case WASM_SECTION_ID_TYPE: {
        uint32_t count;
        error = wasm_parse_uint(32, parser, *end, &count, end);
        if (error != WASM_PARSER_NO_ERROR) return error;

        (*module)->typec = count;
        (*module)->typev = calloc(count, sizeof(wasm_function_type));
        for (uint32_t i = 0; i < count; i++) {
          wasm_function_type result;
          wasm_parse_function_type(parser, *end, &result, end);
          (*module)->typev[i] = result;
        }
        break;
      }
      default:
        exit(1); // unimplemented
        break;
    }

    id++;
  }

  return WASM_PARSER_NO_ERROR;
}
