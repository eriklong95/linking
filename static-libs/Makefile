CC = gcc

prog: main.o libvector.a
	$(CC) -static -o $@ $< ./libvector.a

main.o: main.s
	$(CC) -Og -g -c $^

main.s: main.c
	$(CC) -Og -S $^

libvector.a: addvec.o multvec.o
	ar rcs $@ $^

addvec.o: addvec.s
	$(CC) -Og -g -c $^

multvec.o: multvec.s
	$(CC) -Og -g -c $^

addvec.s: addvec.c
	$(CC) -Og -S $^

multvec.s: multvec.c
	$(CC) -Og -S $^

.PHONY: clean
clean:
	rm -f *.o *.s prog static-library.md

static-library.md: static-library.template.md
	sed 's/>>>//e' $< > static-library.md

