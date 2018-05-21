#include <wasm.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void test_parser_uint() {
  printf("Running %s\n", __func__);

  {
    uint8_t input[] = {0x03};
    wasm_parser *parser = &(wasm_parser){input, sizeof input};
    uint32_t result = 0;
    size_t end = 0;
    wasm_parser_error error = wasm_parse_uint(8, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == 3);
    assert(end == 1);
  }

  {
    uint8_t input[] = {0x83, 0x00};
    wasm_parser *parser = &(wasm_parser){input, sizeof input};
    uint32_t result = 0;
    size_t end = 0;
    wasm_parser_error error = wasm_parse_uint(8, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == 3);
    assert(end == 2);
  }
}

void test_parser_sint() {
  printf("Running %s\n", __func__);

  {
    uint8_t input[] = {0x7E};
    wasm_parser *parser = &(wasm_parser){input, sizeof input};
    int32_t result = 0;
    size_t end = 0;
    wasm_parser_error error = wasm_parse_sint(32, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == -2);
  }

  {
    uint8_t input[] = {0xFE, 0x7F};
    wasm_parser *parser = &(wasm_parser){input, sizeof input};

    int32_t result = 0;
    size_t end = 0;
    wasm_parser_error error = wasm_parse_sint(32, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == -2);
  }

  {
    uint8_t input[] = {0xFE, 0xFF, 0x7F};
    wasm_parser *parser = &(wasm_parser){input, sizeof input};

    int32_t result = 0;
    size_t end = 0;
    wasm_parser_error error = wasm_parse_sint(32, parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == -2);
  }
}

void test_parser_name() {
  printf("Running %s\n", __func__);

  {
    uint8_t input[] = {
        0x0F, 0x57, 0x65, 0x62, 0xF0, 0x9F, 0x8C, 0x8F,
        0x41, 0x73, 0x73, 0x65, 0x6D, 0x62, 0x6C, 0x79,
    };
    wasm_parser *parser = &(wasm_parser){input, sizeof input};

    wasm_utf8 *result = NULL;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_name(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(strcmp(result, u8"Web🌏Assembly") == 0);
    assert(end == 16);
  }

  {
    uint8_t input[] = {0x00, 0x57, 0x65, 0x62};
    wasm_parser *parser = &(wasm_parser){input, sizeof input};

    wasm_utf8 *result = NULL;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_name(parser, 0, &result, &end);
    assert(error == WASM_PARSER_INVALID_NAME_LENGTH);
  }
}

void test_parser_value_type() {
  printf("Running %s\n", __func__);

  uint8_t input[] = {0x7F, 0x7E, 0x7D, 0x7C};
  wasm_parser *parser = &(wasm_parser){input, sizeof input};

  {
    wasm_value_type result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_value_type(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == WASM_VALUE_TYPE_I32);
    assert(end == 1);
  }

  {
    wasm_value_type result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_value_type(parser, 1, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == WASM_VALUE_TYPE_I64);
    assert(end == 2);
  }

  {
    wasm_value_type result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_value_type(parser, 2, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == WASM_VALUE_TYPE_F32);
    assert(end == 3);
  }

  {
    wasm_value_type result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_value_type(parser, 3, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result == WASM_VALUE_TYPE_F64);
    assert(end == 4);
  }
}

void test_parser_result_type() {
  printf("Running %s\n", __func__);

  uint8_t input[] = {0x40, 0x7F};
  wasm_parser *parser = &(wasm_parser){input, sizeof input};

  {
    wasm_result_type result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_result_type(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.typec == 0);
    assert(end == 1);
  }

  {
    wasm_result_type result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_result_type(parser, 1, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.typec == 1);
    assert(result.typev[0] == WASM_VALUE_TYPE_I32);
    assert(end == 2);
  }
}

void test_parser_function_type() {
  printf("Running %s\n", __func__);

  uint8_t input[] = {0x60, 0x00, 0x00, 0x60, 0x01, 0x7E, 0x01, 0x7D};
  wasm_parser *parser = &(wasm_parser){input, sizeof input};

  {
    wasm_function_type result;
    size_t end = 0;

    wasm_parser_error error =
        wasm_parse_function_type(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.paramc == 0);
    assert(result.resultc == 0);
    assert(end == 3);
  }

  {
    wasm_function_type result;
    size_t end = 0;

    wasm_parser_error error =
        wasm_parse_function_type(parser, 3, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.paramc == 1);
    assert(result.paramv[0] == WASM_VALUE_TYPE_I64);
    assert(result.resultc == 1);
    assert(result.resultv[0] == WASM_VALUE_TYPE_F32);
    assert(end == 8);
  }
}

void test_parser_limit() {
  printf("Running %s\n", __func__);

  uint8_t input[] = {0x00, 0x01, 0x01, 0x02, 0x03};
  wasm_parser *parser = &(wasm_parser){input, sizeof input};

  {
    wasm_limit result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_limit(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.has_max == false);
    assert(result.min == 1);
    assert(end == 2);
  }

  {
    wasm_limit result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_limit(parser, 2, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.has_max == true);
    assert(result.min == 2);
    assert(result.max == 3);
    assert(end == 5);
  }
}

void test_parser_global_type() {
  printf("Running %s\n", __func__);

  uint8_t input[] = {0x7F, 0x00, 0x7F, 0x01};
  wasm_parser *parser = &(wasm_parser){input, sizeof input};

  {
    wasm_global_type result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_global_type(parser, 0, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.value_type == WASM_VALUE_TYPE_I32);
    assert(result.mutablily == WASM_MUTABILITY_CONST);
    assert(end == 2);
  }

  {
    wasm_global_type result;
    size_t end = 0;

    wasm_parser_error error = wasm_parse_global_type(parser, 2, &result, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(result.value_type == WASM_VALUE_TYPE_I32);
    assert(result.mutablily == WASM_MUTABILITY_VARIABLE);
    assert(end == 4);
  }
}

void test_parser_type_section() {
  {
    uint8_t input[] = {0x00, 0x61, 0x73, 0x6D, 0x01, 0x00, 0x00, 0x00};
    wasm_parser *parser = &(wasm_parser){input, sizeof input};

    wasm_module *module;
    size_t end = 0;
    wasm_parser_error error = wasm_parse_module(parser, 0, &module, &end);
    assert(error == WASM_PARSER_NO_ERROR);
    assert(module->typec == 0);
    assert(module->functionc == 0);
    assert(module->tablec == 0);
    assert(module->memoryc == 0);
    assert(module->globalc == 0);
    assert(module->exportc == 0);
  }
}

int main() {
  test_parser_uint();
  test_parser_sint();
  test_parser_name();
  test_parser_value_type();
  test_parser_result_type();
  test_parser_function_type();
  test_parser_limit();
  test_parser_global_type();
  test_parser_type_section();
}
