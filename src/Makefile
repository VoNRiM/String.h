CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LIBS = -lcheck -lsubunit -lm -lpthread

# Все исходные файлы
SRC_FILES = basic_functions.c \
            special_processing_functions.c \
            s21_sscanf.c \
            s21_sscanf_dispatcher.c \
            s21_sscanf_parser.c \
            s21_sscanf_processor.c \
            s21_sscanf_readers.c \
            s21_sscanf_utils.c \
            format_functions.c

OBJ_FILES = $(SRC_FILES:.c=.o)

# Тестовый файл
TEST_SRC = test_s21_string.c
TEST_OBJ = $(TEST_SRC:.c=.o)

# Имя исполняемого теста
TEST_EXEC = test_app

# Основные цели
all: s21_string.a test

# Сборка статической библиотеки
s21_string.a: $(OBJ_FILES)
	ar rc s21_string.a $(OBJ_FILES)
	ranlib s21_string.a

# Правило для компиляции .c файлов в .o файлы
%.o: %.c s21_string.h errors.h s21_sscanf.h
	$(CC) $(CFLAGS) -c $< -o $@

# Тестирование
test: s21_string.a $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) s21_string.a -o $(TEST_EXEC) $(LIBS)
	./$(TEST_EXEC)

# Отчет покрытия
gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC_FILES) $(TEST_SRC) -o gcov_app $(LIBS)
	./gcov_app
	lcov -c -d . -o coverage.info
	genhtml coverage.info -o report

# Очистка
clean:
	rm -f *.a $(TEST_EXEC) gcov_app *.gcda *.gcno coverage.info *.o
	rm -rf report

# Стилевая правка 
clang: 
	clang-format -i *.c *.h 

# Статический анализ памяти
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c 

.PHONY: all test gcov_report clean clang cppcheck
