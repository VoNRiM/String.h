CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LIBS = -lcheck -lsubunit -lm -lpthread

# Файлы исходного кода
SRC_FILES = basic_functions.c
OBJ_FILES = $(SRC_FILES:.c=.o)
TEST_SRC = test_s21_string.c

# Основные цели
all: s21_string.a test

# Сборка статической библиотеки
s21_string.a: $(OBJ_FILES)
	ar rc s21_string.a $(OBJ_FILES)
	ranlib s21_string.a

# Правило для компиляции .c файлов в .o файлы
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Тестирование
test: s21_string.a
	$(CC) $(CFLAGS) $(TEST_SRC) s21_string.a -o test_app $(LIBS)
	./test_app

# Отчет покрытия
gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC_FILES) $(TEST_SRC) -o gcov_app $(LIBS)
	./gcov_app
	lcov -c -d . -o coverage.info
	genhtml coverage.info -o report

# Очистка
clean:
	rm -f *.a test_app gcov_app *.gcda *.gcno coverage.info *.o
	rm -rf report

# Стилевая правка 
clang: 
	clang-format -i *.c *.h 

# Статический анализ памяти
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c 

.PHONY: all test gcov_report clean clang cppcheck 
