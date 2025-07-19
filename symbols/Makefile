CC = gcc

prog: main.o mod.o state.o
	$(CC) -Og -g -o $@ $^

main.o: main.s
	$(CC) -Og -g -c -o $@ $^

mod.o: mod.s
	$(CC) -Og -g -c -o $@ $^

state.o: state.s
	$(CC) -Og -g -c -o $@ $^

main.s: main.c
	$(CC) -Og -S -o $@ $^

mod.s: mod.c
	$(CC) -Og -S -o $@ $^

state.s: state.c
	$(CC) -Og -S -o $@ $^

.PHONY: clean
clean:
	rm -f *.o *.s prog symbol-table.md relocation.md

symbol-table.md: symbol-table.template.md prog
	sed 's/>>>//e' $< > symbol-table.md

relocation.md: relocation.template.md prog
	sed 's/>>>//e' $< > relocation.md
