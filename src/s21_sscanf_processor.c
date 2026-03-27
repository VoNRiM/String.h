#include "s21_sscanf.h"

// Основной цикл обработки формата
void process_format_string(s21_sscanf_context *context) {
  int should_continue = 1;

  while (should_continue && *context->format != '\0' && context->error == 0) {
    // Сохраняем позиции для возможного отката
    const char *saved_format = context->format;
    const char *saved_str = context->str;

    if (*context->format == '%') {
      context->format++;
      process_specifier(context);
    } else {
      process_literal(context);
    }

    // Если ошибка - откатываемся и прекращаем
    if (context->error == 1) {
      context->format = saved_format;
      context->str = saved_str;
      context->error = 0;  // Сбрасываем, т.к. для sscanf это нормально
      should_continue = 0;
    }
  }
}

// Обработка спецификатора (%...)
void process_specifier(s21_sscanf_context *context) {
  const char *saved_str = context->str;
  parse_result parsed = parse_specifier(&context->format);
  if (!parsed.success) {
    context->error = 1;
  } else if (parsed.spec.specifier != 'n' && *context->str == '\0') {
    // Не %n и строка кончилась - ошибка
    context->error = 1;
  } else {
    // Пропуск пробелов если нужно
    if (parsed.spec.skip_whitespace) {
      while (is_space(*context->str)) {
        context->str++;
        context->read_count++;
      }
    }
    // Обработка спецификатора
    if (dispatch_handler(context, &parsed.spec) == 0) {
      context->str = saved_str;
      context->error = 1;
    }
  }
}

// Обработка обычного символа (не спецификатора)
void process_literal(s21_sscanf_context *context) {
  // Если в формате пробел - пропускаем все пробелы
  if (is_space(*context->format)) {
    // Пропускаем пробелы в формате
    while (is_space(*context->format)) {
      context->format++;
    }
    // Пропускаем пробелы во входной строке
    while (is_space(*context->str)) {
      context->str++;
      context->read_count++;
    }
  }
  // Если символы совпадают - продвигаемся
  else if (*context->format == *context->str) {
    context->format++;
    context->str++;
    context->read_count++;
  }
  // Иначе - ошибка (прекращаем чтение)
  else {
    context->error = 1;
  }
}
