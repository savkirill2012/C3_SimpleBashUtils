CC=gcc
FLAGS=-Wall -Werror -Wextra 

s21_grep: s21_grep.o s21_grep_lib.o
	$(CC) $(FLAGS) s21_grep.o s21_grep_lib.o -o s21_grep

s21_grep_lib.o: s21_grep_lib.c s21_grep_lib.h
	$(CC) $(FLAGS) -c s21_grep_lib.c

s21_grep.o: s21_grep.c s21_grep_lib.h
	$(CC) $(FLAGS) -c s21_grep.c

clean:
	rm *.o
	rm s21_grep