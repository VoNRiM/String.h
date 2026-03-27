#include "s21_string.h"

void *s21_to_upper(const char *str) {
  s21_size len = s21_strlen(str) + 1;
  char *str_upper = S21_NULL;
  if (str != S21_NULL) str_upper = malloc(len * sizeof(char));

  if (str != S21_NULL && str_upper != S21_NULL) {
    for (s21_size i = 0; i < len; i++) {
      if (str[i] > 96 && str[i] < 123)
        str_upper[i] = (str[i] - 32);
      else
        str_upper[i] = str[i];
    }
  }
  return str_upper;
}

void *s21_to_lower(const char *str) {
  s21_size len = s21_strlen(str) + 1;
  char *str_lower = S21_NULL;
  if (str != S21_NULL) str_lower = malloc(len * sizeof(char));

  if (str != S21_NULL && str_lower != S21_NULL) {
    for (s21_size i = 0; i < len; i++) {
      if (str[i] > 64 && str[i] < 91)
        str_lower[i] = (str[i] + 32);
      else
        str_lower[i] = str[i];
    }
  }
  return str_lower;
}

void *s21_insert(const char *str, const char *insert, s21_size start_index) {
  int stop = 0;
  s21_size len_str = s21_strlen(str);
  s21_size len_insert = s21_strlen(insert);
  s21_size len_new_str = len_str + len_insert + 1;

  char *new_str = S21_NULL;
  if (str != S21_NULL && insert != S21_NULL &&
      (start_index < len_str || start_index == len_str))
    new_str = malloc(len_new_str * sizeof(char));

  if (str == S21_NULL || insert == S21_NULL || start_index > len_str ||
      new_str == S21_NULL)
    stop = 1;

  for (s21_size i = 0; i < len_str + 1 && (!stop); i++) {
    s21_size index_in_new_str = i;
    if (i < start_index) {
      new_str[index_in_new_str] = str[i];
    } else if (i == start_index) {
      for (const char *ch = insert; *ch != '\0'; ch++) {
        new_str[index_in_new_str] = *ch;
        index_in_new_str++;
      }
      new_str[index_in_new_str] = str[i];
    } else {
      index_in_new_str += len_insert;
      new_str[index_in_new_str] = str[i];
    }
  }
  return new_str;
}

void *s21_trim(const char *str, const char *trim_chars) {
  int stop = 0;
  int flag_start = 1;
  int flag_end = 1;
  s21_size len_str = s21_strlen(str);
  s21_size len_trim_chars = s21_strlen(trim_chars);
  char *new_str = S21_NULL;

  if (str != S21_NULL && trim_chars != S21_NULL)
    new_str = malloc((len_str + 1) * sizeof(char));

  if (str == S21_NULL || trim_chars == S21_NULL || new_str == S21_NULL)
    stop = 1;

  s21_size index_in_new_str = 0;
  for (s21_size i = 0; i < len_str && (!stop); i++) {
    if (s21_strstr(str + i, trim_chars) == str + i && flag_start)
      i += len_trim_chars - 1;
    else {
      flag_start = 0;
      new_str[index_in_new_str] = str[i];
      index_in_new_str++;
    }
  }
  new_str[index_in_new_str] = '\0';

  s21_size len_new_str = s21_strlen(new_str);
  if (len_new_str < len_trim_chars) stop = 1;
  s21_size i = len_new_str - len_trim_chars;
  while (i > 0 && (!stop) && flag_end) {
    if (s21_strstr(new_str + i, trim_chars) == new_str + i && flag_end) {
      new_str[i] = '\0';
      i -= len_trim_chars;
    } else
      flag_end = 0;
  }

  return new_str;
}
