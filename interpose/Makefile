CC = gcc

all: int intc

int: int.c
	$(CC) -o $@ $^

intc: int.o cmalloc.o
	$(CC) -o $@ $^

int.o: int.s
	$(CC) -Og -g -c $^

int.s: int.c
	$(CC) -I. -Og -S $^

cmalloc.o: cmalloc.s
	$(CC) -Og -g -c $^

cmalloc.s: cmalloc.c
	$(CC) -Og -S $^
	
.PHONY: clean
clean:
	rm -f *.so *.o *.s int intc

