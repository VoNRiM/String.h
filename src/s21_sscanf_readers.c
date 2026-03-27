#include "s21_sscanf.h"

// Обработка переполнения
static int handle_overflow(long long *value, int digit, int base) {
  int result = 0;
  if (*value > LLONG_MAX / base) result = 1;
  long long multiplied = *value * base;
  if (multiplied > LLONG_MAX - digit) result = 1;
  *value = multiplied + digit;
  return result;
}

// Чтение цифр
static int read_digits(const char **current, long long *value, int width_left,
                       int base, int *overflow) {
  int digits_count = 0;

  while ((width_left == 0 || digits_count < width_left) &&
         is_digit_for_base(**current, base) && **current) {
    int digit = char_to_digit(**current);

    if (!(*overflow)) {
      *overflow = handle_overflow(value, digit, base);
    }
    (*current)++;
    digits_count++;
  }

  return digits_count;
}

// Обработка знака (+/-)
static void process_sign(const char **current, int *width_left, int *negative,
                         int allow_sign) {
  if (allow_sign) {
    if (**current == '-') {
      *negative = 1;
      (*current)++;
      if (*width_left > 0) (*width_left)--;
    } else if (**current == '+') {
      (*current)++;
      if (*width_left > 0) (*width_left)--;
    }
  } else {
    // Для беззнаковых: пропускаем знак
    if (**current == '-' || **current == '+') {
      (*current)++;
      if (*width_left > 0) (*width_left)--;
    }
  }
}

// Завершающая обработка результата чтения числа
static int finalize_result(const char *start, const char *current,
                           int digits_count, long long *value, int overflow,
                           int negative, int allow_sign, long long *result) {
  int processed_chars = 0;

  if (digits_count > 0) {
    if (overflow) {
      *value = negative ? LLONG_MIN : LLONG_MAX;
    } else if (negative && allow_sign) {
      *value = -(*value);
    }
    *result = *value;
    processed_chars = current - start;
  } else if (current > start && !allow_sign) {
    *result = 0;
    processed_chars = current - start;
  }

  return processed_chars;
}

// Основная функция чтения цифр с заданным основанием
int read_number_with_base(const char *str, long long *result, int width,
                          int base, int allow_sign) {
  const char *start = str, *current = str;
  int negative = 0, digits_count = 0, overflow = 0, width_left = width;
  long long value = 0;

  // Пропуск пробелов
  skip_whitespace(&current);

  // Определение системы счисления
  if (base == 0) {
    base = detect_base(&current, &width_left, &negative, allow_sign);

  } else {
    // Обработка знака
    process_sign(&current, &width_left, &negative, allow_sign);

    if (base == 16 && *current == '0' &&
        (current[1] == 'x' || current[1] == 'X')) {
      current += 2;
      if (width_left > 0) width_left -= 2;
    }
  }

  if (width_left < 0) width_left = 0;

  // Чтение цифр
  digits_count = read_digits(&current, &value, width_left, base, &overflow);

  // Финальная обработка
  int processed = finalize_result(start, current, digits_count, &value,
                                  overflow, negative, allow_sign, result);

  return processed;
}

// Определить систему счисления числа по префиксу
int detect_base(const char **str, int *width, int *negative, int is_signed) {
  int base = 10;

  skip_whitespace(str);

  // Проверка на знак для знаковых чисел
  if (is_signed) {
    if (**str == '-') {
      *negative = 1;
      (*str)++;
      if (*width > 0) (*width)--;
    } else if (**str == '+') {
      (*str)++;
      if (*width > 0) (*width)--;
    }
  } else {
    // Для беззнаковых: пропускаем знак
    if (**str == '-' || **str == '+') {
      (*str)++;
      if (*width > 0) (*width)--;
      *negative = 0;
    }
  }

  // Определение системы счисления
  if (**str == '0') {
    (*str)++;
    if (*width > 0) (*width)--;

    if (**str == 'x' || **str == 'X') {
      base = 16;
      (*str)++;
      if (*width > 0) (*width)--;
    } else {
      base = 8;
    }
  }
  return base;
}

// Чтение символа(ов)
int read_char(const char *str, char *value, int width) {
  int read_width = (width > 0) ? width : 1;
  int i = 0;

  // Читаем width символов (или 1 если width не указана)
  while (i < read_width && *str != '\0') {
    value[i] = str[i];
    i++;
  }

  return i;
}

void init_float_context(s21_sscanf_float_reader_context *context,
                        const char *str_src, int width) {
  context->have_any_mantissa = 0;
  context->pos = str_src;
  context->value = 0.0L;
  context->sign = 1;
  context->width_left = (width > 0) ? width : INT_MAX;
  context->total_chars_consumed = 0;
  context->res = 0;
}

void read_mastissa_before_dot(s21_sscanf_float_reader_context *context) {
  while (context->width_left > 0 && *context->pos != '\0' &&
         is_digit(*context->pos)) {
    int digit = char_to_digit(*context->pos);
    context->value = context->value * 10.0L + (long double)digit;

    context->pos++;
    context->total_chars_consumed++;
    if (context->width_left != INT_MAX) context->width_left--;
    context->have_any_mantissa = 1;
  }
}

void read_mastissa_after_dot(s21_sscanf_float_reader_context *context) {
  if (context->width_left > 0 && *context->pos != '\0' &&
      *context->pos == '.') {
    context->pos++;
    context->total_chars_consumed++;
    if (context->width_left != INT_MAX) context->width_left--;

    long double scale = 0.1L;
    while (context->width_left > 0 && *context->pos != '\0' &&
           is_digit(*context->pos)) {
      int digit = char_to_digit(*context->pos);
      context->value += (long double)digit * scale;
      scale *= 0.1L;  // уменьшение разрядности дробной части. -> 0.1 -> 0.01 ->
                      // 0.001 ...

      context->pos++;
      context->total_chars_consumed++;
      if (context->width_left != INT_MAX) context->width_left--;
      context->have_any_mantissa = 1;
    }
  }
}

void read_exponent(s21_sscanf_float_reader_context *context) {
  if (context->have_any_mantissa && context->width_left > 0 &&
      *context->pos != '\0' && (*context->pos == 'e' || *context->pos == 'E')) {
    const char *e_start = context->pos;
    int backup_total_chars_consumed = context->total_chars_consumed;
    int backup_width_left = context->width_left;

    context->pos++;
    context->total_chars_consumed++;
    if (context->width_left != INT_MAX) {
      context->width_left--;
    }
    // тут можно применить process_context->sign(), но не факт что удобнее
    int e_sign = 1;
    if (context->width_left > 0 &&
        (*context->pos == '-' || *context->pos == '+')) {
      e_sign = (*context->pos == '-') ? -1 : 1;
      context->pos++;
      context->total_chars_consumed++;
      if (context->width_left != INT_MAX) {
        context->width_left--;
      }
    }

    int have_any_exponent = 0;
    long e_value = 0;
    while (context->width_left > 0 && *context->pos != '\0' &&
           is_digit(*context->pos)) {
      have_any_exponent = 1;
      e_value = e_value * 10 + char_to_digit(*context->pos);
      context->pos++;
      context->total_chars_consumed++;
      if (context->width_left != INT_MAX) {
        context->width_left--;
      }
    }

    if (have_any_exponent && e_value != 0) {
      // применяем экспоненту (мантисса * 10^экспонента)
      context->value *= powl(10.0L, (long double)(e_sign * e_value));
    } else {
      // если что-то не получилось с экспонентой - откатываемся
      context->pos = e_start;
      context->total_chars_consumed = backup_total_chars_consumed;
      context->width_left = backup_width_left;
    }
  }
}

int read_signed_float(const char *str_src, long double *value_dst, int width,
                      int *chars_consumed) {
  // ======^^^ Начальные инициализации ^^^======
  *chars_consumed = 0;
  s21_sscanf_float_reader_context context = {0};
  init_float_context(&context, str_src, width);

  // ======^^^ проверка знака числа ^^^======
  if (*context.pos == '-' || *context.pos == '+') {
    context.sign = (*context.pos == '-') ? -1 : 1;
    context.pos++;
    context.total_chars_consumed++;
    if (context.width_left != INT_MAX) {
      context.width_left--;
    }
  }

  read_mastissa_before_dot(&context);
  read_mastissa_after_dot(&context);
  read_exponent(&context);

  // ======^^^ Обработка результатов ^^^======
  if (context.have_any_mantissa) {
    context.value *= (long double)context.sign;

    *value_dst = context.value;
    *chars_consumed = context.total_chars_consumed;
    context.res = 1;
  } else {
    *chars_consumed = 0;
    context.res = 0;
  }

  return context.res;
}

// чтение указателя
int read_ptr(const char *str, void **result, int width, int *chars_consumed) {
  int success = 0, wleft = width > 0 ? width : INT_MAX, prefix = 0, digits = 0;
  const char *cur = str;
  unsigned long long val = 0;
  // Пропуск начальных пробелов с учетом ширины
  while (wleft > 0 && is_space(*cur)) cur++, wleft--;
  // Вариант 1: чтение (nil) как NULL указателя
  if (wleft >= 5 && cur[0] == '(' && cur[1] == 'n' && cur[2] == 'i' &&
      cur[3] == 'l' && cur[4] == ')') {
    *chars_consumed = cur + 5 - str;
    if (result) *result = S21_NULL;
    success = 1;
  }
  // Вариант 2: чтение шестнадцатеричного числа
  if (!success) {
    // Проверяем необязательный префикс 0x или 0X
    if (cur[0] == '0' && (cur[1] == 'x' || cur[1] == 'X') && wleft >= 2) {
      prefix = 1;
      cur += 2;
      wleft -= 2;
    }
    // Чтение шестнадцатеричных цифр
    while (wleft > 0 && is_digit_for_base(*cur, 16) && *cur) {
      val = val * 16 + char_to_digit(*cur);
      cur++;
      wleft--;
      digits++;
    }
    // Успех если: были цифры ИЛИ был префикс 0x
    if (digits > 0 || prefix) {
      *chars_consumed = cur - str;
      // Если были цифры - возвращаем значение, иначе NULL
      if (result) *result = digits > 0 ? (void *)(s21_size)val : S21_NULL;
      success = 1;
    }
  }
  // Если чтение неудачно - обнуляем счетчик прочитанных символов
  if (!success) *chars_consumed = 0;
  return success;
}