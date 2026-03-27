#include <check.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "format_functions.h"
#include "s21_sscanf.h"
#include "s21_string.h"

#ifndef S21_SYS_NERR
#define S21_SYS_NERR 150
#endif

// ==================== strlen тесты ====================
START_TEST(test_strlen_basic) {
  ck_assert_int_eq(s21_strlen(""), strlen(""));
  ck_assert_int_eq(s21_strlen("a"), strlen("a"));
  ck_assert_int_eq(s21_strlen("hello"), strlen("hello"));
  ck_assert_int_eq(s21_strlen("Hello, World!"), strlen("Hello, World!"));
}
END_TEST

START_TEST(test_strlen_edge_cases) {
  ck_assert_int_eq(s21_strlen(" "), strlen(" "));
  ck_assert_int_eq(s21_strlen("12345"), strlen("12345"));
  ck_assert_int_eq(s21_strlen("!@#$%"), strlen("!@#$%"));
}
END_TEST

// ==================== memcmp тесты ====================
START_TEST(test_memcmp_basic) {
  char str1[] = "Hello";
  char str2[] = "Hello";
  char str3[] = "Hellp";

  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
  ck_assert_int_eq(s21_memcmp(str1, str3, 5), memcmp(str1, str3, 5));
  ck_assert_int_eq(s21_memcmp(str1, str2, 3), memcmp(str1, str2, 3));
}
END_TEST

// ==================== strchr тесты ====================
START_TEST(test_strchr_basic) {
  char str[] = "Hello, World!";

  ck_assert_ptr_eq(s21_strchr(str, 'H'), strchr(str, 'H'));
  ck_assert_ptr_eq(s21_strchr(str, 'o'), strchr(str, 'o'));
  ck_assert_ptr_eq(s21_strchr(str, '!'), strchr(str, '!'));
  ck_assert_ptr_eq(s21_strchr(str, 'x'), strchr(str, 'x'));
}
END_TEST

// ==================== strerror тесты ====================
START_TEST(test_strerror_known_codes) {
  for (int i = 0; i < S21_SYS_NERR; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}
END_TEST

START_TEST(test_strerror_edge_cases) {
  ck_assert_str_eq(s21_strerror(-1), strerror(-1));
  ck_assert_str_eq(s21_strerror(999), strerror(999));
}
END_TEST

// ==================== strstr тесты ====================
START_TEST(test_strstr_basic) {
  char haystack[] = "Hello, World!";

  ck_assert_ptr_eq(s21_strstr(haystack, "Hello"), strstr(haystack, "Hello"));
  ck_assert_ptr_eq(s21_strstr(haystack, "World"), strstr(haystack, "World"));
  ck_assert_ptr_eq(s21_strstr(haystack, "abc"), strstr(haystack, "abc"));
}
END_TEST

// ==================== memcpy тесты ====================
START_TEST(test_memcpy_normal_values) {
  char src[] = "I love Anapa town!";
  char dest1[19] = {};
  char dest2[19] = {};

  ck_assert_mem_eq(s21_memcpy(dest1, src, 3), memcpy(dest2, src, 3), 3);
  ck_assert_mem_eq(s21_memcpy(dest1, src, 19), memcpy(dest2, src, 19), 19);
}
END_TEST

// ==================== strncmp тесты ====================
START_TEST(test_strncmp_normal_values) {
  ck_assert_int_eq(s21_strncmp("Hello", "Hello", 5),
                   strncmp("Hello", "Hello", 5));
  ck_assert_int_eq(s21_strncmp("Hello", "Hella", 5),
                   strncmp("Hello", "Hella", 5));
  ck_assert_int_eq(s21_strncmp("HelloX", "HelloY", 5),
                   strncmp("HelloX", "HelloY", 5));
}
END_TEST

// ==================== strtok тесты ====================
START_TEST(test_strtok_basic_tokens) {
  char str1[] = "I love,Anapa town";
  char str2[] = "I love,Anapa town";

  char* token_std = strtok(str1, " ,");
  char* token_s21 = s21_strtok(str2, " ,");

  while (token_std != NULL || token_s21 != NULL) {
    ck_assert_str_eq(token_s21, token_std);
    token_std = strtok(NULL, " ,");
    token_s21 = s21_strtok(NULL, " ,");
  }
}
END_TEST

// ==================== memset тесты ====================
START_TEST(test_memset_regular) {
  char str1[] = "Test_string";
  char str2[] = "Test_string";
  char c = '0';

  s21_memset((void*)str1, (int)c, 8);
  memset((void*)str2, (int)c, 8);
  ck_assert_str_eq(str1, str2);
}
END_TEST

// ==================== strncpy тесты ====================
START_TEST(test_strncpy_regular) {
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "aghfdaga";

  char* result = s21_strncpy(dest1, src, 5);
  char* expectation = strncpy(dest2, src, 5);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

// ==================== strpbrk тесты ====================
START_TEST(test_strpbrk_regular) {
  char dest[20] = "Test_string";
  char src[20] = "erg";

  char* result = s21_strpbrk(dest, src);
  char* expectation = strpbrk(dest, src);

  ck_assert_str_eq(result, expectation);
}
END_TEST

// ==================== Другие базовые функции ====================
START_TEST(test_memchr) {
  ck_assert_ptr_eq(memchr("Hello, world", 'e', 10),
                   s21_memchr("Hello, world", 'e', 10));
}
END_TEST

START_TEST(test_strcspn) {
  ck_assert_int_eq(strcspn("Hello", "kgrl"), s21_strcspn("Hello", "kgrl"));
  ck_assert_int_eq(strcspn("Hello123", "0987654321"),
                   s21_strcspn("Hello123", "0987654321"));
}
END_TEST

START_TEST(test_strncat) {
  char dest1[50] = "Hello world1";
  char dest2[50] = "Hello world1";

  ck_assert_str_eq(strncat(dest1, "I_LIKE_OLD_WOMAN", 10),
                   s21_strncat(dest2, "I_LIKE_OLD_WOMAN", 10));
}
END_TEST

START_TEST(test_strrchr) {
  ck_assert_ptr_eq(strrchr("Hello world", 'l'),
                   s21_strrchr("Hello world", 'l'));
  ck_assert_ptr_eq(strrchr("Hello world", 'h'),
                   s21_strrchr("Hello world", 'h'));
}
END_TEST

// ==================== Специальные функции обработки строк ====================
START_TEST(test_to_upper) {
  void* arr[] = {"mini_agz 1 2 5 9 0 11,?!: UP low MAX AGZ.", "INSERT insert",
                 "abcd efgh ijkl: mnoprst!", "\0", ""};

  void* arr_result[] = {"MINI_AGZ 1 2 5 9 0 11,?!: UP LOW MAX AGZ.",
                        "INSERT INSERT", "ABCD EFGH IJKL: MNOPRST!", "\0", ""};

  for (int i = 0; i < 5; i++) {
    char* str_check = s21_to_upper(arr[i]);
    ck_assert_str_eq(str_check, arr_result[i]);
    free(str_check);
  }
}
END_TEST

START_TEST(test_to_lower) {
  void* arr[] = {"mini_agz 1 2 5 9 0 11,?!: UP low MAX AGZ.", "INSERT insert",
                 "ABCD EFGH IJKL: MNOPRST!", "\0", ""};

  void* arr_result[] = {"mini_agz 1 2 5 9 0 11,?!: up low max agz.",
                        "insert insert", "abcd efgh ijkl: mnoprst!", "\0", ""};

  for (int i = 0; i < 5; i++) {
    char* str_check = s21_to_lower(arr[i]);
    ck_assert_str_eq(str_check, arr_result[i]);
    free(str_check);
  }
}
END_TEST

START_TEST(test_insert) {
  char* result = s21_insert("mini_agz 1 2 5 9 0 11?", "INSERT", 0);
  ck_assert_str_eq(result, "INSERTmini_agz 1 2 5 9 0 11?");
  free(result);

  result = s21_insert("abcd", "INSERT", 1);
  ck_assert_str_eq(result, "aINSERTbcd");
  free(result);

  result = s21_insert("mini_agz 1 2 5 9 0 11?", "INSERT", 22);
  ck_assert_str_eq(result, "mini_agz 1 2 5 9 0 11?INSERT");
  free(result);
}
END_TEST

START_TEST(test_trim) {
  char* result = s21_trim("123abc123", "123");
  ck_assert_str_eq(result, "abc");
  free(result);

  result = s21_trim("   ABabd98 C .&f( )   ", " ");
  ck_assert_str_eq(result, "ABabd98 C .&f( )");
  free(result);
}
END_TEST

// Тест для случая, когда c == '\0' в s21_strchr
START_TEST(test_strchr_null_terminator) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_strchr(str, '\0'), strchr(str, '\0'));
}
END_TEST

// Тест для s21_strstr с пустой строкой needle
START_TEST(test_strstr_empty_needle) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_strstr(str, ""), strstr(str, ""));
}
END_TEST

// Тест для s21_strtok с последовательными разделителями
START_TEST(test_strtok_consecutive_delimiters) {
  char str1[] = ",,,test";
  char str2[] = ",,,test";

  char* token_std = strtok(str1, ",");
  char* token_s21 = s21_strtok(str2, ",");

  ck_assert_str_eq(token_s21, token_std);

  token_std = strtok(NULL, ",");
  token_s21 = s21_strtok(NULL, ",");
  ck_assert_ptr_eq(token_s21, token_std);
}
END_TEST

// Тест для s21_strncpy с NULL аргументами
START_TEST(test_strncpy_null_args) {
  ck_assert_ptr_eq(s21_strncpy(NULL, "test", 5), NULL);
  ck_assert_ptr_eq(s21_strncpy("dest", NULL, 5), NULL);
}
END_TEST

// Тест для s21_strncmp с нулевой длиной
START_TEST(test_strncmp_zero_length) {
  ck_assert_int_eq(s21_strncmp("Hello", "World", 0),
                   strncmp("Hello", "World", 0));
}
END_TEST

// ==================== Part 2 - Обязательные спецификаторы ====================
START_TEST(test_sprintf_part2_specifiers) {
  char std_buf[256], s21_buf[256];

  // %c - символ
  sprintf(std_buf, "%c", 'A');
  s21_sprintf(s21_buf, "%c", 'A');
  ck_assert_str_eq(std_buf, s21_buf);

  // %d - десятичное целое
  sprintf(std_buf, "%d", 123);
  s21_sprintf(s21_buf, "%d", 123);
  ck_assert_str_eq(std_buf, s21_buf);

  // %f - число с плавающей точкой
  sprintf(std_buf, "%f", 3.1415);
  s21_sprintf(s21_buf, "%f", 3.1415);
  ck_assert_str_eq(std_buf, s21_buf);

  // %s - строка
  sprintf(std_buf, "%s", "test");
  s21_sprintf(s21_buf, "%s", "test");
  ck_assert_str_eq(std_buf, s21_buf);

  // %u - беззнаковое целое
  sprintf(std_buf, "%u", 123);
  s21_sprintf(s21_buf, "%u", 123);
  ck_assert_str_eq(std_buf, s21_buf);

  // %% - символ процента
  sprintf(std_buf, "%%");
  s21_sprintf(s21_buf, "%%");
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== Part 2 - Флаги ====================
START_TEST(test_sprintf_part2_flags) {
  char std_buf[256], s21_buf[256];

  // - (выравнивание влево)
  sprintf(std_buf, "%-10d", 123);
  s21_sprintf(s21_buf, "%-10d", 123);
  ck_assert_str_eq(std_buf, s21_buf);

  // + (обязательный знак)
  sprintf(std_buf, "%+d", 123);
  s21_sprintf(s21_buf, "%+d", 123);
  ck_assert_str_eq(std_buf, s21_buf);

  // (пробел) (пробел перед положительными)
  sprintf(std_buf, "% d", 123);
  s21_sprintf(s21_buf, "% d", 123);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== Part 2 - Ширина и точность ====================
START_TEST(test_sprintf_part2_width_precision) {
  char std_buf[256], s21_buf[256];

  // Ширина
  sprintf(std_buf, "%5d", 123);
  s21_sprintf(s21_buf, "%5d", 123);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%10s", "test");
  s21_sprintf(s21_buf, "%10s", "test");
  ck_assert_str_eq(std_buf, s21_buf);

  // Точность
  sprintf(std_buf, "%.5d", 42);
  s21_sprintf(s21_buf, "%.5d", 42);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%.2f", 3.14159);
  s21_sprintf(s21_buf, "%.2f", 3.14159);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%.3s", "hello");
  s21_sprintf(s21_buf, "%.3s", "hello");
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== Part 2 - Модификаторы длины ====================
START_TEST(test_sprintf_part2_length) {
  char std_buf[256], s21_buf[256];

  // h для short
  short sh = 123;
  sprintf(std_buf, "%hd", sh);
  s21_sprintf(s21_buf, "%hd", sh);
  ck_assert_str_eq(std_buf, s21_buf);

  // l для long
  long lg = 123456L;
  sprintf(std_buf, "%ld", lg);
  s21_sprintf(s21_buf, "%ld", lg);
  ck_assert_str_eq(std_buf, s21_buf);

  // l для unsigned long
  unsigned long ulg = 123456UL;
  sprintf(std_buf, "%lu", ulg);
  s21_sprintf(s21_buf, "%lu", ulg);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== Part 3 - Бонусные спецификаторы ====================
START_TEST(test_sprintf_part3_specifiers) {
  char std_buf[256], s21_buf[256];

  // %x/X - шестнадцатеричное
  sprintf(std_buf, "%x", 255);
  s21_sprintf(s21_buf, "%x", 255);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%X", 255);
  s21_sprintf(s21_buf, "%X", 255);
  ck_assert_str_eq(std_buf, s21_buf);

  // %o - восьмеричное
  sprintf(std_buf, "%o", 63);
  s21_sprintf(s21_buf, "%o", 63);
  ck_assert_str_eq(std_buf, s21_buf);

  // %e/E - научная нотация
  sprintf(std_buf, "%e", 123.456);
  s21_sprintf(s21_buf, "%e", 123.456);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%E", 123.456);
  s21_sprintf(s21_buf, "%E", 123.456);
  ck_assert_str_eq(std_buf, s21_buf);

  // %g/G - кратчайшее представление
  sprintf(std_buf, "%g", 123.456);
  s21_sprintf(s21_buf, "%g", 123.456);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== Part 3 - Бонусные флаги ====================
START_TEST(test_sprintf_part3_flags) {
  char std_buf[256], s21_buf[256];

  // # для шестнадцатеричных
  sprintf(std_buf, "%#x", 255);
  s21_sprintf(s21_buf, "%#x", 255);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%#X", 255);
  s21_sprintf(s21_buf, "%#X", 255);
  ck_assert_str_eq(std_buf, s21_buf);

  // # для восьмеричных
  sprintf(std_buf, "%#o", 63);
  s21_sprintf(s21_buf, "%#o", 63);
  ck_assert_str_eq(std_buf, s21_buf);

  // 0 - заполнение нулями
  sprintf(std_buf, "%05d", 42);
  s21_sprintf(s21_buf, "%05d", 42);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== Комбинации флагов ====================
START_TEST(test_sprintf_combinations) {
  char std_buf[256], s21_buf[256];

  // Флаг + с шириной и точностью
  sprintf(std_buf, "%+10.5d", 123);
  s21_sprintf(s21_buf, "%+10.5d", 123);
  ck_assert_str_eq(std_buf, s21_buf);

  // Флаг - с шириной
  sprintf(std_buf, "%-10s", "test");
  s21_sprintf(s21_buf, "%-10s", "test");
  ck_assert_str_eq(std_buf, s21_buf);

  // Флаг # с шириной
  sprintf(std_buf, "%#10x", 255);
  s21_sprintf(s21_buf, "%#10x", 255);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== Граничные случаи ====================
START_TEST(test_sprintf_edge_cases) {
  char std_buf[256], s21_buf[256];

  // Нулевые значения
  sprintf(std_buf, "%d", 0);
  s21_sprintf(s21_buf, "%d", 0);
  ck_assert_str_eq(std_buf, s21_buf);

  // Отрицательные значения
  sprintf(std_buf, "%d", -123);
  s21_sprintf(s21_buf, "%d", -123);
  ck_assert_str_eq(std_buf, s21_buf);

  // Максимальные значения
  sprintf(std_buf, "%d", INT_MAX);
  s21_sprintf(s21_buf, "%d", INT_MAX);
  ck_assert_str_eq(std_buf, s21_buf);

  // Пустая строка
  sprintf(std_buf, "%s", "");
  s21_sprintf(s21_buf, "%s", "");
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// Дополнительные тесты для sprintf
START_TEST(test_sprintf_hash_flag) {
  char std_buf[256], s21_buf[256];

  // # с float
  sprintf(std_buf, "%#.0f", 123.0);
  s21_sprintf(s21_buf, "%#.0f", 123.0);
  ck_assert_str_eq(std_buf, s21_buf);

  // # с g/G
  sprintf(std_buf, "%#g", 123.456);
  s21_sprintf(s21_buf, "%#g", 123.456);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

START_TEST(test_sprintf_zero_flag) {
  char std_buf[256], s21_buf[256];

  // 0 с разной шириной
  sprintf(std_buf, "%010d", 123);
  s21_sprintf(s21_buf, "%010d", 123);
  ck_assert_str_eq(std_buf, s21_buf);

  // 0 с отрицательными числами
  sprintf(std_buf, "%010d", -123);
  s21_sprintf(s21_buf, "%010d", -123);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

START_TEST(test_sprintf_long_double) {
  char std_buf[256], s21_buf[256];
  long double ld = 123.456789L;

  // L модификатор
  sprintf(std_buf, "%.10Lf", ld);
  s21_sprintf(s21_buf, "%.10Lf", ld);
  ck_assert_str_eq(std_buf, s21_buf);

  // L с научной нотацией
  sprintf(std_buf, "%.5Le", ld);
  s21_sprintf(s21_buf, "%.5Le", ld);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

START_TEST(test_sprintf_inf_nan) {
  char std_buf[256], s21_buf[256];
  double inf = 1.0 / 0.0;
  double nan = 0.0 / 0.0;
  // inf
  sprintf(std_buf, "%f", inf);
  s21_sprintf(s21_buf, "%f", inf);
  // Проверка на наличие "inf" в строке
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%e", inf);
  s21_sprintf(s21_buf, "%e", inf);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%E", inf);
  s21_sprintf(s21_buf, "%E", inf);
  ck_assert_str_eq(std_buf, s21_buf);
  // -inf
  sprintf(std_buf, "%f", -inf);
  s21_sprintf(s21_buf, "%f", -inf);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%g", -inf);
  s21_sprintf(s21_buf, "%g", -inf);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%G", -inf);
  s21_sprintf(s21_buf, "%G", -inf);
  ck_assert_str_eq(std_buf, s21_buf);

  // nan
  sprintf(std_buf, "%f", nan);
  s21_sprintf(s21_buf, "%f", nan);
  // Проверка на наличие "nan" в строке
  ck_assert(strstr(std_buf, "nan") != NULL);
  ck_assert(strstr(s21_buf, "nan") != NULL);

  sprintf(std_buf, "%E", nan);
  s21_sprintf(s21_buf, "%E", nan);
  // Проверка на наличие "nan" в строке
  ck_assert(strstr(std_buf, "NAN") != NULL);
  ck_assert(strstr(s21_buf, "NAN") != NULL);

  sprintf(std_buf, "%e", nan);
  s21_sprintf(s21_buf, "%e", nan);
  // Проверка на наличие "nan" в строке
  ck_assert(strstr(std_buf, "nan") != NULL);
  ck_assert(strstr(s21_buf, "nan") != NULL);

  sprintf(std_buf, "%G", nan);
  s21_sprintf(s21_buf, "%G", nan);
  // Проверка на наличие "nan" в строке
  ck_assert(strstr(std_buf, "NAN") != NULL);
  ck_assert(strstr(s21_buf, "NAN") != NULL);

  sprintf(std_buf, "%g", nan);
  s21_sprintf(s21_buf, "%g", nan);
  // Проверка на наличие "nan" в строке
  ck_assert(strstr(std_buf, "nan") != NULL);
  ck_assert(strstr(s21_buf, "nan") != NULL);
}
END_TEST

START_TEST(test_sprintf_pointer) {
  char std_buf[256], s21_buf[256];
  int x = 42;
  void* ptr = &x;

  sprintf(std_buf, "%p", ptr);
  s21_sprintf(s21_buf, "%p", ptr);
  ck_assert_str_eq(std_buf, s21_buf);

  // NULL указатель
  sprintf(std_buf, "%p", NULL);
  s21_sprintf(s21_buf, "%p", NULL);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

START_TEST(test_sprintf_gG_edge_cases) {
  char std_buf[256], s21_buf[256];

  // g с очень маленькими числами
  sprintf(std_buf, "%g", 0.0000123);
  s21_sprintf(s21_buf, "%g", 0.0000123);
  ck_assert_str_eq(std_buf, s21_buf);

  // g с очень большими числами
  sprintf(std_buf, "%g", 123456789.0);
  s21_sprintf(s21_buf, "%g", 123456789.0);
  ck_assert_str_eq(std_buf, s21_buf);

  // G в верхнем регистре
  sprintf(std_buf, "%G", 123.456);
  s21_sprintf(s21_buf, "%G", 123.456);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

START_TEST(test_sprintf_width_star) {
  char std_buf[256], s21_buf[256];
  int width = 10;

  sprintf(std_buf, "%*d", width, 123);
  s21_sprintf(s21_buf, "%*d", width, 123);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%-*s", width, "test");
  s21_sprintf(s21_buf, "%-*s", width, "test");
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

START_TEST(test_sprintf_precision_star) {
  char std_buf[256], s21_buf[256];
  int prec = 3;

  sprintf(std_buf, "%.*f", prec, 3.14159);
  s21_sprintf(s21_buf, "%.*f", prec, 3.14159);
  ck_assert_str_eq(std_buf, s21_buf);

  sprintf(std_buf, "%.*s", prec, "hello");
  s21_sprintf(s21_buf, "%.*s", prec, "hello");
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== sscanf ====================
START_TEST(test_sscanf_basic_integers) {
  const char src[] = "42 99";
  const char format[] = "%d %u";

  int a = 0, s21_a = 0;
  unsigned b = 0, s21_b = 0;

  int r1 = sscanf(src, format, &a, &b);
  int r2 = s21_sscanf(src, format, &s21_a, &s21_b);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a, s21_a);
  ck_assert_uint_eq(b, s21_b);
}
END_TEST

START_TEST(test_sscanf_basic_strings_and_chars) {
  const char src[] = "Hello Z";
  const char format[] = "%s %c";

  char s1[32] = {0}, s21_s1[32] = {0};
  char ch = 0, s21_ch = 0;

  int r1 = sscanf(src, format, s1, &ch);
  int r2 = s21_sscanf(src, format, s21_s1, &s21_ch);

  ck_assert_int_eq(r1, r2);
  ck_assert_str_eq(s1, s21_s1);
  ck_assert_int_eq((unsigned char)ch, (unsigned char)s21_ch);
}
END_TEST

START_TEST(test_sscanf_basic_float) {
  const char src[] = "3.25";
  const char format[] = "%f";

  float f = 0.0f, s21_f = 0.0f;
  int r1 = sscanf(src, format, &f);
  int r2 = s21_sscanf(src, format, &s21_f);

  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq_tol(f, s21_f, 1e-6f);
}
END_TEST

START_TEST(test_sscanf_basic_hex) {
  const char src[] = "ff";
  const char format[] = "%x";

  unsigned x = 0, s21_x = 0;
  int r1 = sscanf(src, format, &x);
  int r2 = s21_sscanf(src, format, &s21_x);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(x, s21_x);
}
END_TEST

// ==================== Строковые литералы ====================
START_TEST(test_sscanf_literals) {
  const char src[] = "id=7";
  const char format[] = "id=%d";

  int id = 0, s21_id = 0;
  int r1 = sscanf(src, format, &id);
  int r2 = s21_sscanf(src, format, &s21_id);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(id, s21_id);
}
END_TEST

// ==================== Подсчет символов %n ====================
START_TEST(test_sscanf_scan_counter) {
  const char src[] = "abc 12";
  const char format[] = "%3s %d %n";

  char s1[8] = {0}, s21_s1[8] = {0};
  int v = 0, s21_v = 0;
  int n = -1, s21_n = -1;

  int r1 = sscanf(src, format, s1, &v, &n);
  int r2 = s21_sscanf(src, format, s21_s1, &s21_v, &s21_n);

  ck_assert_int_eq(r1, r2);
  ck_assert_str_eq(s1, s21_s1);
  ck_assert_int_eq(v, s21_v);
  ck_assert_int_eq(n, s21_n);
}
END_TEST

// ==================== Граничные случаи ====================
START_TEST(test_sscanf_edge_empty_input) {
  const char src[] = "";
  const char format[] = "%d";

  int a = 777, s21_a = 777;

  int r1 = sscanf(src, format, &a);
  int r2 = s21_sscanf(src, format, &s21_a);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a, s21_a);
}
END_TEST

START_TEST(test_sscanf_edge_matching_failure) {
  const char src[] = "abc";
  const char format[] = "%d";

  int a = 555, s21_a = 555;

  int r1 = sscanf(src, format, &a);
  int r2 = s21_sscanf(src, format, &s21_a);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a, s21_a);
}
END_TEST

START_TEST(test_sscanf_edge_char_whitespace) {
  const char src[] = "   A";
  const char format[] = "%c";

  char ch = 0, s21_ch = 0;

  int r1 = sscanf(src, format, &ch);
  int r2 = s21_sscanf(src, format, &s21_ch);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((unsigned char)ch, (unsigned char)s21_ch);
}
END_TEST

// ==================== Модификаторы длины ====================
START_TEST(test_sscanf_length_modifiers) {
  const char src[] = "12345";
  const char format_short[] = "%hd";
  const char format_long[] = "%ld";

  // short
  short sh1 = 0, sh2 = 0;
  int r1 = sscanf(src, format_short, &sh1);
  int r2 = s21_sscanf(src, format_short, &sh2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(sh1, sh2);

  // long
  long lg1 = 0, lg2 = 0;
  r1 = sscanf(src, format_long, &lg1);
  r2 = s21_sscanf(src, format_long, &lg2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(lg1, lg2);
}
END_TEST

// ==================== Ширина чтения ====================
START_TEST(test_sscanf_width) {
  const char src[] = "12345";
  const char format[] = "%2d";

  int a1 = 0, a2 = 0;

  int r1 = sscanf(src, format, &a1);
  int r2 = s21_sscanf(src, format, &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

// ==================== Указатели %p ====================
START_TEST(test_sscanf_pointer) {
  int x = 1337;
  char src[128] = {0};
  snprintf(src, sizeof(src), "ptr=%p", (void*)&x);

  const char format[] = "ptr=%p";

  void* p_std = NULL;
  void* p_s21 = NULL;

  int r1 = sscanf(src, format, &p_std);
  int r2 = s21_sscanf(src, format, &p_s21);

  ck_assert_int_eq(r1, r2);
  ck_assert_ptr_eq(p_std, p_s21);
}
END_TEST

// ==================== Символ процента ====================
START_TEST(test_sscanf_percent) {
  const char src[] = "100%";
  const char format[] = "%d%%";

  int a1 = -1, a2 = -1;

  int r1 = sscanf(src, format, &a1);
  int r2 = s21_sscanf(src, format, &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

// ==================== Числа с плавающей точкой ====================
START_TEST(test_sscanf_float_variants) {
  const char src1[] = "12.345";
  const char src2[] = "1.2345e+03";
  const char src3[] = "1.25e-5";

  float f1 = 0, f2 = 0;
  double d1 = 0, d2 = 0;
  long double ld1 = 0, ld2 = 0;

  // float
  int r1 = sscanf(src1, "%f", &f1);
  int r2 = s21_sscanf(src1, "%f", &f2);
  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq_tol(f1, f2, 1e-6);

  // double научная нотация
  r1 = sscanf(src2, "%lf", &d1);
  r2 = s21_sscanf(src2, "%lf", &d2);
  ck_assert_int_eq(r1, r2);
  ck_assert_double_eq_tol(d1, d2, 1e-6);

  // long double
  r1 = sscanf(src3, "%Lf", &ld1);
  r2 = s21_sscanf(src3, "%Lf", &ld2);
  ck_assert_int_eq(r1, r2);
  ck_assert_ldouble_eq_tol(ld1, ld2, 1e-12L);
}
END_TEST

// ==================== Отрицательные числа ====================
START_TEST(test_sscanf_negative_numbers) {
  const char src[] = "-123 -45.67";
  const char format[] = "%d %f";

  int i1 = 0, i2 = 0;
  float f1 = 0, f2 = 0;

  int r1 = sscanf(src, format, &i1, &f1);
  int r2 = s21_sscanf(src, format, &i2, &f2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(i1, i2);
  ck_assert_float_eq_tol(f1, f2, 1e-6);
}
END_TEST

// ==================== Комплексные форматы ====================
START_TEST(test_sscanf_complex_formats) {
  const char src[] = "Name: John, Age: 25, Score: 85.5";
  const char format[] = "Name: %s, Age: %d, Score: %f";

  char name1[32] = {0}, name2[32] = {0};
  int age1 = 0, age2 = 0;
  float score1 = 0, score2 = 0;

  int r1 = sscanf(src, format, name1, &age1, &score1);
  int r2 = s21_sscanf(src, format, name2, &age2, &score2);

  ck_assert_int_eq(r1, r2);
  ck_assert_str_eq(name1, name2);
  ck_assert_int_eq(age1, age2);
  ck_assert_float_eq_tol(score1, score2, 1e-6);
}
END_TEST

// Тесты для sscanf с разными системами счисления
START_TEST(test_sscanf_octal) {
  const char src[] = "755 0777";
  const char format[] = "%o %o";

  unsigned o1 = 0, o2 = 0;
  unsigned s21_o1 = 0, s21_o2 = 0;

  int r1 = sscanf(src, format, &o1, &o2);
  int r2 = s21_sscanf(src, format, &s21_o1, &s21_o2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(o1, s21_o1);
  ck_assert_uint_eq(o2, s21_o2);
}
END_TEST

START_TEST(test_sscanf_hex_variants) {
  // Тестируем по одному числу для отладки
  const char src1[] = "0xff";
  const char src2[] = "0XFF";
  const char src3[] = "ff";
  const char src4[] = "FF";
  const char format[] = "%x";

  unsigned x1 = 0, x2 = 0, x3 = 0, x4 = 0;
  unsigned s21_x1 = 0, s21_x2 = 0, s21_x3 = 0, s21_x4 = 0;

  // Тест 1: 0xff
  int r1 = sscanf(src1, format, &x1);
  int r2 = s21_sscanf(src1, format, &s21_x1);
  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(x1, s21_x1);

  // Тест 2: 0XFF
  r1 = sscanf(src2, format, &x2);
  r2 = s21_sscanf(src2, format, &s21_x2);
  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(x2, s21_x2);

  // Тест 3: ff (без префикса)
  r1 = sscanf(src3, format, &x3);
  r2 = s21_sscanf(src3, format, &s21_x3);
  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(x3, s21_x3);

  // Тест 4: FF (заглавные, без префикса)
  r1 = sscanf(src4, format, &x4);
  r2 = s21_sscanf(src4, format, &s21_x4);
  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(x4, s21_x4);
}
END_TEST

START_TEST(test_sscanf_float_scientific) {
  const char src[] = "1.23e4 1.23E-4 -5.67e+10";
  const char format[] = "%e %e %e";

  float f1 = 0, f2 = 0, f3 = 0;
  float s21_f1 = 0, s21_f2 = 0, s21_f3 = 0;

  int r1 = sscanf(src, format, &f1, &f2, &f3);
  int r2 = s21_sscanf(src, format, &s21_f1, &s21_f2, &s21_f3);

  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq_tol(f1, s21_f1, 1e-6);
  ck_assert_float_eq_tol(f2, s21_f2, 1e-6);
  ck_assert_float_eq_tol(f3, s21_f3, 1e-6);
}
END_TEST

START_TEST(test_sscanf_pointer_variants) {
  const char src1[] = "(nil)";
  const char src2[] = "0x0";
  const char src3[] = "0x7ffd12345678";

  void *p1 = NULL, *p2 = NULL, *p3 = NULL;
  void *s21_p1 = NULL, *s21_p2 = NULL, *s21_p3 = NULL;

  // (nil)
  int r1 = sscanf(src1, "%p", &p1);
  int r2 = s21_sscanf(src1, "%p", &s21_p1);
  ck_assert_int_eq(r1, r2);
  ck_assert_ptr_eq(p1, s21_p1);

  // 0x0
  r1 = sscanf(src2, "%p", &p2);
  r2 = s21_sscanf(src2, "%p", &s21_p2);
  ck_assert_int_eq(r1, r2);
  ck_assert_ptr_eq(p2, s21_p2);

  // обычный адрес
  r1 = sscanf(src3, "%p", &p3);
  r2 = s21_sscanf(src3, "%p", &s21_p3);
  ck_assert_int_eq(r1, r2);
  ck_assert_ptr_eq(p3, s21_p3);
}
END_TEST

START_TEST(test_sscanf_width_limitation) {
  const char src[] = "1234567890";
  const char format[] = "%5d";

  int a1 = 0, a2 = 0;

  int r1 = sscanf(src, format, &a1);
  int r2 = s21_sscanf(src, format, &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_suppress_assignment) {
  const char src[] = "123 abc";
  const char format[] = "%*d %s";

  char s1[32] = {0}, s21_s1[32] = {0};

  int r1 = sscanf(src, format, s1);
  int r2 = s21_sscanf(src, format, s21_s1);

  ck_assert_int_eq(r1, r2);
  ck_assert_str_eq(s1, s21_s1);
}
END_TEST

// Тест для звездочки в формате (подавление)
START_TEST(test_sscanf_star_format) {
  const char src[] = "123 abc 456";
  const char format[] = "%*d %s %d";

  char str[32] = {0};
  int num = 0;
  char s21_str[32] = {0};
  int s21_num = 0;

  int r1 = sscanf(src, format, str, &num);
  int r2 = s21_sscanf(src, format, s21_str, &s21_num);

  ck_assert_int_eq(r1, r2);
  ck_assert_str_eq(str, s21_str);
  ck_assert_int_eq(num, s21_num);
}
END_TEST

// Тест для ширины с *
START_TEST(test_sscanf_width_star) {
  const char src[] = "12345";
  const char format[] = "%*2d %d";

  int num = 0, s21_num = 0;

  int r1 = sscanf(src, format, &num);
  int r2 = s21_sscanf(src, format, &s21_num);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(num, s21_num);
}
END_TEST

// Тест для отрицательной ширины
START_TEST(test_sscanf_negative_width) {
  const char src[] = "123";
  const char format[] = "%d";

  int num = 0, s21_num = 0;

  int r1 = sscanf(src, format, &num);
  int r2 = s21_sscanf(src, format, &s21_num);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(num, s21_num);
}
END_TEST

// Тест для %n с разными позициями
START_TEST(test_sscanf_n_position) {
  const char src[] = "123 abc";
  const char format1[] = "%d %n %s";
  const char format2[] = "%d %s %n";

  int n1 = 0, n2 = 0, num = 0;
  char str[32] = {0};
  int s21_n1 = 0, s21_n2 = 0, s21_num = 0;
  char s21_str[32] = {0};

  // %n в середине
  int r1 = sscanf(src, format1, &num, &n1, str);
  int r2 = s21_sscanf(src, format1, &s21_num, &s21_n1, s21_str);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(n1, s21_n1);

  // %n в конце
  r1 = sscanf(src, format2, &num, str, &n2);
  r2 = s21_sscanf(src, format2, &s21_num, s21_str, &s21_n2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(n2, s21_n2);
}
END_TEST

// Тест для чисел с разными системами счисления
START_TEST(test_sscanf_mixed_bases) {
  const char src[] = "0x10 010 10";
  const char format[] = "%i %i %i";

  int a = 0, b = 0, c = 0;
  int s21_a = 0, s21_b = 0, s21_c = 0;

  int r1 = sscanf(src, format, &a, &b, &c);
  int r2 = s21_sscanf(src, format, &s21_a, &s21_b, &s21_c);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a, s21_a);  // 16
  ck_assert_int_eq(b, s21_b);  // 8
  ck_assert_int_eq(c, s21_c);  // 10
}
END_TEST

// Тест для очень больших/маленьких чисел
START_TEST(test_sscanf_extreme_numbers) {
  const char src1[] = "2147483647";
  const char src2[] = "-2147483648";
  const char src3[] = "9999999999";
  const char format[] = "%d";

  int num = 0, s21_num = 0;

  int r1 = sscanf(src1, format, &num);
  int r2 = s21_sscanf(src1, format, &s21_num);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(num, s21_num);

  r1 = sscanf(src2, format, &num);
  r2 = s21_sscanf(src2, format, &s21_num);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(num, s21_num);

  r1 = sscanf(src3, format, &num);
  r2 = s21_sscanf(src3, format, &s21_num);
  ck_assert_int_eq(r1, r2);
}
END_TEST

// Тест для float с разными форматами
START_TEST(test_sscanf_float_formats) {
  const char src[] = "123.456 .789 123. 0.001";
  const char format[] = "%f %f %f %f";

  float f1 = 0, f2 = 0, f3 = 0, f4 = 0;
  float s21_f1 = 0, s21_f2 = 0, s21_f3 = 0, s21_f4 = 0;

  int r1 = sscanf(src, format, &f1, &f2, &f3, &f4);
  int r2 = s21_sscanf(src, format, &s21_f1, &s21_f2, &s21_f3, &s21_f4);

  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq_tol(f1, s21_f1, 1e-6);
  ck_assert_float_eq_tol(f2, s21_f2, 1e-6);
  ck_assert_float_eq_tol(f3, s21_f3, 1e-6);
  ck_assert_float_eq_tol(f4, s21_f4, 1e-6);
}
END_TEST

// Тест для %s с модификатором 'l' (wide char)
START_TEST(test_sscanf_string_wide_char) {
  const char src[] = "Hello";
  const char format[] = "%ls";
  wchar_t ws[10] = {0};

  int r1 = sscanf(src, format, ws);
  int r2 = s21_sscanf(src, format, ws);

  ck_assert_int_eq(r1, r2);
}
END_TEST

// Тест для handle_unsigned_generic (функция вызывается через макросы)
START_TEST(test_sscanf_unsigned_generic_path) {
  const char src[] = "123 456 789";
  const char format[] = "%u %o %x";

  unsigned u = 0, o = 0, x = 0;
  unsigned s21_u = 0, s21_o = 0, s21_x = 0;

  int r1 = sscanf(src, format, &u, &o, &x);
  int r2 = s21_sscanf(src, format, &s21_u, &s21_o, &s21_x);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(u, s21_u);
  ck_assert_uint_eq(o, s21_o);
  ck_assert_uint_eq(x, s21_x);
}
END_TEST

// Тест для записи значений через модификаторы с разными типами
START_TEST(test_sscanf_value_modifiers_different_sizes) {
  const char src1[] = "32767";  // SHRT_MAX
  const char src2[] = "32768";  // SHRT_MAX + 1

  // Тест для short (SHRT_MAX)
  short s1 = 0, s21_s1 = 0;
  sscanf(src1, "%hd", &s1);
  s21_sscanf(src1, "%hd", &s21_s1);
  ck_assert_int_eq(s1, s21_s1);

  // Тест для unsigned short
  unsigned short us = 0, s21_us = 0;
  sscanf(src2, "%hu", &us);
  s21_sscanf(src2, "%hu", &s21_us);
  ck_assert_uint_eq(us, s21_us);
}
END_TEST

// Также добавьте этот тест для проверки чтения нулевого числа
START_TEST(test_sscanf_zero_number) {
  const char src[] = "0";
  const char format[] = "%d %u %o %x";

  int d = -1;
  unsigned u = -1;
  unsigned o = -1;
  unsigned x = -1;
  int s21_d = -1;
  unsigned s21_u = -1;
  unsigned s21_o = -1;
  unsigned s21_x = -1;

  int r1 = sscanf(src, format, &d, &u, &o, &x);
  int r2 = s21_sscanf(src, format, &s21_d, &s21_u, &s21_o, &s21_x);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(d, s21_d);
  ck_assert_uint_eq(u, s21_u);
  ck_assert_uint_eq(o, s21_o);
  ck_assert_uint_eq(x, s21_x);
}
END_TEST

// Тест для несовпадающих литералов
START_TEST(test_sscanf_literal_mismatch) {
  const char src[] = "abc";
  const char format[] = "xyz";  // Литералы не совпадают

  int r1 = sscanf(src, format);
  int r2 = s21_sscanf(src, format);

  ck_assert_int_eq(r1, r2);
}
END_TEST

// Тест для чисел с знаками для беззнаковых типов
START_TEST(test_sscanf_unsigned_with_sign) {
  const char src[] = "-123 +456";
  const char format[] = "%u %u";

  unsigned u1 = 0, u2 = 0;
  unsigned s21_u1 = 0, s21_u2 = 0;

  int r1 = sscanf(src, format, &u1, &u2);
  int r2 = s21_sscanf(src, format, &s21_u1, &s21_u2);

  ck_assert_int_eq(r1, r2);
  // Ожидаемое поведение: знак игнорируется для %u
}
END_TEST

START_TEST(test_sscanf_float_spec_check) {
  // Этот тест косвенно проверяет is_float_spec через обработку float
  const char src[] = "3.14 2.718 1.618";
  const char format[] = "%f %e %g";

  float f1 = 0, f2 = 0, f3 = 0;
  float s21_f1 = 0, s21_f2 = 0, s21_f3 = 0;

  int r1 = sscanf(src, format, &f1, &f2, &f3);
  int r2 = s21_sscanf(src, format, &s21_f1, &s21_f2, &s21_f3);

  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq_tol(f1, s21_f1, 1e-6);
  ck_assert_float_eq_tol(f2, s21_f2, 1e-6);
  ck_assert_float_eq_tol(f3, s21_f3, 1e-6);
}
END_TEST

// Тест для чисел с префиксом 0x в восьмеричной системе
START_TEST(test_sscanf_octal_with_prefix) {
  const char src[] = "0x123";  // Префикс 0x при чтении восьмеричного
  const char format[] = "%o";

  unsigned o1 = 0, o2 = 0;

  int r1 = sscanf(src, format, &o1);
  int r2 = s21_sscanf(src, format, &o2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(o1, o2);
}
END_TEST

// Тест для чтения экспоненты с ошибкой
START_TEST(test_sscanf_exponent_error) {
  const char src[] = "1.23e";  // Экспонента без числа
  const char format[] = "%f";

  float f1 = 0, f2 = 0;

  int r1 = sscanf(src, format, &f1);
  int r2 = s21_sscanf(src, format, &f2);

  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq_tol(f1, f2, 1e-6);
}
END_TEST

// Тест для s21_insert с неверным индексом
START_TEST(test_insert_invalid_index) {
  ck_assert_ptr_eq(s21_insert("test", "insert", 10), NULL);
}
END_TEST

START_TEST(test_sprintf_short_unsigned) {
  char std_buf[256], s21_buf[256];

  unsigned short us = 65535;

  sprintf(std_buf, "%hu", us);
  s21_sprintf(s21_buf, "%hu", us);
  ck_assert_str_eq(std_buf, s21_buf);

  us = 0;
  sprintf(std_buf, "%hu", us);
  s21_sprintf(s21_buf, "%hu", us);
  ck_assert_str_eq(std_buf, s21_buf);

  us = 42;
  sprintf(std_buf, "%.5hu", us);
  s21_sprintf(s21_buf, "%.5hu", us);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

START_TEST(test_sprintf_num_to_char_bases) {
  char std_buf[256], s21_buf[256];

  int val = -255;

  // base 10, отрицательное
  sprintf(std_buf, "%d", val);
  s21_sprintf(s21_buf, "%d", val);
  ck_assert_str_eq(std_buf, s21_buf);

  // base 16
  sprintf(std_buf, "%x", val);
  s21_sprintf(s21_buf, "%x", val);
  ck_assert_str_eq(std_buf, s21_buf);

  // base 8
  sprintf(std_buf, "%o", val);
  s21_sprintf(s21_buf, "%o", val);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

START_TEST(test_sprintf_num_to_char_zero_and_min) {
  char std_buf[256], s21_buf[256];

  // value  0
  sprintf(std_buf, "%x", 0);
  s21_sprintf(s21_buf, "%x", 0);
  ck_assert_str_eq(std_buf, s21_buf);
}
END_TEST

// ==================== СЬЮТЫ ПО ЧАСТЯМ ЗАДАНИЯ ====================

// === Part 1: Базовые функции string.h ===
Suite* create_part1_string_suite(void) {
  Suite* s = suite_create("Part 1: Basic string.h Functions");

  TCase* tc_core = tcase_create("Core Functions");
  // strlen
  tcase_add_test(tc_core, test_strlen_basic);
  tcase_add_test(tc_core, test_strlen_edge_cases);

  // memcpy/memcmp/memset/memchr
  tcase_add_test(tc_core, test_memcmp_basic);
  tcase_add_test(tc_core, test_memcpy_normal_values);
  tcase_add_test(tc_core, test_memset_regular);
  tcase_add_test(tc_core, test_memchr);

  // strcpy/strncpy/strcmp/strncmp
  tcase_add_test(tc_core, test_strncmp_normal_values);
  tcase_add_test(tc_core, test_strncmp_zero_length);
  tcase_add_test(tc_core, test_strncpy_regular);
  tcase_add_test(tc_core, test_strncpy_null_args);

  // strcat/strncat
  tcase_add_test(tc_core, test_strncat);

  // strchr/strrchr
  tcase_add_test(tc_core, test_strchr_basic);
  tcase_add_test(tc_core, test_strrchr);
  tcase_add_test(tc_core, test_strchr_null_terminator);

  // strstr
  tcase_add_test(tc_core, test_strstr_basic);
  tcase_add_test(tc_core, test_strstr_empty_needle);

  // strpbrk/strcspn
  tcase_add_test(tc_core, test_strpbrk_regular);
  tcase_add_test(tc_core, test_strcspn);

  // strtok
  tcase_add_test(tc_core, test_strtok_basic_tokens);
  tcase_add_test(tc_core, test_strtok_consecutive_delimiters);

  // strerror
  tcase_add_test(tc_core, test_strerror_known_codes);

  suite_add_tcase(s, tc_core);
  return s;
}

// === Part 2: sprintf (обязательные спецификаторы) ===
Suite* create_part2_sprintf_suite(void) {
  Suite* s = suite_create("Part 2: sprintf (Basic Specifiers)");
  TCase* tc = tcase_create("Basic Specifiers Tests");

  // Обязательные спецификаторы: c, d, f, s, u, %
  tcase_add_test(tc, test_sprintf_part2_specifiers);

  // Обязательные флаги: -, +, (пробел)
  tcase_add_test(tc, test_sprintf_part2_flags);

  // Ширина и точность: (число), .(число)
  tcase_add_test(tc, test_sprintf_part2_width_precision);

  // Модификаторы длины: h, l
  tcase_add_test(tc, test_sprintf_part2_length);

  // Граничные случаи для Part 2
  tcase_add_test(tc, test_sprintf_edge_cases);

  suite_add_tcase(s, tc);
  return s;
}

// === Part 3: sprintf (дополнительные модификаторы) ===
Suite* create_part3_sprintf_suite(void) {
  Suite* s = suite_create("Part 3: sprintf (Extended Specifiers)");
  TCase* tc = tcase_create("Extended Specifiers Tests");

  // Дополнительные спецификаторы: g, G, e, E, x, X, o, p
  tcase_add_test(tc, test_sprintf_part3_specifiers);

  // Дополнительные флаги: #, 0
  tcase_add_test(tc, test_sprintf_part3_flags);
  tcase_add_test(tc, test_sprintf_hash_flag);
  tcase_add_test(tc, test_sprintf_zero_flag);

  // Расширенные возможности: * ширина и точность
  tcase_add_test(tc, test_sprintf_width_star);
  tcase_add_test(tc, test_sprintf_precision_star);

  // Дополнительные модификаторы длины: L
  tcase_add_test(tc, test_sprintf_long_double);

  // Специальные значения
  tcase_add_test(tc, test_sprintf_inf_nan);
  tcase_add_test(tc, test_sprintf_gG_edge_cases);

  // Тест для указателей %p (ДОБАВЛЕНО ЗДЕСЬ)
  tcase_add_test(tc, test_sprintf_pointer);

  // Комбинации флагов
  tcase_add_test(tc, test_sprintf_combinations);

  suite_add_tcase(s, tc);
  return s;
}

// === Part 4: sscanf (полная реализация) ===
Suite* create_part4_sscanf_suite(void) {
  Suite* s = suite_create("Part 4: sscanf (Full Implementation)");

  // Группа 1: Базовые типы данных
  TCase* tc_basic = tcase_create("Basic Data Types");
  tcase_add_test(tc_basic, test_sscanf_basic_integers);
  tcase_add_test(tc_basic, test_sscanf_basic_strings_and_chars);
  tcase_add_test(tc_basic, test_sscanf_basic_float);
  tcase_add_test(tc_basic, test_sscanf_basic_hex);
  tcase_add_test(tc_basic, test_sscanf_octal);
  tcase_add_test(tc_basic, test_sscanf_pointer);
  tcase_add_test(tc_basic, test_sscanf_percent);
  suite_add_tcase(s, tc_basic);

  // Группа 2: Форматирование и модификаторы
  TCase* tc_format = tcase_create("Format Modifiers");
  tcase_add_test(tc_format, test_sscanf_literals);
  tcase_add_test(tc_format, test_sscanf_length_modifiers);
  tcase_add_test(tc_format, test_sscanf_width);
  tcase_add_test(tc_format, test_sscanf_width_limitation);
  tcase_add_test(tc_format, test_sscanf_suppress_assignment);
  tcase_add_test(tc_format, test_sscanf_star_format);
  tcase_add_test(tc_format, test_sscanf_width_star);
  tcase_add_test(tc_format, test_sscanf_n_position);
  tcase_add_test(tc_format, test_sscanf_scan_counter);
  suite_add_tcase(s, tc_format);

  // Группа 3: Числа с плавающей точкой
  TCase* tc_float = tcase_create("Floating Point");
  tcase_add_test(tc_float, test_sscanf_float_variants);
  tcase_add_test(tc_float, test_sscanf_float_scientific);
  tcase_add_test(tc_float, test_sscanf_float_formats);
  tcase_add_test(tc_float, test_sscanf_float_spec_check);
  tcase_add_test(tc_float, test_sscanf_negative_numbers);
  suite_add_tcase(s, tc_float);

  // Группа 4: Системы счисления и комбинации
  TCase* tc_numeric = tcase_create("Numeric Systems");
  tcase_add_test(tc_numeric, test_sscanf_hex_variants);
  tcase_add_test(tc_numeric, test_sscanf_mixed_bases);
  tcase_add_test(tc_numeric, test_sscanf_unsigned_with_sign);
  tcase_add_test(tc_numeric, test_sscanf_octal_with_prefix);
  tcase_add_test(tc_numeric, test_sscanf_zero_number);
  suite_add_tcase(s, tc_numeric);

  return s;
}

// === Part 5: Специальные функции обработки строк ===
Suite* create_part5_special_suite(void) {
  Suite* s = suite_create("Part 5: Special String Functions");
  TCase* tc = tcase_create("Special Functions Tests");

  tcase_add_test(tc, test_to_upper);
  tcase_add_test(tc, test_to_lower);
  tcase_add_test(tc, test_insert);
  tcase_add_test(tc, test_trim);

  suite_add_tcase(s, tc);
  return s;
}

// === Edge Cases: Краевые и особые случаи ===
Suite* create_edge_cases_suite(void) {
  Suite* s = suite_create("Edge Cases and Special Conditions");

  TCase* tc_string = tcase_create("String Functions Edge Cases");
  tcase_add_test(tc_string, test_strerror_edge_cases);
  tcase_add_test(tc_string, test_insert_invalid_index);
  tcase_add_test(tc_string, test_sscanf_edge_empty_input);
  tcase_add_test(tc_string, test_sscanf_edge_matching_failure);
  tcase_add_test(tc_string, test_sscanf_edge_char_whitespace);
  tcase_add_test(tc_string, test_sscanf_negative_width);
  tcase_add_test(tc_string, test_sscanf_exponent_error);
  tcase_add_test(tc_string, test_sscanf_literal_mismatch);
  suite_add_tcase(s, tc_string);

  TCase* tc_numeric = tcase_create("Numeric Edge Cases");
  tcase_add_test(tc_numeric, test_sscanf_extreme_numbers);
  tcase_add_test(tc_numeric, test_sscanf_complex_formats);
  tcase_add_test(tc_numeric, test_sscanf_pointer_variants);
  tcase_add_test(tc_numeric, test_sscanf_string_wide_char);
  tcase_add_test(tc_numeric, test_sscanf_unsigned_generic_path);
  tcase_add_test(tc_numeric, test_sscanf_value_modifiers_different_sizes);
  suite_add_tcase(s, tc_numeric);

  return s;
}

Suite* create_part4_plus_sprintf_suite(void) {
  Suite* s =
      suite_create("Part 4+: sprintf (Additional tests for good coverage)");
  TCase* tc = tcase_create("Edge cases for coverage");
  tcase_add_test(tc, test_sprintf_short_unsigned);
  tcase_add_test(tc, test_sprintf_num_to_char_bases);
  tcase_add_test(tc, test_sprintf_num_to_char_zero_and_min);
  suite_add_tcase(s, tc);
  return s;
}

// ==================== MAIN ====================

int run_suite(Suite* suite, const char* name) {
  printf("\n========== %s ==========\n", name);

  SRunner* runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);

  int failed = srunner_ntests_failed(runner);

  srunner_free(runner);
  return failed;
}

int main(void) {
  int total_failed = 0;

  // Part 1: Базовые функции string.h
  Suite* part1 = create_part1_string_suite();
  total_failed += run_suite(part1, "PART 1: BASIC STRING.H FUNCTIONS");

  // Part 2: sprintf (обязательные спецификаторы)
  Suite* part2 = create_part2_sprintf_suite();
  total_failed += run_suite(part2, "PART 2: SPRINTF (BASIC SPECIFIERS)");

  // Part 3: sprintf (дополнительные модификаторы)
  Suite* part3 = create_part3_sprintf_suite();
  total_failed += run_suite(part3, "PART 3: SPRINTF (EXTENDED SPECIFIERS)");

  // Part 4: sscanf (полная реализация)
  Suite* part4 = create_part4_sscanf_suite();
  total_failed += run_suite(part4, "PART 4: SSCANF (FULL IMPLEMENTATION)");

  // Part 4+: дополнительные тесты для полного покрытия
  Suite* part4_plus = create_part4_plus_sprintf_suite();
  total_failed += run_suite(
      part4_plus, "PART 4 PLUS: SPRINTF (ADDITIONAL TESTS FOR COVERAGE)");

  // Part 5: Специальные функции обработки строк
  Suite* part5 = create_part5_special_suite();
  total_failed += run_suite(part5, "PART 5: SPECIAL STRING FUNCTIONS");

  // Edge Cases: Краевые и особые случаи
  Suite* edge = create_edge_cases_suite();
  total_failed += run_suite(edge, "EDGE CASES AND SPECIAL CONDITIONS");

  printf("\n========== SUMMARY ==========\n");
  printf("Total failed tests: %d\n\n", total_failed);

  return total_failed == 0 ? 0 : 1;
}