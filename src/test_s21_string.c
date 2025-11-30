#include <check.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "s21_string.h"

// ==================== strlen тесты ====================
START_TEST(test_s21_strlen_basic) {
  // Проверяем базовые случаи
  ck_assert_int_eq(s21_strlen(""), strlen(""));
  ck_assert_int_eq(s21_strlen("a"), strlen("a"));
  ck_assert_int_eq(s21_strlen("hello"), strlen("hello"));
  ck_assert_int_eq(s21_strlen("Hello, World!"), strlen("Hello, World!"));
}
END_TEST

START_TEST(test_s21_strlen_edge_cases) {
  // Граничные случаи
  ck_assert_int_eq(s21_strlen(" "), strlen(" ")); // один пробел
  ck_assert_int_eq(s21_strlen("\0"), strlen("\0")); // только нуль-терминатор
  ck_assert_int_eq(s21_strlen("12345"), strlen("12345")); // цифры
  ck_assert_int_eq(s21_strlen("!@#$%"), strlen("!@#$%")); // спецсимволы
}
END_TEST

START_TEST(test_s21_strlen_long_strings) {
  // Длинные строки
  char long_str[100] =
      "This is a very long string for testing strlen function with multiple "
      "characters";
  ck_assert_int_eq(s21_strlen(long_str), strlen(long_str));

  // Строка с повторяющимися символами
  ck_assert_int_eq(s21_strlen("aaaaaaaaaa"), strlen("aaaaaaaaaa"));
}
END_TEST

// ==================== memcmp тесты ====================
START_TEST(test_s21_memcmp_basic) {
  char str1[] = "Hello";
  char str2[] = "Hello";
  char str3[] = "Hellp";

  // Сравнение одинаковых строк
  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));

  // Сравнение разных строк
  ck_assert_int_eq(s21_memcmp(str1, str3, 5), memcmp(str1, str3, 5));

  // Сравнение части строки
  ck_assert_int_eq(s21_memcmp(str1, str2, 3), memcmp(str1, str2, 3));
}
END_TEST

START_TEST(test_s21_memcmp_edge_cases) {
  char str1[] = "abc";
  char str2[] = "abd";

  // Нулевая длина
  ck_assert_int_eq(s21_memcmp(str1, str2, 0), memcmp(str1, str2, 0));

  // Сравнение одного символа
  ck_assert_int_eq(s21_memcmp(str1, str2, 1), memcmp(str1, str2, 1));

  // Отрицательный результат
  ck_assert_int_lt(s21_memcmp(str1, str2, 3), 0);
  ck_assert_int_lt(memcmp(str1, str2, 3), 0);
}
END_TEST

START_TEST(test_s21_memcmp_binary_data) {
  unsigned char data1[] = {0x01, 0x02, 0x03, 0x04};
  unsigned char data2[] = {0x01, 0x02, 0x03, 0x05};

  ck_assert_int_eq(s21_memcmp(data1, data2, 4), memcmp(data1, data2, 4));
  ck_assert_int_eq(s21_memcmp(data1, data2, 3), memcmp(data1, data2, 3));
}
END_TEST

// ==================== strchr тесты ====================
START_TEST(test_s21_strchr_basic) {
  char str[] = "Hello, World!";

  // Поиск существующего символа
  ck_assert_ptr_eq(s21_strchr(str, 'H'), strchr(str, 'H'));
  ck_assert_ptr_eq(s21_strchr(str, 'o'), strchr(str, 'o'));
  ck_assert_ptr_eq(s21_strchr(str, '!'), strchr(str, '!'));

  // Поиск несуществующего символа
  ck_assert_ptr_eq(s21_strchr(str, 'x'), strchr(str, 'x'));
  ck_assert_ptr_eq(s21_strchr(str, 'Z'), strchr(str, 'Z'));
}
END_TEST

START_TEST(test_s21_strchr_special_cases) {
  char str[] = "test";

  // Поиск нулевого терминатора
  ck_assert_ptr_eq(s21_strchr(str, '\0'), strchr(str, '\0'));

  // Поиск в строке с повторяющимися символами
  char repeated[] = "abcabc";
  ck_assert_ptr_eq(s21_strchr(repeated, 'b'), strchr(repeated, 'b'));

  // Пустая строка
  char empty[] = "";
  ck_assert_ptr_eq(s21_strchr(empty, 'a'), strchr(empty, 'a'));
  ck_assert_ptr_eq(s21_strchr(empty, '\0'), strchr(empty, '\0'));
}
END_TEST

START_TEST(test_s21_strchr_unicode) {
  // Тестирование с ASCII символами
  char str[] = "abc123!@#";

  ck_assert_ptr_eq(s21_strchr(str, '1'), strchr(str, '1'));
  ck_assert_ptr_eq(s21_strchr(str, '!'), strchr(str, '!'));
  ck_assert_ptr_eq(s21_strchr(str, '#'), strchr(str, '#'));
}
END_TEST

// ==================== strerror тесты ====================
START_TEST(test_s21_strerror_known_codes) {
  // Проверяем известные коды ошибок
  for (int i = 0; i < S21_SYS_NERR; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}
END_TEST

START_TEST(test_s21_strerror_edge_cases) {
  // Код сразу за границей
  ck_assert_str_eq(s21_strerror(S21_SYS_NERR), strerror(S21_SYS_NERR));

  // Отрицательные коды
  ck_assert_str_eq(s21_strerror(-1), strerror(-1));
  ck_assert_str_eq(s21_strerror(-100), strerror(-100));
}
END_TEST

START_TEST(test_s21_strerror_unknown_codes) {
  // Очень большие коды
  ck_assert_str_eq(s21_strerror(999), strerror(999));
  ck_assert_str_eq(s21_strerror(1000), strerror(1000));
}
END_TEST

// ==================== strstr тесты ====================
START_TEST(test_s21_strstr_basic) {
  char haystack[] = "Hello, World!";

  // Поиск существующих подстрок
  ck_assert_ptr_eq(s21_strstr(haystack, "Hello"), strstr(haystack, "Hello"));
  ck_assert_ptr_eq(s21_strstr(haystack, "World"), strstr(haystack, "World"));
  ck_assert_ptr_eq(s21_strstr(haystack, "!"), strstr(haystack, "!"));

  // Поиск несуществующих подстрок
  ck_assert_ptr_eq(s21_strstr(haystack, "abc"), strstr(haystack, "abc"));
  ck_assert_ptr_eq(s21_strstr(haystack, "test"), strstr(haystack, "test"));
}
END_TEST

START_TEST(test_s21_strstr_edge_cases) {
  // Пустая needle
  ck_assert_ptr_eq(s21_strstr("abc", ""), strstr("abc", ""));

  // Пустой haystack
  ck_assert_ptr_eq(s21_strstr("", "abc"), strstr("", "abc"));

  // Оба пустые
  ck_assert_ptr_eq(s21_strstr("", ""), strstr("", ""));

  // Needle длиннее haystack
  ck_assert_ptr_eq(s21_strstr("abc", "abcdef"), strstr("abc", "abcdef"));
}
END_TEST

START_TEST(test_s21_strstr_partial_matches) {
  char text[] = "abcabcabc";

  // Частичные совпадения
  ck_assert_ptr_eq(s21_strstr(text, "abc"), strstr(text, "abc"));
  ck_assert_ptr_eq(s21_strstr(text, "bca"), strstr(text, "bca"));
  ck_assert_ptr_eq(s21_strstr(text, "cab"), strstr(text, "cab"));

  // Поиск с пересечениями
  ck_assert_ptr_eq(s21_strstr("mississippi", "iss"),
                   strstr("mississippi", "iss"));
}
END_TEST

// ==================== memcpy тесты ==================== venonata
START_TEST(test_s21_memcpy_normal_values) {
  char src[] = "I love Anapa town!";
  char dest1[19] = {};
  char dest2[19] = {};

  // сравниваем прям так т.к. memcpy возвращает указатель на dest
  ck_assert_mem_eq(s21_memcpy(dest1, src, 3), memcpy(dest2, src, 3), 3);
  ck_assert_mem_eq(s21_memcpy(dest1, src, 6), memcpy(dest2, src, 6), 6);
  ck_assert_mem_eq(s21_memcpy(dest1, src, 19), memcpy(dest2, src, 19), 19);
}
END_TEST

START_TEST(test_s21_memcpy_edge_values) {
  char src[] = "123";
  char dest1[4] = "Hey";
  char dest2[4] = "Hey";

  // ничего не копируем dest не должен измениться
  ck_assert_mem_eq(s21_memcpy(dest1, src, 0), memcpy(dest2, src, 0),
                   sizeof(dest1));
  // минимальный ненулевой размер
  ck_assert_mem_eq(s21_memcpy(dest1, src, 1), memcpy(dest2, src, 1),
                   sizeof(dest1));
  // без копирования завершающего \0
  ck_assert_mem_eq(s21_memcpy(dest1, src, 3), memcpy(dest2, src, 3),
                   sizeof(dest1));
  // просто проверка указателя
  ck_assert_ptr_eq(s21_memcpy(dest1, src, 2), dest1);
}
END_TEST

START_TEST(test_s21_memcpy_complicated_edge_values) {
  int src_int[4] = {1, 2, 3, 4};
  int dest1_int[4] = {0};
  int dest2_int[4] = {0};

  char big1[16] = {0};
  char big2[16] = {0};

  // тест бинарных данных - интов
  s21_memcpy(dest1_int, src_int, sizeof(src_int));
  memcpy(dest2_int, src_int, sizeof(src_int));
  ck_assert_mem_eq(dest1_int, dest2_int, sizeof(src_int));

  // адреса со смещением
  s21_memcpy(big1 + 1, "ABCDEFG", 7);
  memcpy(big2 + 1, "ABCDEFG", 7);
  ck_assert_mem_eq(big1, big2, sizeof(big1));
}
END_TEST

// ==================== memset тесты ====================
START_TEST(test_s21_memset_regular_1) {
  // Простой тест - 1, 8 байтов
  char str1[] = "Test_string";
  char str2[] = "Test_string";
  char c = '0';

  s21_memset((void *)str1, (int)c, 8);
  memset((void *)str2, (int)c, 8);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_s21_memset_regular_2) {
  // Простой тест - 2, 1 байт
  char str1[] = "Test_string";
  char str2[] = "Test_string";
  char c = 'S';

  s21_memset((void *)str1, (int)c, 1);
  memset((void *)str2, (int)c, 1);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_s21_memset_regular_space) {
  // Простой тест с пробелом, 10 байтов
  char str1[] = "Test_string";
  char str2[] = "Test_string";
  char c = ' ';

  s21_memset((void *)str1, (int)c, 10);
  memset((void *)str2, (int)c, 10);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_s21_memset_small_str) {
  // Тест с маленькой строкой, 2 байта
  char str1[] = "T";
  char str2[] = "T";
  char c = '3';

  s21_memset((void *)str1, (int)c, 1);
  memset((void *)str2, (int)c, 1);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_s21_memset_null_1) {
  // Тест с нулевым символом в тестовой строке, 2 байта
  char str1[] = "\0Test_string";
  char str2[] = "\0Test_string";
  char c = '<';

  s21_memset((void *)str1, (int)c, 2);
  memset((void *)str2, (int)c, 2);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_s21_memset_null_2) {
  // Тест с нулевым символом для заполнения, 2 байта
  char str1[] = "Test_string";
  char str2[] = "Test_string";
  char c = '\0';

  s21_memset((void *)str1, (int)c, 2);
  memset((void *)str2, (int)c, 2);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_s21_memset_special) {
  // Тест со специальным символом, 6 байт
  char str1[] = "Test_string";
  char str2[] = "Test_string";
  char c = '\t';

  s21_memset((void *)str1, (int)c, 6);
  memset((void *)str2, (int)c, 6);
  ck_assert_str_eq(str1, str2);
}
END_TEST

// ==================== strncpy тесты ====================
START_TEST(test_s21_strncpy_regular_1) {
  // Простой тест - 1
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "aghfdaga";

  char *result = s21_strncpy(dest1, src, 5);
  char *expectation = strncpy(dest2, src, 5);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_regular_2) {
  // Простой тест - 2
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "aghfdaga";

  char *result = s21_strncpy(dest1, src, 1);
  char *expectation = strncpy(dest2, src, 1);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_shorter_src) {
  // Тест со строкой src меньшей n
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "agh";

  char *result = s21_strncpy(dest1, src, 8);
  char *expectation = strncpy(dest2, src, 8);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_longer_src) {
  // Тест со строкой src большей n
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "agha1234";

  char *result = s21_strncpy(dest1, src, 4);
  char *expectation = strncpy(dest2, src, 4);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_empty_src) {
  // Тест с пустой строкой src
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "";

  char *result = s21_strncpy(dest1, src, 5);
  char *expectation = strncpy(dest2, src, 5);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_empty_dest) {
  // Тест с пустой строкой dest
  char dest1[10] = "";
  char dest2[10] = "";
  char src[] = "asbas";

  char *result = s21_strncpy(dest1, src, 5);
  char *expectation = strncpy(dest2, src, 5);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_null_src) {
  // Тест с нулевым символом в src
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "asb\0";

  char *result = s21_strncpy(dest1, src, 5);
  char *expectation = strncpy(dest2, src, 5);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_identical) {
  // Тест с одинаковыми src и dest
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "Test_string";

  char *result = s21_strncpy(dest1, src, 11);
  char *expectation = strncpy(dest2, src, 11);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_null_dest) {
  // Тест с нулевыми символами в dest
  char dest1[20] = "Test\0";
  char dest2[20] = "Test\0";
  char src[] = "123456789";

  char *result = s21_strncpy(dest1, src, 7);
  char *expectation = strncpy(dest2, src, 7);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_null_both) {
  // Тест с нулевыми символами в dest и src
  char dest1[] = "\0\0\0";
  char dest2[] = "\0\0\0";
  char src[] = "\0";

  char *result = s21_strncpy(dest1, src, 1);
  char *expectation = strncpy(dest2, src, 1);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strncpy_shifted_start) {
  // Тест со стартом не в начале строки
  char dest1[] = "Test_string";
  char dest2[] = "Test_string";
  char src[] = "1234";

  char *result = s21_strncpy((dest1 + 5), src, 4);
  char *expectation = strncpy((dest2 + 5), src, 4);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(result, expectation);
}
END_TEST

// ==================== strpbrk тесты ====================

START_TEST(test_s21_strpbrk_regular) {
  // Простой тест - 1
  char dest[20] = "Test_string";
  char src[20] = "erg";

  char *result = s21_strpbrk(dest, src);
  char *expectation = strpbrk(dest, src);

  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strpbrk_empty_dest) {
  // Тест с пустым dest
  char dest[20] = "";
  char src[20] = "erg";

  char *result = s21_strpbrk(dest, src);
  char *expectation = strpbrk(dest, src);

  ck_assert_ptr_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strpbrk_zero_match) {
  // Тест без совпадений
  char dest[20] = "absdgdafda";
  char src[20] = "1234";

  char *result = s21_strpbrk(dest, src);
  char *expectation = strpbrk(dest, src);

  ck_assert_ptr_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strpbrk_empty_src) {
  // Тест с пустым src
  char dest[10] = "absdgdafda";
  char src[10] = "";

  char *result = s21_strpbrk(dest, src);
  char *expectation = strpbrk(dest, src);

  ck_assert_ptr_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strpbrk_empty_both) {
  // Тест с пустыми src и dest
  char dest[20] = "";
  char src[20] = "";

  char *result = s21_strpbrk(dest, src);
  char *expectation = strpbrk(dest, src);

  ck_assert_ptr_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strpbrk_single_match) {
  // Тест с одним совпадением
  char dest[20] = "absdgda2da";
  char src[20] = "2";

  char *result = s21_strpbrk(dest, src);
  char *expectation = strpbrk(dest, src);

  ck_assert_str_eq(result, expectation);
}
END_TEST

START_TEST(test_s21_strpbrk_null) {
  // Тест с нулевыми символами
  char dest[20] = "\0\0";
  char src[20] = "\0";

  char *result = s21_strpbrk(dest, src);
  char *expectation = strpbrk(dest, src);

  ck_assert_ptr_eq(result, expectation);
}
END_TEST

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

START_TEST(test_strcspn_null) {
  ck_assert_int_eq(strcspn("Hello", ""), s21_strcspn("Hello", ""));
  ck_assert_int_eq(strcspn("", "0987654321"), s21_strcspn("", "0987654321"));
  ck_assert_int_eq(strcspn("", ""), s21_strcspn("", ""));
}
END_TEST

START_TEST(test_strncat) {
  char dest1[50] = "Hello world1";
  char dest2[50] = "Hello world1";

  ck_assert_str_eq(strncat(dest1, "I_LIKE_OLD_WOMAN", 10),
                   s21_strncat(dest2, "I_LIKE_OLD_WOMAN", 10));
  ck_assert_str_eq(strncat(dest1, "I_LIKE_OLD_WOMAN", 5),
                   s21_strncat(dest2, "I_LIKE_OLD_WOMAN", 5));
}
END_TEST

START_TEST(test_strncat_null) {
  char dest1[10] = "";
  char dest2[10] = "";

  ck_assert_str_eq(strncat(dest1, "", 5), s21_strncat(dest2, "", 5));
  ck_assert_str_eq(strncat(dest1, "I_LIKE_OLD_WOMAN", 5),
                   s21_strncat(dest2, "I_LIKE_OLD_WOMAN", 5));
}
END_TEST

START_TEST(test_strrchr) {
  ck_assert_ptr_eq(strrchr("Hello world", 'l'),
                   s21_strrchr("Hello world", 'l'));
  ck_assert_ptr_eq(strrchr("Hello world", 'h'),
                   s21_strrchr("Hello world", 'h'));
}
END_TEST

START_TEST(test_strrchr_null) {
  ck_assert_ptr_eq(strrchr("", 'l'), s21_strrchr("", 'l'));
}
END_TEST

int main(void) {
  Suite *suite = suite_create("s21_string");
  TCase *tc = tcase_create("Core");
  SRunner *runner = srunner_create(suite);

  tcase_add_test(tc, test_s21_strlen_basic);
  tcase_add_test(tc, test_s21_strlen_edge_cases);
  tcase_add_test(tc, test_s21_strlen_long_strings);

  tcase_add_test(tc, test_s21_memcmp_basic);
  tcase_add_test(tc, test_s21_memcmp_edge_cases);
  tcase_add_test(tc, test_s21_memcmp_binary_data);

  tcase_add_test(tc, test_s21_strchr_basic);
  tcase_add_test(tc, test_s21_strchr_special_cases);
  tcase_add_test(tc, test_s21_strchr_unicode);

  tcase_add_test(tc, test_s21_strerror_known_codes);
  tcase_add_test(tc, test_s21_strerror_edge_cases);
  tcase_add_test(tc, test_s21_strerror_unknown_codes);

  tcase_add_test(tc, test_s21_strstr_basic);
  tcase_add_test(tc, test_s21_strstr_edge_cases);
  tcase_add_test(tc, test_s21_strstr_partial_matches);

  tcase_add_test(tc, test_s21_memcpy_normal_values);
  tcase_add_test(tc, test_s21_memcpy_edge_values);
  tcase_add_test(tc, test_s21_memcpy_complicated_edge_values);

  tcase_add_test(tc, test_s21_memset_regular_1);
  tcase_add_test(tc, test_s21_memset_regular_2);
  tcase_add_test(tc, test_s21_memset_regular_space);
  tcase_add_test(tc, test_s21_memset_small_str);
  tcase_add_test(tc, test_s21_memset_null_1);
  tcase_add_test(tc, test_s21_memset_null_2);
  tcase_add_test(tc, test_s21_memset_special);

  tcase_add_test(tc, test_s21_strncpy_regular_1);
  tcase_add_test(tc, test_s21_strncpy_regular_2);
  tcase_add_test(tc, test_s21_strncpy_shorter_src);
  tcase_add_test(tc, test_s21_strncpy_longer_src);
  tcase_add_test(tc, test_s21_strncpy_empty_src);
  tcase_add_test(tc, test_s21_strncpy_empty_dest);
  tcase_add_test(tc, test_s21_strncpy_null_src);
  tcase_add_test(tc, test_s21_strncpy_identical);
  tcase_add_test(tc, test_s21_strncpy_null_dest);
  tcase_add_test(tc, test_s21_strncpy_null_both);
  tcase_add_test(tc, test_s21_strncpy_shifted_start);

  tcase_add_test(tc, test_s21_strpbrk_regular);
  tcase_add_test(tc, test_s21_strpbrk_empty_dest);
  tcase_add_test(tc, test_s21_strpbrk_zero_match);
  tcase_add_test(tc, test_s21_strpbrk_empty_src);
  tcase_add_test(tc, test_s21_strpbrk_empty_both);
  tcase_add_test(tc, test_s21_strpbrk_single_match);
  tcase_add_test(tc, test_s21_strpbrk_null);

  tcase_add_test(tc, test_memchr);
  tcase_add_test(tc, test_strcspn);
  tcase_add_test(tc, test_strncat);
  tcase_add_test(tc, test_strrchr);
  tcase_add_test(tc, test_strcspn_null);
  tcase_add_test(tc, test_strncat_null);
  tcase_add_test(tc, test_strrchr_null);

  suite_add_tcase(suite, tc);

  srunner_run_all(runner, CK_NORMAL);
  srunner_free(runner);

  return 0;
}