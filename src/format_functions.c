// Bonus assignment for extra points. You need to implement some format
// modifiers of the sprintf function from the stdio.h library:

//     The function must be placed in the s21_string.h library.
//     All of the requirements outlined in the first part are applied to
//     function implementation. The next additional format modifiers must be
//     supported:
//         Specifiers: g, G, e, E, x, X, o, p
//         Flags: #, 0
//         Width description: *
//         Precision description: .*
//         Length description: L
#include "format_functions.h"

#include <iso646.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

#include "s21_string.h"

void s21_strcpy(char *dest, const char *src) {
  while (*src != '\0') {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = '\0';
}

int s21_sprintf(char *str, const char *format, ...) {
  FormatSpecs spec_flags = {0};
  int format_index = 0;
  int str_index = 0;
  int index_error = 0;
  int error = 0;
  va_list args;
  str[0] = '\0';
  va_start(args, format);  // Инициализируем аргументы после Format
  while (format[format_index] != '\0' && error == 0) {
    if (format[format_index] != '%') {
      str[str_index] = format[format_index];
      format_index++;
      str_index++;
    } else {
      format_index++;
      flags_parser(&spec_flags, format, &format_index);
      width_parser(&spec_flags, format, &format_index, args);
      precision_parser(&spec_flags, format, &format_index, args);
      length_parser(&spec_flags, format, &format_index);
      specifiers_parser(&spec_flags, format, &format_index);
      if (s21_strchr(spec_flags.length_descriptor, 'l') != S21_NULL &&
          spec_flags.specifier == 's' &&
          s21_strlen(spec_flags.length_descriptor) < 1) {
        error = 1;
        index_error = str_index;
      } else {
        append_argument(args, &spec_flags, str, format, &str_index,
                        &format_index);
      }
      s21_memset(&spec_flags, 0, sizeof(FormatSpecs));
    }
  }
  if (error == 1) {
    str[index_error + 1] = '\0';
  } else {
    str[str_index] = '\0';
  }
  va_end(args);
  return 0;
}

void flags_parser(FormatSpecs *spec_flags, const char *format,
                  int *format_index) {
  int n = 0;
  while (s21_strpbrk(&format[*format_index], SPRINTF_FLAGS) ==
         &format[*format_index]) {
    char c = format[*format_index];
    if (s21_strchr(spec_flags->flags, c) == S21_NULL) {
      spec_flags->flags[n++] = c;
    }
    (*format_index)++;
  }
  spec_flags->flags[n] = '\0';
}

void width_parser(FormatSpecs *spec_flags, const char *format,
                  int *format_index, va_list args) {
  if (format[*format_index] == '*') {
    spec_flags->length = va_arg(args, int);
    (*format_index)++;
  } else {
    while (s21_strpbrk(&format[*format_index], DIGIT_LIST) ==
           &format[*format_index]) {
      spec_flags->length =
          spec_flags->length * 10 + (format[*format_index] - '0');
      (*format_index)++;
    }
  }
}

void precision_parser(FormatSpecs *spec_flags, const char *format,
                      int *format_index, va_list args) {
  spec_flags->precision = -1;
  if (format[*format_index] == '.') {
    (*format_index)++;
    spec_flags->precision = 0;
    if (format[*format_index] == '*') {
      spec_flags->precision = va_arg(args, int);
      (*format_index)++;
    }
    while (s21_strpbrk(&format[*format_index], DIGIT_LIST) ==
               &format[*format_index] &&
           spec_flags->star_in_precision < 1) {
      spec_flags->precision =
          spec_flags->precision * 10 + (format[*format_index] - '0');
      (*format_index)++;
    }
  }
}

void length_parser(FormatSpecs *spec_flags, const char *format,
                   int *format_index) {
  int n = 0;
  while (s21_strpbrk(&format[*format_index], LENGTH_DESCRIPTORS) ==
         &format[*format_index]) {
    char c = format[*format_index];
    if (s21_strchr(spec_flags->length_descriptor, c) == S21_NULL) {
      spec_flags->length_descriptor[n++] = c;
    }
    (*format_index)++;
  }
  spec_flags->length_descriptor[n] = '\0';
}

void specifiers_parser(FormatSpecs *spec_flags, const char *format,
                       int *format_index) {
  if (s21_strpbrk(&format[*format_index], SPECIFIERS) ==
      &format[*format_index]) {
    spec_flags->specifier = format[*format_index];
    if (format[*format_index + 1] != '\0') {
    }
  }
}

void append_argument(va_list args, FormatSpecs *spec_flags, char *str,
                     const char *format, int *str_index, int *format_index) {
  // Если указано больше двух флагов делаем поправку.
  if (format[*format_index] == 'd') {
    append_d(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 'c') {
    append_c(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 's') {
    append_s(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 'f') {
    append_f(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 'u') {
    append_u(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 'g' || format[*format_index] == 'G') {
    append_g(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 'e' || format[*format_index] == 'E') {
    append_e(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 'x' || format[*format_index] == 'X') {
    append_x(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 'o') {
    append_o(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == 'p') {
    append_p(args, spec_flags, str, str_index, format_index);
  } else if (format[*format_index] == '%') {
    str[*str_index] = '%';
    (*str_index)++;
    *format_index += 1;
  }
}

void format_precision(FormatSpecs *spec_flags, int *len, char *word) {
  if (spec_flags->precision > *len && spec_flags->specifier != 's') {
    int space_len = spec_flags->precision - *len;
    for (int i = *len - 1; i >= 0; i--) {
      word[i + space_len] = word[i];
    }
    for (int i = 0; i < space_len; i++) {
      word[i] = '0';
    }
    word[*len + space_len] = '\0';
    *len = spec_flags->precision;
  }
  if (spec_flags->precision < *len && spec_flags->specifier == 's' &&
      spec_flags->precision >= 0) {
    word[spec_flags->precision] = '\0';
    *len = spec_flags->precision;
  }
}

void format_flag(FormatSpecs *spec_flags, int *len, char *word, int value) {
  if (s21_strchr(spec_flags->flags, '+') != S21_NULL) {
    if (value >= 0) {
      for (int i = *len; i >= 0; i--) {
        word[i + 1] = word[i];
      }
      word[0] = '+';
      (*len)++;
    }
  } else if (s21_strchr(spec_flags->flags, ' ') != S21_NULL) {
    if (value >= 0) {
      for (int i = *len; i >= 0; i--) {
        word[i + 1] = word[i];
      }
      word[0] = ' ';
      (*len)++;
    }
  }
}

void format_width(FormatSpecs *spec_flags, int *len, char *word) {
  int is_negativ = 0;
  int space_len = spec_flags->length - *len;
  if (s21_strchr(spec_flags->flags, '-') != S21_NULL) {
    if (*len < spec_flags->length) {
      for (int i = 0; i < space_len; i++) {
        word[*len + i] = ' ';
      }
      (*len) += space_len;
      word[*len] = '\0';
    }
  } else if (s21_strchr(spec_flags->flags, '0') != S21_NULL &&
             *len < spec_flags->length) {
    if (word[0] == '-') {
      is_negativ = 1;
      s21_strcpy(word, word + 1);
      *len -= 1;
      space_len = spec_flags->length - *len;
    }
    if (*len < spec_flags->length) {
      for (int i = *len; i >= 0; i--) {
        word[i + space_len] = word[i];
      }
    }
    for (int i = 0; i < space_len; i++) {
      word[i] = '0';
    }
    (*len) += space_len;
    if (is_negativ) {
      word[0] = '-';
    }
  }
  if (spec_flags->length > 0 && *len < spec_flags->length) {
    for (int i = *len; i >= 0; i--) {
      word[i + space_len] = word[i];
    }
    for (int i = 0; i < space_len; i++) {
      word[i] = ' ';
    }
    (*len) += space_len;
  }
}
// Добавляем число
void append_d(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;

  char word[5000];
  long value = 0;

  if (spec_flags->length_descriptor[0] == 'l') {
    value = va_arg(args, long);
    long_to_char(value, word);
  } else if (spec_flags->length_descriptor[0] == 'h') {
    value = (short)va_arg(args, long);
    int_to_char(value, word);
  } else {
    value = va_arg(args, int);
    int_to_char(value, word);
  }

  int len = (int)s21_strlen(word);
  format_precision(spec_flags, &len,
                   word);  // ЗДесь мы добавляем 0 перед числом
  format_flag(spec_flags, &len, word, value);  // здесь
  if (spec_flags->precision == 0 && value == 0) {
    word[0] = '\0';
    len = 0;
  }
  format_width(spec_flags, &len, word);
  s21_size word_len = s21_strlen(word);
  s21_strncpy(str + *str_index, word, word_len);
  *str_index += word_len;
}

void append_c(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;

  int ch_int = va_arg(
      args, int);  // всё равно берём инт из-за особоенностей работы va_arg

  char word[5000];
  word[0] = (char)ch_int;
  word[1] = '\0';
  int len = (int)s21_strlen(word);
  format_width(spec_flags, &len, word);
  s21_size str_len = s21_strlen(word);
  s21_strncpy(str + *str_index, word, str_len);
  *str_index += str_len;
}

// Добавляем слово.
void append_s(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;
  char *string = va_arg(args, char *);
  char word[5000];
  int n = 0;
  while (string[n] != '\0') {
    word[n] = string[n];
    n++;
  }
  word[n] = '\0';
  int len = (int)s21_strlen(word);
  format_precision(spec_flags, &len, word);
  format_width(spec_flags, &len, word);
  s21_size str_len = s21_strlen(word);
  s21_strncpy(str + *str_index, word, str_len);
  *str_index += str_len;
}

// Добавляем число с точкой
void append_f(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;
  long double double_val = 0.0;
  if (s21_strchr(spec_flags->length_descriptor, 'L') != S21_NULL) {
    double_val = va_arg(args, long double);
  } else {
    double_val = va_arg(args, double);
  }
  char string[5000] = {0};
  if (!check_to_inf_nan(double_val, string, spec_flags)) {
    int precision = 6;
    if (spec_flags->precision == -1) {
      precision = 6;
    } else {
      precision = spec_flags->precision;
    }
    float_to_char(double_val, string, precision);
    int len = (int)s21_strlen(string);
    format_flag(spec_flags, &len, string, double_val);
    apply_hash_flag_for_float(spec_flags, string);
    format_width(spec_flags, &len, string);
  }
  s21_size str_len = s21_strlen(string);
  s21_strncpy(str + *str_index, string, str_len);
  *str_index += str_len;
}

// добавляем безнаковое число
void append_u(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;
  char string[5000];
  unsigned long un_val = 0;
  if (spec_flags->length_descriptor[0] == 'l') {
    un_val = va_arg(args, unsigned long);
    long_uni_to_char(un_val, string);
  } else if (spec_flags->length_descriptor[0] == 'h') {
    un_val = va_arg(args, unsigned int);
    short_uni_to_char(un_val, string);
  } else {
    un_val = va_arg(args, unsigned int);
    uni_to_char(un_val, string);
  }
  int len = (int)s21_strlen(string);
  format_precision(spec_flags, &len, string);
  if (spec_flags->precision == 0 && un_val == 0) {
    string[0] = '\0';
    len = 0;
  }
  format_width(spec_flags, &len, string);
  s21_size str_len = s21_strlen(string);
  s21_strncpy(str + *str_index, string, str_len);
  *str_index += str_len;
}

void append_g(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;
  char buffer[5000];
  long double value = 0;
  if (s21_strchr(spec_flags->length_descriptor, 'L') != S21_NULL) {
    value = va_arg(args, long double);
  } else {
    value = va_arg(args, double);
  }
  if (!check_to_inf_nan(value, buffer, spec_flags)) {
    int precision = (spec_flags->precision == -1) ? 6 : spec_flags->precision;
    if (precision == 0) precision = 1;
    long double abs_value = (value < 0) ? -value : value;
    int exponent = 0;
    find_exponent(&exponent, &abs_value);  // находим экспоненту
    if (exponent < -4 || exponent >= precision) {
      create_exponential_contact(value, buffer, precision - 1,
                                 (spec_flags->specifier == 'g') ? 'e' : 'E');
      remove_tralling_zeroes_exponential(buffer);
    } else {
      int f_precision = precision - (exponent + 1);
      if (f_precision < 0) f_precision = 0;
      float_to_char((double)value, buffer, f_precision);
      remove_zeroes(buffer);
    }
  }
  int len = (int)s21_strlen(buffer);
  format_flag(spec_flags, &len, buffer, value);
  apply_hash_flag_for_float(spec_flags, buffer);
  format_width(spec_flags, &len, buffer);

  s21_size word_len = s21_strlen(buffer);
  s21_strncpy(str + *str_index, buffer, word_len);
  *str_index += word_len;
}

void append_e(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;

  char buffer[5000] = "";
  long double value = 0;
  if (s21_strchr(spec_flags->length_descriptor, 'L') != S21_NULL) {
    value = va_arg(args, long double);
  } else {
    value = va_arg(args, double);
  }
  if (!check_to_inf_nan(value, buffer, spec_flags)) {
    int precision = (spec_flags->precision == -1) ? 6 : spec_flags->precision;
    if (precision == 0) precision = 1;
    int exponent = 0;
    int is_negative = (value < 0);
    long double abs_value = is_negative ? -value : value;
    find_exponent(&exponent, &abs_value);
    if (is_negative) abs_value = -abs_value;
    char exp_char = (spec_flags->specifier == 'e') ? 'e' : 'E';
    create_exponential_contact(value, buffer, precision, exp_char);
    int len = (int)s21_strlen(buffer);
    format_flag(spec_flags, &len, buffer, value);
    format_width(spec_flags, &len, buffer);
    apply_hash_flag_for_float(spec_flags, buffer);
  }
  s21_size word_len = s21_strlen(buffer);
  s21_strncpy(str + *str_index, buffer, word_len);
  *str_index += word_len;
}

void append_x(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;
  char buffer[5000] = "";
  uint64_t value = 0;
  if (s21_strchr(spec_flags->length_descriptor, 'l') != S21_NULL)
    value = va_arg(args, unsigned long);
  else if (s21_strchr(spec_flags->length_descriptor, 'h') != S21_NULL)
    value = (unsigned short)va_arg(args, unsigned int);
  else
    value = va_arg(args, unsigned int);

  unsigned_num_to_char(value, buffer, 16);
  if (spec_flags->specifier == 'X') {
    for (int i = 0; buffer[i]; i++) {
      if (buffer[i] >= 'a' && buffer[i] <= 'f') {
        buffer[i] = buffer[i] - 'a' + 'A';
      }
    }
  }
  int len = (int)s21_strlen(buffer);
  format_precision(spec_flags, &len, buffer);
  if (s21_strchr(spec_flags->flags, '#') != S21_NULL && value != 0) {
    for (int i = len; i >= 0; i--) {
      buffer[i + 2] = buffer[i];
    }
    buffer[0] = '0';
    buffer[1] = (spec_flags->specifier == 'X') ? 'X' : 'x';
    len += 2;
  }
  format_width(spec_flags, &len, buffer);
  s21_strncpy(str + *str_index, buffer, len);
  *str_index += len;
}

void append_o(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;
  char buffer[5000] = "";
  uint64_t value = 0;
  if (s21_strchr(spec_flags->length_descriptor, 'l') != S21_NULL)
    value = va_arg(args, unsigned long);
  else if (s21_strchr(spec_flags->length_descriptor, 'h') != S21_NULL)
    value = (unsigned short)va_arg(args, unsigned int);
  else
    value = va_arg(args, unsigned int);

  unsigned_num_to_char(value, buffer, 8);
  int len = (int)s21_strlen(buffer);
  format_precision(spec_flags, &len, buffer);
  if (s21_strchr(spec_flags->flags, '#') != S21_NULL && value != 0) {
    for (int i = len; i >= 0; i--) {
      buffer[i + 1] = buffer[i];
    }
    buffer[0] = '0';
    len++;
  }
  format_width(spec_flags, &len, buffer);
  s21_strncpy(str + *str_index, buffer, len);
  *str_index += len;
}

void append_p(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index) {
  *format_index += 1;
  char buffer[5000] = "";
  char point[5000] = "";
  void *pointer = va_arg(args, void *);
  if (pointer == S21_NULL) {
    s21_strcpy(buffer, "(nil)");
  } else {
    uint64_t address = (uint64_t)pointer;
    unsigned_num_to_char(address, point, 16);
    int lenght = s21_strlen(point);
    if (spec_flags->precision > lenght) {
      int zeros_to_add = spec_flags->precision - lenght;
      for (int i = lenght - 1; i >= 0; i--) {
        point[i + zeros_to_add] = point[i];
      }
      for (int j = 0; j < zeros_to_add; j++) {
        point[j] = '0';
      }
    }
    buffer[0] = '0';
    buffer[1] = 'x';
    s21_strcpy(buffer + 2, point);
  }
  int len = (int)s21_strlen(buffer);
  format_width(spec_flags, &len, buffer);
  s21_strncpy(str + *str_index, buffer, len);
  *str_index += len;
}

void int_to_char(int num, char *buffer) {  // записываем число в текстовый буфер
  int flag_minus = 0;
  if (num < 0) {  // если отицательно помечаем флаг и убираем минус для работы
    flag_minus = 1;
    num = -num;
  }
  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
  } else {
    int x = num;
    int len = 0;
    while (x != 0) {
      x = x / 10;
      len++;
    }
    char wrong_buffer[len];
    for (int i = 0; i < len; i++) {
      int result_number = num % 10;
      num = num / 10;
      wrong_buffer[i] = '0' + result_number;
    }
    int n = len - 1;
    if (flag_minus == 1) {
      buffer[0] = '-';
    }
    for (int j = flag_minus; j < (len + flag_minus);
         j++) {  // Если есть минус то начинаем за полнять со второго
      buffer[j] = wrong_buffer[n];
      n = n - 1;
    }
    buffer[len + flag_minus] = '\0';
  }
}

void long_to_char(long num,
                  char *buffer) {  // Записываем число в текстовый буфер
  int flag_minus = 0;
  if (num < 0) {  // если отицательно помечаем флаг и убираем минус для работы
    flag_minus = 1;
    num = -num;
  }
  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
  } else {
    long x = num;
    int len = 0;
    while (x != 0) {
      x = x / 10;
      len++;
    }
    char wrong_buffer[len];
    for (int i = 0; i < len; i++) {
      long result_number = num % 10;
      num = num / 10;
      wrong_buffer[i] = '0' + result_number;
    }
    int n = len - 1;
    if (flag_minus == 1) {
      buffer[0] = '-';
    }
    for (int j = flag_minus; j < (len + flag_minus);
         j++) {  // Если есть минус то начинаем за полнять со второго
      buffer[j] = wrong_buffer[n];
      n = n - 1;
    }
    buffer[len + flag_minus] = '\0';
  }
}

void float_to_char(double num, char *buffer, int precision) {
  int is_neganiv = num < 0;
  if (is_neganiv) num = -num;
  double round_adjust = 0.5;
  for (int j = 0; j < precision; j++) {
    round_adjust /= 10.0;
  }
  num += round_adjust;
  long long int_part = (long long)num;
  char int_str[5000];
  int pos = 0;
  long_to_char(int_part, int_str);
  int i = 0;
  if (is_neganiv) {
    buffer[pos++] = '-';
  }
  while (int_str[i] != '\0') {
    buffer[pos++] = int_str[i++];
  }
  if (precision > 0) {
    buffer[pos++] = '.';
    double fraction = num - (double)int_part;
    for (int i = 0; i < precision; i++) {
      fraction *= 10.0;
      int digit = (int)fraction;
      buffer[pos++] = '0' + digit;
      fraction -= digit;
      if (fraction < 0.0) fraction = 0.0;
    }
  }
  buffer[pos] = '\0';
}

// проверка является ли число бесконечным или nan
int check_to_inf_nan(double num, char *buffer, FormatSpecs *spec_flags) {
  int is_special = 0;
  int upper_case =
      (spec_flags->specifier == 'E' || spec_flags->specifier == 'G');
  if (isinf(num)) {
    int is_negativ = (num < 0);
    if (upper_case) {
      s21_strcpy(buffer, is_negativ ? "-INF" : "INF");
    } else {
      s21_strcpy(buffer, is_negativ ? "-inf" : "inf");
    }
    is_special = 1;
  } else if (isnan(num)) {
    s21_strcpy(buffer, upper_case ? "NAN" : "nan");
    is_special = 1;
  }
  return is_special;
}

void uni_to_char(unsigned int num, char *buffer) {
  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return;
  } else {
    unsigned int temp = num;
    int len = 0;
    while (temp != 0) {
      len++;
      temp /= 10;
    }
    temp = num;
    buffer[len] = '\0';
    for (int i = len - 1; i >= 0; i--) {
      buffer[i] = '0' + (temp % 10);
      temp /= 10;
    }
  }
}

void long_uni_to_char(unsigned long num, char *buffer) {
  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return;
  } else {
    unsigned long temp = num;
    int len = 0;
    while (temp != 0) {
      len++;
      temp /= 10;
    }
    temp = num;
    buffer[len] = '\0';
    for (int i = len - 1; i >= 0; i--) {
      buffer[i] = '0' + (temp % 10);
      temp /= 10;
    }
  }
}

void unsigned_num_to_char(uint64_t value, char *src_buffer, int base) {
  int buffer_size = 512;
  char digit_list[] = "0123456789abcdef";
  char buffer[buffer_size];
  int index = 0;

  if (value == 0) {
    src_buffer[0] = '0';
    src_buffer[1] = '\0';
    return;
  }

  while (value > 0 && index < buffer_size - 1) {
    buffer[index++] = digit_list[value % base];
    value /= base;
  }

  buffer[index] = '\0';

  for (int i = 0; i < index; i++) {
    src_buffer[i] = buffer[index - 1 - i];
  }

  src_buffer[index] = '\0';
}

void short_uni_to_char(unsigned int num, char *buffer) {
  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return;
  } else {
    unsigned short temp = num;
    int len = 0;
    while (temp != 0) {
      len++;
      temp /= 10;
    }
    temp = num;
    buffer[len] = '\0';
    for (int i = len - 1; i >= 0; i--) {
      buffer[i] = '0' + (temp % 10);
      temp /= 10;
    }
  }
}

void create_exponential_contact(long double value, char *buffer, int precision,
                                char exp_char) {
  if (precision < 0) precision = 0;
  int exponent = 0;
  long double abs_value = (value < 0) ? -value : value;
  find_exponent(&exponent, &abs_value);
  if (value < 0) abs_value = -abs_value;
  char mantissa[5000] = "";
  float_to_char(abs_value, mantissa, precision);
  s21_strcpy(buffer, mantissa);
  format_mantiss(buffer, (exponent >= 0) ? exponent : -exponent,
                 (exponent >= 0) ? '+' : '-');
  for (int i = 0; buffer[i]; i++) {
    if (buffer[i] == 'e' && exp_char == 'E') {
      buffer[i] = 'E';
    }
  }
}
void remove_zeroes(char *buffer) {
  int length = (int)s21_strlen(buffer);
  char *separator_pointer = s21_strchr(buffer, '.');
  int flag_exit = 0;
  if (separator_pointer != S21_NULL) {
    for (int i = length - 1; i >= 0 && !flag_exit; i--) {
      if (buffer[i] == '0')
        buffer[i] = '\0';
      else
        flag_exit = 1;
    }
    if (separator_pointer[1] == '\0') separator_pointer[0] = '\0';
  }
}

void format_mantiss(char *buffer, int powers, char leading_sign) {
  int length = (int)s21_strlen(buffer);
  buffer[length] = 'e';
  buffer[length + 1] = leading_sign;
  buffer[length + 3] = powers % 10 + '0';
  powers /= 10;
  buffer[length + 2] = powers % 10 + '0';
  buffer[length + 4] = '\0';
}

void find_exponent(int *exponent, long double *abs_value) {
  if (*abs_value == 0.0) {
    (*exponent) = 0;
  } else {
    while (*abs_value >= 10.0) {
      *abs_value /= 10.0;
      (*exponent)++;
    }
    while (*abs_value <= 1.0) {
      *abs_value *= 10.0;
      (*exponent)--;
    }
  }
}

void apply_hash_flag_for_float(FormatSpecs *spec_flags, char *buffer) {
  if (s21_strchr(spec_flags->flags, '#') != S21_NULL) {
    if (s21_strchr("efgEFG", spec_flags->specifier)) {
      char *dot = s21_strchr(buffer, '.');
      if (dot == S21_NULL && spec_flags->precision == 0) {
        int len = (int)s21_strlen(buffer);
        buffer[len] = '.';
        buffer[len + 1] = '\0';
      }
    }
  }
}

void remove_tralling_zeroes_exponential(char *buffer) {
  char *e_pos = s21_strchr(buffer, 'e');
  if (!e_pos) e_pos = s21_strchr(buffer, 'E');
  char *dot = s21_strchr(buffer, '.');
  if (!e_pos || !dot || dot > e_pos) {
    return;
  }
  char *p = e_pos - 1;
  while (p > dot && *p == '0') {
    for (char *src = p + 1; *src != '\0'; src++) {
      *(src - 1) = *src;
    }
    char *end = buffer + s21_strlen(buffer);
    *(end - 1) = '\0';
    e_pos--;
    p--;
  }
  if (dot + 1 == e_pos) {
    for (char *src = dot + 1; *src != '\0'; src++) {
      *(src - 1) = *src;
    }
  }
}
