# Symbol Resolution

## Symbol Tables

Compiling and assembling the source file

```state.c
int state = 1;
```

by running `gcc -c state.c` yields an object file `state.o` whose symbol
table is

Symbol table '.symtab' contains 3 entries:

```
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS state.c
     2: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    2 state
```

This means that the object module stored in the file `state.o` defines
three linker symbols. 
