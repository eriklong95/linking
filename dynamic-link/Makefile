CC = gcc

all: prog progr

prog: main.o libvector.so
	$(CC) -o $@ $< ./libvector.so

progr: mainr.o libvector.so
	$(CC) -rdynamic -o $@ $< -ldl

mainr.o: mainr.s
	$(CC) -Og -g -c $^

mainr.s: mainr.c
	$(CC) -Og -S $^

main.o: main.s
	$(CC) -Og -g -c $^

main.s: main.c
	$(CC) -Og -S $^

libvector.so: addvec.o multvec.o
	$(CC) -shared -fpic -o $@ $^

addvec.o: addvec.s
	$(CC) -Og -g -fpic -c $^

multvec.o: multvec.s
	$(CC) -Og -g -fpic -c $^

addvec.s: addvec.c
	$(CC) -Og -S -fpic $^

multvec.s: multvec.c
	$(CC) -Og -S -fpic $^

.PHONY: clean
clean:
	rm -f *.so *.o *.s prog static-library.md

dynamic-linking.md: dynamic-linking.template.md
	sed 's/>>>//e' $< > $@

