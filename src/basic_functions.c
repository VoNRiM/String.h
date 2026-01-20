#include "errors.h"
#include "s21_string.h"

s21_size s21_strlen(const char *str) {
  s21_size len = 0;
  if (str) {
    while (str[len] != '\0') {
      len++;
    }
  }
  return len;
}
// 1
void *s21_memchr(const void *str, int c, s21_size n) {
  const unsigned char *p = (unsigned char *)str;
  unsigned char uns_char = (unsigned char)c;
  void *result = S21_NULL;
  for (s21_size i = 0; i < n && result == S21_NULL; i++) {
    if (p[i] == uns_char) {
      result = (void *)(p + i);
    }
  }
  return result;
}
// 2
int s21_memcmp(const void *s1, const void *s2, s21_size n) {
  const unsigned char *p1 = s1;
  const unsigned char *p2 = s2;
  int result = 0;
  s21_size i = 0;

  while (i < n && result == 0) {
    if (p1[i] != p2[i]) {
      result = p1[i] - p2[i];
    }
    i++;
  }

  return result;
}
// 5
char *s21_strncat(char *dest, const char *src, s21_size n) {
  const char *un_src = src;
  char *un_dest = dest;
  while (*un_dest != '\0') {
    un_dest++;
  }
  s21_size j;
  int stop = 0;
  for (j = 0; j < n && !stop; j++) {
    if (un_src[j] != '\0') {
      un_dest[j] = un_src[j];
    } else {
      stop = 1;
    }
  }
  un_dest[j] = '\0';
  return dest;
}

// 6
char *s21_strchr(const char *str, int c) {
  char *result = S21_NULL;
  int found = 0;

  // Ищем символ в строке
  for (const char *s = str; *s != '\0' && !found; s++) {
    if (*s == c) {
      result = (char *)s;
      found = 1;
    }
  }

  // Проверяем нулевой терминатор
  if (!found && c == '\0') {
    result = (char *)str + s21_strlen(str);
  }

  return result;
}
// 9
s21_size s21_strcspn(const char *str1, const char *str2) {
  int should_break = 0;
  const char *un_str1 = str1;
  const char *un_str2 = str2;
  s21_size count = 0;
  while (un_str1[count] != '\0' && should_break == 0) {
    s21_size n = 0;
    int stop = 0;
    while (un_str2[n] != '\0' && !stop) {
      if (un_str1[count] == un_str2[n]) {
        should_break = 1;
        stop = 1;
      }
      n++;
    }
    if (should_break != 1) {
      count++;
    }
  }
  return count;
}

// 10
char *s21_strerror(int errnum) {
  static char buffer[50];
  char *result = S21_NULL;
  int is_known_error = 0;
  if (errnum >= 0 && errnum < S21_SYS_NERR) {  // Проверяем известные ошибки
    result = (char *)s21_sys_errlist[errnum];
    is_known_error = 1;
  }
  if (!is_known_error) {  // Если неизвестная ошибка - формируем сообщение
    char *p = buffer;
    const char *base_msg = "Unknown error ";  // Копируем базовое сообщение
    while (*base_msg) {
      *p++ = *base_msg++;
    }
    int num = errnum;  // Обрабатываем номер ошибки
    if (num < 0) {
      *p++ = '-';
      num = -num;
    }

    // Конвертируем число в строку
    char digits[20];  // Сохраняем цифры во временный буфер
    int digit_count = 0;
    while (num > 0) {
      digits[digit_count++] = '0' + (num % 10);
      num /= 10;
    }
    for (int i = digit_count - 1; i >= 0;
         i--) {  // Записываем цифры в правильном порядке
      *p++ = digits[i];
    }

    *p = '\0';
    result = buffer;
  }
  return result;
}
// 13
char *s21_strrchr(const char *str, int c) {
  int count = 0;
  char *adress = S21_NULL;
  while (str[count] != '\0') {
    if (str[count] == c) {
      adress = (char *)&str[count];
    }
    count++;
  }
  if ((char)c == '\0') {
    adress = (char *)str + count;
  }
  return adress;
}

// 14
char *s21_strstr(const char *haystack, const char *needle) {
  char *result = S21_NULL;
  s21_size needle_len = s21_strlen(needle);

  if (needle_len == 0) {
    result = (char *)haystack;
  } else {
    char first_char = *needle;
    for (const char *h = haystack; *h != '\0' && result == S21_NULL; h++) {
      result = (*h == first_char && s21_memcmp(h, needle, needle_len) == 0)
                   ? (char *)h
                   : S21_NULL;
    }
  }

  return result;
}

// venonata 15
char *s21_strtok(char *str, const char *delim) {
  static char *saved_token = S21_NULL;
  char *return_token = S21_NULL;

  if (str == S21_NULL) {
    str = saved_token;
  }
  // этот return это проверка входных параметров, можно его убрать ценой
  // мракобесия в виде заворачивания всего остального в if
  if (str == S21_NULL) {
    return S21_NULL;
  }

  // пропуск разделителей которые стоят вначале например ",,,,,123" -> "123"
  int stop = 0;
  while (!stop && *str != '\0') {
    if (s21_strchr(delim, *str) != S21_NULL) {
      str++;
    } else {
      stop = 1;
    }
  }

  // если добежали до конца материнской строки (до \0) - возвращаем S21_NULL
  // иначе действуем дальше
  if (*str != '\0') {
    return_token = str;
    // сдвиг адреса str на количество далее-идущих символов среди которых НЕ
    // встречаются разделители, либо вплоть до конца строки - \0
    str += s21_strcspn(str, delim);  // TODO заменить на s21 версию - done

    // если после сдвига мы не в конце строки - значит мы наткнулись на
    // разделитель, заменяем его на \0 завершая токен-подстроку
    if (*str != '\0') {
      *str = '\0';
      // запоминаем начало нового токена - это конец старого + 1 символ.
      saved_token = str + 1;
    } else {  // если после сдвига мы уткнулись в конец материнской строки -
              // дальше токенов не будет, предыдущий мы и так вернем, поэтому
              // сбрасываем память
      saved_token = S21_NULL;
    }
  } else {
    saved_token = S21_NULL;
  }

  return return_token;
}

// venonata 3
void *s21_memcpy(void *destination, const void *source, s21_size amount) {
  // сразу возвращаем dest при нулевом количестве
  if (amount != 0) {
    // приводим к типу указатели, также как делает оригинальная функция, теперь
    // можно инкрементить адрес (адресная арифметика)
    unsigned char *dest = (unsigned char *)destination;
    unsigned char *src = (unsigned char *)source;

    // в цикле копируем байты
    while (amount != 0) {
      *dest = *src;

      dest++;
      src++;
      amount--;
    }
  }
  return destination;
}

// venonata 7
int s21_strncmp(const char *str1, const char *str2, s21_size amount) {
  int res = 0;
  if (amount != 0) {
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;

    int stop = 0;
    while (amount-- > 0 && !stop) {
      if (*s1 != *s2) {
        stop = 1;
        res = (int)(*s1) - (int)(*s2);
        amount = 0;

      } else if (*s1 == '\0') {  // значит и во второй строке тоже нуль
        stop = 1;
        amount = 0;
      }

      s1++;
      s2++;
    }
  }
  return res;
}

// 4 - memset: Заполняет n байтов памяти указанным символом c. Возвращает
// указатель на блок памяти.
void *s21_memset(void *str, int c, s21_size n) {
  if (str != S21_NULL) {
    for (s21_size i = 0; i < n; i++) {
      ((char *)str)[i] = c;
    }
  }
  return str;
}

// 8 - strncpy: Копирует n символов строки из src в dest. Возврщает указатель на
// строку dest.
char *s21_strncpy(char *dest, const char *src, s21_size n) {
  int error_flag = 0;
  char *dest_pointer = dest;

  if (dest == S21_NULL || src == S21_NULL) {
    error_flag = 1;
    dest_pointer = S21_NULL;
  }

  if (!error_flag) {
    s21_size counter = 0;

    while (counter < n && src[counter] != '\0') {
      dest[counter] = src[counter];
      counter++;
    }

    while (counter < n) {
      dest[counter] = '\0';
      counter++;
    }
  }
  return dest_pointer;
}

// 12 - strpbrk: Выполняет поиск первого вхождения в str1 любого из символов
// str2. Возвращает указатель на первое вхождение в str1 найденного символа.

char *s21_strpbrk(const char *str1, const char *str2) {
  char *res_str = S21_NULL;

  if (str1 != S21_NULL || str2 != S21_NULL) {
    s21_size len_str = s21_strlen(str2);

    if (len_str > 0) {
      int match_found = 0;
      for (s21_size i = 0;
           str1[i] != '\0' && res_str == S21_NULL && !match_found; i++) {
        for (s21_size j = 0; j < len_str && !match_found; j++) {
          if (str1[i] == str2[j]) {
            res_str = (char *)&str1[i];
            match_found = 1;
          }
        }
      }
    }
  }

  return res_str;
}