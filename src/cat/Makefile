CC = gcc
FLAGS =-Wall -Wextra -Werror -std=c11
PROJECT_NAME = s21_cat
STYLE_BASE = --style=Google
CPPFLAGS = --enable=all --suppress=missingIncludeSystem --std=c11 --check-config


all: $(PROJECT_NAME)

$(PROJECT_NAME): s21_cat.o parcer.o reader.o
	$(CC) s21_cat.o parcer.o reader.o -o $(PROJECT_NAME)

s21_cat.o:
	$(CC) $(FLAGS) s21_cat.c parcer.c reader.c -c

clang:
	clang-format $(STYLE_BASE) -n *.c *.h

fix:
	clang-format $(STYLE_BASE) -i *.c *.h


cpp:
	cppcheck ${CPPFLAGS} .

clean:
	rm -rf *.o
	rm -rf $(PROJECT_NAME)

rebuild: clean
	make