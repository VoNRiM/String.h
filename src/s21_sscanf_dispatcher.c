
#include "s21_sscanf.h"

static void write_value_by_type(void *ptr, char length_modifier, int is_signed,
                                long long value);
static long long clamp_value(long long value, long long min, long long max);
static int handle_integer_generic(s21_sscanf_context *context,
                                  s21_sscanf_spec *spec, int base,
                                  int is_signed);

// Диспетчер - выбирает нужный обработчик
int dispatch_handler(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  int result = 0;

  if (spec->specifier == 'c') {
    result = handle_char(context, spec);
  } else if (spec->specifier == 'd') {
    result = handle_int_decimal(context, spec);
  } else if (spec->specifier == '%') {
    result = handle_percent(context, spec);
  } else if (spec->specifier == 's') {
    result = handle_string(context, spec);
  } else if (s21_strchr("eEfgG", spec->specifier) != S21_NULL) {
    result = handle_floats(context, spec);
  } else if (spec->specifier == 'i') {
    result = handle_int_multi(context, spec);
  } else if (spec->specifier == 'u') {
    result = handle_unsigned_decimal(context, spec);
  } else if (spec->specifier == 'o') {
    result = handle_unsigned_octal(context, spec);
  } else if (spec->specifier == 'x' || spec->specifier == 'X') {
    result = handle_unsigned_hex(context, spec);
  } else if (spec->specifier == 'p') {
    result = handle_ptr(context, spec);
  } else if (spec->specifier == 'n') {
    result = handle_count(context, spec);
  }

  return result;
}

// Обработчик для %c
int handle_char(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  int result = 0;
  char value[4096] = {0};
  int chars_read = 0;

  chars_read = read_char(context->str, value, spec->width);

  if (chars_read > 0) {
    context->str += chars_read;
    result = 1;

    if (spec->suppress == 0) {
      char *ptr = va_arg(*context->args, char *);
      if (ptr != S21_NULL) {
        for (int i = 0; i < chars_read; i++) ptr[i] = value[i];
      }
      context->assignments++;
    }
  }

  return result;
}

// Обработчик для %%
int handle_percent(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  int result = 0;
  (void)spec;

  if (*context->str == '%') {
    context->str++;
    result = 1;
  }
  return result;
}

// Обработчик для %s venonata
int handle_string(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  int chars_read = 0;
  skip_whitespace(&context->str);
  int is_wide = 0;

  char *ptr = S21_NULL;
  wchar_t *wide_ptr = S21_NULL;
  if (!spec->suppress) {
    if (spec->length == 'l') {
      is_wide = 1;
      wide_ptr = va_arg(*context->args, wchar_t *);
    } else {
      ptr = va_arg(*context->args, char *);
    }
  }
  // если подавление выкл, но оба указателя все равно S21_NULL дальше НЕ идем
  if (!(!spec->suppress && (ptr == S21_NULL && wide_ptr == S21_NULL))) {
    int limit =
        (spec->width > 0) ? spec->width : INT_MAX;  // не совсем по стандарту
    // в реальном sscanf цикл будет БЕЗ ограничения

    while (!is_space(*context->str) && *context->str != '\0' &&
           chars_read < limit) {
      if (!spec->suppress) {
        if (is_wide) {
          *wide_ptr = (wchar_t)(unsigned char)*context->str;
          wide_ptr++;
        } else {
          *ptr = *context->str;
          ptr++;
        }
      }
      context->str++;

      chars_read++;
    }

    if (chars_read > 0) {
      if (!spec->suppress && is_wide == 0) {
        *ptr = '\0';
        context->assignments++;
      } else if (!spec->suppress && is_wide == 1) {
        wide_ptr++;
        context->assignments++;
      }
    }
  }

  return (chars_read > 0) ? 1 : 0;
}

// Общий обработчик для e E f g G чисел с плавающей точкой
int handle_floats(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  skip_whitespace(&context->str);

  int chars_read = 0;
  long double value = 0.0;
  int is_parsed =
      read_signed_float(context->str, &value, spec->width, &chars_read);
  int result = (chars_read && is_parsed);

  // == Выше прочли число, если чтение успешно - сдвигаемся по строке и решаем
  // что с ним делать
  if (result) {
    context->str += chars_read;

    if (!spec->suppress && is_float_spec(spec->specifier)) {
      if (spec->length == 'L') {
        long double *ptr = va_arg(*context->args, long double *);
        if (ptr != S21_NULL) {
          *ptr = value;
          context->assignments++;
        }
      } else if (spec->length == 'l') {
        double *ptr = va_arg(*context->args, double *);
        if (ptr != S21_NULL) {
          *ptr = (double)value;
          context->assignments++;
        }
      } else {
        float *ptr = va_arg(*context->args, float *);
        if (ptr != S21_NULL) {
          *ptr = (float)value;
          context->assignments++;
        }
      }
    }
  }

  return result;
}

// Обработчик для десятичного целого %d
int handle_int_decimal(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  return handle_integer_generic(context, spec, 10, 1);  // base=10, signed=1
}
// Обработчик для целого с автоопределением системы счисления %i
int handle_int_multi(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  return handle_integer_generic(context, spec, 0, 1);  // base=0, signed=1
}

// Единая общая функция
static int handle_integer_generic(s21_sscanf_context *context,
                                  s21_sscanf_spec *spec, int base,
                                  int is_signed) {
  int result = 0;
  long long value = 0;
  int chars_read =
      read_number_with_base(context->str, &value, spec->width, base, 1);

  if (chars_read > 0) {
    context->str += chars_read;

    if (!spec->suppress) {
      if (process_value_with_modifier(context, spec, is_signed, value)) {
        context->assignments++;
      }
    } else {
      // Для suppress (*) также считаем успешное чтение
      result = 1;
    }
    result = 1;
  }

  return result;
}

// Алиас для десятичных беззнаковых чисел %u
int handle_unsigned_decimal(s21_sscanf_context *context,
                            s21_sscanf_spec *spec) {
  return handle_integer_generic(context, spec, 10, 0);
}
// Обработчик для восьмеричных беззнаковых чисел %o
int handle_unsigned_octal(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  return handle_integer_generic(context, spec, 8, 0);
}
// Обработчик для шестнадцатеричных беззнаковых чисел %x/%X
int handle_unsigned_hex(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  return handle_integer_generic(context, spec, 16, 0);
}

// Обработчик для %p (указатель)
int handle_ptr(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  int result = 0;
  int chars_read = 0;
  void *value = S21_NULL;

  if (spec->skip_whitespace) {
    skip_whitespace(&context->str);
  }

  if (read_ptr(context->str, &value, spec->width, &chars_read)) {
    if (chars_read > 0) {
      context->str += chars_read;  // ОБНОВЛЯЕМ ПОЗИЦИЮ
      result = 1;

      if (!spec->suppress) {
        void **ptr = va_arg(*context->args, void **);
        if (ptr != S21_NULL) {
          *ptr = value;
          context->assignments++;
        }
      }
    }
  }

  return result;
}

// Обработчик для %n (подсчет прочитанных символов)
int handle_count(s21_sscanf_context *context, s21_sscanf_spec *spec) {
  int result = 1;  // %n всегда успешен

  if (!spec->suppress) {
    int *ptr = va_arg(*context->args, int *);

    if (ptr != S21_NULL) {
      int count = context->str - context->start_str;

      *ptr = count;

    } else {
    }
  }

  return result;
}

// ======вспомогательные функции=================
// Получить указатель из списка аргументов с учетом модификатора длины
static void *get_pointer_by_modifier(va_list *args, char length_modifier,
                                     int is_signed, s21_size base_size) {
  void *ptr = NULL;

  if (length_modifier == 'h') {
    if (is_signed) {
      if (base_size == sizeof(int)) {
        ptr = va_arg(*args, short *);
      }
    } else {
      if (base_size == sizeof(unsigned int)) {
        ptr = va_arg(*args, unsigned short *);
      }
    }
  } else if (length_modifier == 'l') {
    if (is_signed) {
      if (base_size == sizeof(int)) {
        ptr = va_arg(*args, long *);
      }
    } else {
      if (base_size == sizeof(unsigned int)) {
        ptr = va_arg(*args, unsigned long *);
      }
    }
  } else {
    // Базовый тип (int или unsigned int)
    if (is_signed) {
      ptr = va_arg(*args, int *);
    } else {
      ptr = va_arg(*args, unsigned int *);
    }
  }

  return ptr;
}

// Ограничие значения диапазоном
static long long clamp_value(long long value, long long min, long long max) {
  long long result = value;
  if (value < min) result = min;
  if (value > max) result = max;
  return result;
}

// Запись значения по указателю с учетом типа и модификатора
static void write_value_by_type(void *ptr, char length_modifier, int is_signed,
                                long long value) {
  if (ptr != S21_NULL) {
    if (length_modifier == 'h') {
      if (is_signed) {
        *((short *)ptr) = (short)clamp_value(value, SHRT_MIN, SHRT_MAX);
      } else {
        *((unsigned short *)ptr) = (unsigned short)value;  // wrap-around
      }
    } else if (length_modifier == 'l') {
      if (is_signed) {
        *((long *)ptr) = (long)value;
      } else {
        *((unsigned long *)ptr) = (unsigned long)value;
      }
    } else {
      if (is_signed) {
        *((int *)ptr) = (int)clamp_value(value, INT_MIN, INT_MAX);
      } else {
        *((unsigned int *)ptr) = (unsigned int)value;
      }
    }
  }
}

// Обработчик значения с учетом модификатора длины и типа
int process_value_with_modifier(s21_sscanf_context *context,
                                s21_sscanf_spec *spec, int is_signed,
                                long long value) {
  int result = 0;
  if (!spec->suppress) {
    void *ptr =
        get_pointer_by_modifier(context->args, spec->length, is_signed,
                                is_signed ? sizeof(int) : sizeof(unsigned int));
    if (ptr != S21_NULL) {
      write_value_by_type(ptr, spec->length, is_signed, value);
      result = 1;
    }
  }
  return result;
}
