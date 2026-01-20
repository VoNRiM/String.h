#ifndef FORMAT_FUNCTION_H
#define FORMAT_FUNCTION_H

// #include <__stdarg_va_list.h>
#include <math.h>
#include <stdarg.h>  // для макросов va_list, va_start, va_arg, va_end
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>  // Для write, чтоб можно было отслеживать отображение.

#include "s21_string.h"

typedef struct {
  int length;
  int precision;
  char length_descriptor[10];
  char specifier;
  char flags[10];
  int star_in_width;
  int star_in_precision;
} FormatSpecs;

#define SPRINTF_FLAGS "+- #0"
#define DIGIT_LIST "0123456789"
#define LENGTH_DESCRIPTORS "hlL"
#define SPECIFIERS "cdeEfgGosuxXp%"

int s21_sprintf(char *str, const char *format, ...);
void s21_strcpy(char *dest, const char *src);
void flags_parser(FormatSpecs *spec_flags, const char *format,
                  int *format_index);
void width_parser(FormatSpecs *spec_flags, const char *format,
                  int *format_index, va_list args);
void precision_parser(FormatSpecs *spec_flags, const char *format,
                      int *format_index, va_list args);
void append_argument(va_list args, FormatSpecs *spec_flags, char *str,
                     const char *format, int *str_index, int *format_index);
void specifiers_parser(FormatSpecs *spec_flags, const char *format,
                       int *format_index);

void int_to_char(int num, char *buffer);
void long_to_char(long num, char *buffer);
void length_parser(FormatSpecs *spec_flags, const char *format,
                   int *format_index);

void float_to_char(double num, char *buffer, int precision);
void uni_to_char(unsigned int num, char *buffer);
void long_uni_to_char(unsigned long num, char *buffer);
void short_uni_to_char(unsigned int num, char *buffer);
void create_exponential_contact(long double value, char *buffer, int precision,
                                char exp_char);
void format_flag(FormatSpecs *spec_flags, int *len, char *word, int value);
void format_width(FormatSpecs *spec_flags, int *len, char *word);
void format_precision(FormatSpecs *spec_flags, int *len, char *word);
void find_exponent(int *exponent, long double *abs_value);
int check_to_inf_nan(double num, char *buffer, FormatSpecs *spec_flags);
void format_mantiss(char *buffer, int powers, char leading_sign);
void remove_zeroes(char *buffer);
void unsigned_num_to_char(uint64_t value, char *src_buffer, int base);
void remove_tralling_zeroes_exponential(char *buffer);

void append_c(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_s(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_f(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_u(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_e(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_x(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_g(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_o(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_p(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void append_d(va_list args, FormatSpecs *spec_flags, char *str, int *str_index,
              int *format_index);
void apply_hash_flag_for_float(FormatSpecs *spec_flags, char *buffer);

#endif
