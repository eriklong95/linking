CC = gcc

all: int intc intl

int: int.o
	$(CC) -o $@ $^

intc: intc.o cmalloc.o
	$(CC) -o $@ $^

intl: int.o lmalloc.o
	$(CC) -Wl,--wrap,malloc -Wl,--wrap,free -o $@ $^

int.o: int.s
	$(CC) -Og -g -c $^

int.s: int.c
	$(CC) -Og -S $^

intc.o: intc.s
	$(CC) -Og -g -c $^

intc.s: int.c
	$(CC) -include malloc.h -Og -S -o $@ $^

cmalloc.o: cmalloc.s
	$(CC) -Og -g -c $^

cmalloc.s: cmalloc.c
	$(CC) -Og -S $^

lmalloc.o: lmalloc.s
	$(CC) -Og -g -c $^

lmalloc.s: lmalloc.c
	$(CC) -Og -S $^
	
.PHONY: clean
clean:
	rm -f *.so *.o *.s int intc intl

