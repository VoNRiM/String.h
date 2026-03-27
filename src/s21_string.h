#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define S21_NULL ((void *)0)

typedef unsigned long s21_size;

// Базовые функции string.h
s21_size s21_strlen(const char *str);
int s21_memcmp(const void *s1, const void *s2, s21_size n);
char *s21_strchr(const char *str, int c);
char *s21_strerror(int errnum);
char *s21_strstr(const char *haystack, const char *needle);

// venonata
void *s21_memcpy(void *destination, const void *source, s21_size amount);
int s21_strncmp(const char *str1, const char *str2, s21_size amount);
char *s21_strtok(char *str, const char *delim);

// 4 8 12
void *s21_memset(void *str, int c, s21_size n);
char *s21_strncpy(char *dest, const char *src, s21_size n);
char *s21_strpbrk(const char *str1, const char *str2);

// supermsp 1 5 9 13
void *s21_memchr(const void *str, int c, s21_size n);
s21_size s21_strcspn(const char *str1, const char *str2);
char *s21_strncat(char *dest, const char *src, s21_size n);
char *s21_strrchr(const char *str, int c);

// Special string processing functions
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *str, const char *insert, s21_size start_index);
void *s21_trim(const char *str, const char *trim_chars);

#endif