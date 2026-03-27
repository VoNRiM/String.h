/*
УРОВЕНЬ 0: ИНТЕРФЕЙС (s21_sscanf.c)
└── s21_sscanf()              - публичная функция

УРОВЕНЬ 1: УПРАВЛЕНИЕ ПОТОКОМ (processor.c)
├── process_format_string()   - главный цикл обработки формата
├── process_specifier()       - обработка спецификаторов (%...)
└── process_literal()         - обработка обычных символов

УРОВЕНЬ 2: ПАРСИНГ (parser.c)
└── parse_specifier()         - разбор %[*][width][modifier]specifier

УРОВЕНЬ 3: ДИСПЕТЧЕРИЗАЦИЯ (dispatcher.c)
├── dispatch_handler()        - выбор обработчика по спецификатору
├── handle_int_decimal()      - %d
├── handle_int_multi()        - %i
├── handle_unsigned_decimal() - %u
├── handle_unsigned_octal()   - %o
├── handle_unsigned_hex()     - %x/%X
├── handle_char()             - %c
├── handle_string()           - %s
├── handle_floats()           - %f/%e/%E/%g/%G
├── handle_percent()          - %%
├── handle_ptr()              - %p
├── handle_count()            - %n
└── process_value_with_modifier() - обработка модификаторов длины

УРОВЕНЬ 4: ЧТЕНИЕ ДАННЫХ (readers.c)
├── read_number_with_base()   - чтение числа с произвольным основанием
├── detect_base()             - определение системы счисления
├── read_char()               - чтение символа(ов)
├── read_signed_float()       - чтение чисел с плавающей точкой
├── read_ptr()                - чтение указателя
└── Вспомогательные функции (статические)

УРОВЕНЬ 5: УТИЛИТЫ (utils.c)
├── skip_whitespace()         - пропуск пробельных символов
├── is_digit()                - проверка на цифру (0-9)
├── is_space()                - проверка на пробельный символ
├── char_to_digit()           - преобразование символа в цифру
└── is_digit_for_base()       - проверка цифры для заданной системы счисления

УРОВЕНЬ 6: ДАННЫЕ (s21_sscanf.h)
├── s21_sscanf_spec          - структура спецификатора
├── s21_sscanf_context       - контекст парсинга
└── parse_result             - результат парсинга
*/

#include "s21_sscanf.h"

#include <stdarg.h>

#include "s21_string.h"

int s21_sscanf(const char *str, const char *format, ...) {
  int result = 0;
  int is_eof = 0;

  if (str != S21_NULL && format != S21_NULL) {
    const char *str_ptr = str;
    skip_whitespace(&str_ptr);

    if (*str_ptr == '\0') {
      const char *format_ptr = format;
      skip_whitespace(&format_ptr);

      if (*format_ptr != '\0') {
        is_eof = 1;
        result = -1;
      }
    }

    if (!is_eof) {
      va_list args;
      va_start(args, format);

      s21_sscanf_context context = {.str = str,
                                    .format = format,
                                    .args = &args,
                                    .assignments = 0,
                                    .error = 0,
                                    .read_count = 0,
                                    .start_str = str};

      process_format_string(&context);
      result = context.assignments;

      va_end(args);
    }
  }

  return result;
}