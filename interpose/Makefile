CC = gcc

all: int intc

int: int.c
	$(CC) -o $@ $^

intc: int.c cmalloc.o
	$(CC) -I. -o $@ $^

cmalloc.o: cmalloc.s
	$(CC) -Og -g -c cmalloc.s

cmalloc.s: cmalloc.c
	$(CC) -DCOMPILETIME -Og -S $^
	
.PHONY: clean
clean:
	rm -f *.so *.o *.s int intc

