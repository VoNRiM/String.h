#include "s21_sscanf.h"

// Инициализация структуры по умолчанию
static void init_specifier_defaults(s21_sscanf_spec *spec) {
  spec->width = 0;
  spec->suppress = 0;
  spec->specifier = '\0';
  spec->skip_whitespace = 1;
  spec->length = '\0';
}

// Парсинг подавления присваивания (*)
static const char *parse_suppression(const char *pos, int *suppress) {
  if (*pos == '*') {
    *suppress = 1;
    pos++;
  }
  return pos;
}

// Парсинг ширины
static const char *parse_width(const char *pos, int *width) {
  if (is_digit(*pos)) {
    int w = 0;
    while (is_digit(*pos)) {
      w = w * 10 + (*pos - '0');
      pos++;
    }
    *width = w;
  }
  return pos;
}

// Парсинг модификатора длины
static const char *parse_length_modifier(const char *pos, char *length) {
  if (*pos == 'h' || *pos == 'l' || *pos == 'L') {
    *length = *pos;
    pos++;
  }
  return pos;
}

// Парсинг спецификатора с особыми свойствами
static const char *parse_specifier_with_flags(const char *pos, char *specifier,
                                              int *skip_whitespace) {
  char c = *pos;

  // Спецификаторы, которые НЕ пропускают пробелы
  if (c == 'c' || c == 'n') {
    *specifier = c;
    *skip_whitespace = 0;
    pos++;
  }
  // Обычные спецификаторы
  else if (c == 'd' || c == '%' || c == 's' || c == 'i' || c == 'e' ||
           c == 'E' || c == 'f' || c == 'g' || c == 'G' || c == 'u' ||
           c == 'o' || c == 'p' || c == 'x' || c == 'X') {
    *specifier = c;
    pos++;
  }

  return pos;
}

// Парсинг спецификатора (%[*][width][modifier]specifier)
parse_result parse_specifier(const char **format) {
  parse_result result = {0};
  const char *pos = *format;

  result.success = 1;
  init_specifier_defaults(&result.spec);

  // Парсинг подавления присваивания
  pos = parse_suppression(pos, &result.spec.suppress);

  // Парсинг ширины
  pos = parse_width(pos, &result.spec.width);

  // Парсинг модификатора длины
  pos = parse_length_modifier(pos, &result.spec.length);

  // Парсинг спецификатора
  const char *before_specifier = pos;
  pos = parse_specifier_with_flags(pos, &result.spec.specifier,
                                   &result.spec.skip_whitespace);

  // Проверка успешности парсинга спецификатора
  if (pos == before_specifier) {
    result.success = 0;
  }

  if (result.success) {
    *format = pos;
  }

  return result;
}