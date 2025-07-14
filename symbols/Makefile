CC = gcc

prog: main.o mod.o state.o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) -c $^ -o $@

mod.o: mod.c
	$(CC) -c $^ -o $@

state.o: state.c
	$(CC) -c $^ -o $@

.PHONY: clean
clean:
	rm -f *.o prog

symbol-table.md: symbol-table.template.md prog
	sed 's/>>>//e' $< > symbol-table.md
