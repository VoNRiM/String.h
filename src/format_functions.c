// int sscanf(const char *str, const char *format, ...);
#include "s21_string.h"
#include <stdarg.h> // для макросов va_list, va_start, va_arg, va_end
#include <unistd.h> // Для write, чтоб можно было отслеживать отображение.

int s21_sprintf(char *str, const char *format, ...);
void argument_parser(va_list args,char *str, const char *format, int *str_index, int *format_index);
void int_to_char(int num, char *buffer);
void append_d(va_list args, char *str, int *str_index);



int main (){
    char buffer[1000] = "";
    char *my_str = "Hello_world%d";
    s21_sprintf(buffer, my_str, 10);
    write(1, buffer, 20); 
}

int s21_sprintf(char *str, const char *format, ...){
    int format_index = 0;
    int str_index = 0;
    va_list args;
    va_start(args, format); // Инициализируем аргументы после Format
    while (format[format_index] != '\0') {
        if (format[format_index] != '%'){
            str[str_index] = format[format_index]; 
            format_index ++;
            str_index ++;} 
        else {
            argument_parser(args, str, format, &str_index, &format_index);
        }
    }
    str[str_index] = '\0';
    va_end(args);
    return 0;
}

void argument_parser(va_list args, char *str, const char *format, int *str_index, int *format_index){
    if (format[*format_index + 1] == 'd'){
        append_d(args, str, str_index);
        *format_index += 2;
    }
}

void append_d(va_list args, char *str, int *str_index){
    char word [100];
    int value = va_arg(args,int);
    int_to_char(value, word);

    s21_size_t word_len = s21_strlen(word);
    s21_strncpy(str + *str_index,word, word_len);
    *str_index += word_len;
}

void int_to_char(int num, char*buffer){
    int flag_minus = 0;
    if (num < 0){ // если отицательно помечаем флаг и убмраем минус для работы
        flag_minus = 1;
        num = -num;
    } 
    if (num == 0){
        buffer[0] = '0';
        buffer[1] = '\0';
    } else {
        int x = num;
        int len = 0;       
        while (x!= 0){
            x = x / 10;
            len ++;
        }
        char wrong_buffer [len];
        for (int i = 0; i < len; i ++){
            int result_number = num % 10;
            num =  num / 10;
            wrong_buffer[i] = '0' + result_number; 
        }
        int n = len - 1;
        if (flag_minus == 1){
            buffer[0] = '-';
        } 
        for (int j = flag_minus; j < (len + flag_minus); j++){ // Если есть минус то начинаем за полнять со второго
            buffer[j] = wrong_buffer[n];
            n = n - 1;
        }
        buffer[len+flag_minus] = '\0';
    }
}

