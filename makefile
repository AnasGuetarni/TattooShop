CC=gcc -Wall -Wextra -Werror -std=gnu11 -D_GNU_SOURCE -O3 -g
LIBS= -lpthread -lm -lrt
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
BINS=$(SRCS:%.c=%)

TattooShop: main.o main_functions.o
	$(CC) $^ -o $@ $(LIBS)

main.o: main.c
	$(CC) $< -c

main_functions.o: main_functions.c main_functions.h
	$(CC) $< -c

run: TattooShop
	./TattooShop  5 3 2 2

clean:
	rm -rf $(BINS) $(OBJS) TattooShop
