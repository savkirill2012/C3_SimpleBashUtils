CC=gcc
FL=-Wall -Werror -Wextra

s21_cat: s21_cat.o s21_cat_input_read.o
	$(CC) $(FL) s21_cat_input_read.o s21_cat.o -o s21_cat

s21_cat_input_read.o: s21_cat_input_read.c s21_cat_input_read.h
	$(CC) $(FL) -c s21_cat_input_read.c

s21_cat.o: s21_cat.c s21_cat_input_read.h
	$(CC) $(FL) -c s21_cat.c 

clean:
	rm *.o
	rm s21_cat	