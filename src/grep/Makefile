CC = gcc
FLAGS =-Wall -Wextra -Werror -std=c11
PROJECT_NAME = s21_grep
STYLE_BASE = --style=Google
CPPFLAGS = --enable=all --suppress=missingIncludeSystem --std=c11 --check-config

all: $(PROJECT_NAME)

$(PROJECT_NAME): s21_grep.o
	$(CC) s21_grep.o -o $(PROJECT_NAME)

s21_grep.o:
	$(CC) $(FLAGS) s21_grep.c -c

test:
	bash tests.sh
cpp:
	cppcheck ${CPPFLAGS} .

val:
	bash test_leaks.sh
clang:
	clang-format $(STYLE_BASE) -n *.c *.h

fix:
	clang-format $(STYLE_BASE) -i *.c *.h

clean:
	rm -rf *.o
	rm -rf *.log
	rm -rf $(PROJECT_NAME)

rebuild: clean
	make