#include "s21_sscanf.h"

// Пропуск пробельных символов
void skip_whitespace(const char **str) {
  if (str != S21_NULL && *str != S21_NULL) {
    while (**str == ' ' || **str == '\t' || **str == '\n' || **str == '\r' ||
           **str == '\v' || **str == '\f') {
      (*str)++;
    }
  }
}

// Проверка на цифру
int is_digit(char c) {
  int result = 0;
  if (c >= '0' && c <= '9') {
    result = 1;
  }
  return result;
}

// Преобразование символа в цифру
int char_to_digit(char c) {
  int result = 0;
  char ch = c;
  if (ch >= 'a' && ch <= 'f') ch -= 32;
  if (is_digit(ch)) {
    result = ch - '0';
  } else if (ch >= 'A' && ch <= 'F') {
    result = ch - 'A' + 10;
  }
  return result;
}

// Проверка на пробельный символ
int is_space(char c) {
  int result = 0;
  unsigned char ch = (unsigned char)c;
  // добавил каст, звезды (LLM) сказали нужно. нужно кастить чтобы не было UB
  // при чарах с кодом > 127
  if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\v' ||
      ch == '\f') {
    result = 1;
  }
  return result;
}

// подходит ли число, для системы счисления
int is_digit_for_base(char c, int base) {
  int result = 0;
  char ch = c;
  if (ch >= 'a' && ch <= 'f') ch -= 32;
  if (base == 10 && is_digit(ch))
    result = 1;
  else if (base == 8 && ch >= '0' && ch <= '7')
    result = 1;
  else if (base == 16 && (is_digit(ch) || (ch >= 'A' && ch <= 'F')))
    result = 1;

  return result;
}

// Является ли спецификатор дробным
int is_float_spec(char c) {
  return (c == 'f' || c == 'e' || c == 'E' || c == 'g' || c == 'G');
}