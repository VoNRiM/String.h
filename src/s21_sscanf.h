#ifndef S21_SSCANF_H
#define S21_SSCANF_H

#include <limits.h>
#include <math.h>  // для powl() внутри read_signed_float
#include <stdarg.h>
#include <stdio.h>

#include "s21_string.h"  // для s21_size и S21_NULL

// Структура спецификатора
typedef struct {
  int width;  // Ширина чтения (0 = не ограничено)
  int suppress;  // Подавление присваивания (1 = есть '*')
  char specifier;  // Спецификатор ('c', 'd', 'f', 's', 'u', 'o', 'x', 'X', 'i',
                   // '%')
  int skip_whitespace;  // Пропускать ли пробелы перед чтением
  char length;  // Модификатор длины ('h', 'l', 'L', '\0')
} s21_sscanf_spec;

// Контекст парсинга
typedef struct {
  const char *str;  // Текущая позиция во входной строке
  const char *format;  // Текущая позиция в формате
  va_list *args;  // Аргументы (переменные для записи)
  int assignments;         // Успешные присваивания
  unsigned int error : 1;  // Флаг ошибки
  int read_count;  // Всего прочитано символов (для %n)
  const char *start_str;  // Начальная позиция строки (для расчета %n)

} s21_sscanf_context;

// Результат парсинга
typedef struct {
  int success;           // Успешно распарсено
  s21_sscanf_spec spec;  // Распарсенный спецификатор
} parse_result;

// Флаги для функции-обработчика плавающих чисел
typedef struct {
  // Общий накопитель числа
  long double value;

  int sign;
  const char *pos;
  int width_left;
  int total_chars_consumed;
  int res;
  int have_any_mantissa;
} s21_sscanf_float_reader_context;

// Основная функция
int s21_sscanf(const char *str, const char *format, ...);

// Основной цикл обработки
void process_format_string(s21_sscanf_context *ctx);
void process_specifier(s21_sscanf_context *ctx);
void process_literal(s21_sscanf_context *ctx);

// Парсинг
parse_result parse_specifier(const char **format);

// Диспетчер
int dispatch_handler(s21_sscanf_context *ctx, s21_sscanf_spec *spec);

// Обработчики
int handle_char(s21_sscanf_context *ctx, s21_sscanf_spec *spec);
int handle_int_decimal(s21_sscanf_context *ctx, s21_sscanf_spec *spec);
int handle_percent(s21_sscanf_context *ctx, s21_sscanf_spec *spec);
int handle_string(s21_sscanf_context *context, s21_sscanf_spec *spec);
int handle_int_multi(s21_sscanf_context *context, s21_sscanf_spec *spec);
int handle_floats(s21_sscanf_context *context, s21_sscanf_spec *spec);
int handle_unsigned_decimal(s21_sscanf_context *context, s21_sscanf_spec *spec);
int handle_unsigned_octal(s21_sscanf_context *context, s21_sscanf_spec *spec);
int handle_unsigned_hex(s21_sscanf_context *context, s21_sscanf_spec *spec);
int handle_ptr(s21_sscanf_context *context, s21_sscanf_spec *spec);
int handle_count(s21_sscanf_context *context, s21_sscanf_spec *spec);

// Чтение данных
int read_number_with_base(const char *str, long long *result, int width,
                          int base, int allow_sign);
int read_char(const char *str, char *value, int width);
int read_signed_float(const char *str_src, long double *value_dst, int width,
                      int *chars_consumed);
// int read_simple_integer(const char* str, int* value, int width); не
// используется??
int read_ptr(const char *str, void **result, int width, int *chars_consumed);

int is_digit_for_base(char c, int base);
int detect_base(const char **str, int *width, int *negative, int allow_sign);

// Утилиты
void skip_whitespace(const char **str);
int is_digit(char c);
int char_to_digit(char c);
int is_space(char c);
int char_to_digit(char c);
int is_float_spec(char c);

int process_value_with_modifier(s21_sscanf_context *context,
                                s21_sscanf_spec *spec, int is_signed,
                                long long value);

// Рефакторинг read_signed_float()
void init_float_context(s21_sscanf_float_reader_context *context,
                        const char *str_src, int width);
void read_mastissa_before_dot(s21_sscanf_float_reader_context *context);
void read_exponent(s21_sscanf_float_reader_context *context);

#endif