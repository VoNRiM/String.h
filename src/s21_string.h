#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdlib.h>
#include <string.h> // временно пока не подтянем все 15 функций

#define S21_NULL ((void *)0)

typedef unsigned long s21_size_t;

// Базовые функции string.h
s21_size_t s21_strlen(const char *str);
int s21_memcmp(const void *s1, const void *s2, s21_size_t n);
char *s21_strchr(const char *str, int c);
char *s21_strerror(int errnum);
char *s21_strstr(const char *haystack, const char *needle);

// venonata
void *s21_memcpy(void *destination, const void *source, size_t amount);
int s21_strncmp(const char *str1, const char *str2, size_t amount);
char *s21_strtok(char *str, const char *delim);

// 4 8 12
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
char *s21_strpbrk(const char *look_in, const char *look_for);

// supermsp 1 5 9 13
void *s21_memchr(const void *str, int c, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strrchr(const char *str, int c);

#endif