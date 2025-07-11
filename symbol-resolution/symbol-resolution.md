# Symbol Resolution

## Symbol Tables

Compiling and assembling the source file

```state.c
int state = 1;
```

by running `gcc -c state.c` yields an object file `state.o` whose symbol
table is

```
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS state.c
     2: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    2 state
```

(This table can be printed by running `readelf -s state.o`.)

This means that the object module stored in the file `state.o` defines
three linker symbols. 

How is this symbol table data actually stored in the file `state.o`? To
understand this, we must print the part of the file which contains the data
for the symbol table. But how do we know where to look in the file? We can get
that information from the *section header table*.

The command `readelf -S state.o` outputs the section header table of the object
module stored in `state.o` in a human-readable format. Part of the output looks
like this:

```
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
--
  [ 7] .symtab           SYMTAB           0000000000000000  00000090
       0000000000000048  0000000000000018           8     2     8
```

From this we learn that the symbol starts at offset `0x90` from the beginning of
the file and that it is `0x48` bytes long. We dump this part of `state.o` by
running `xxd -s 0x90 -l 0x48 state.o`. That gives the output.

```
00000090: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000a0: 0000 0000 0000 0000 0100 0000 0400 f1ff  ................
000000b0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000c0: 0900 0000 1100 0300 0000 0000 0000 0000  ................
000000d0: 0400 0000 0000 0000                      ........
```

In its raw form, the symbol table is just an array of symbol table entries. 
With ELF on 64 bit systems, each entry takes up 24 bytes. We know that the
symbol table has 3 symbols, so it makes sense that the symbol takes up `0x48`
bytes since `3 * 24 = 72 = 0x48`.

Let's inspect the last symbol table entry, the one corresponding to the symbol
`state`. The hex-dump of this symbol table entry is

```
000000c0: 0900 0000 1100 0300 0000 0000 0000 0000  ................
000000d0: 0400 0000 0000 0000                      ........
```

Different parts of this byte sequence defines different properties of the symbol.

The first four bytes defines an `int` which is the `name` property. This is an
offset from the beginning of the `.strtab` (string table) section of the object
 module which indicates the beginning of the null-terminated string which is the
name of the symbol. In this case, those four bytes defines the number `9` (little-
endian byte ordering).

To find this string, we first read the section header table to find the offset of
the `.strtab` section.

```
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
--
  [ 8] .strtab           STRTAB           0000000000000000  000000d8
       000000000000000f  0000000000000000           0     0     1
```

We see that the `.strtab` section starts at offset `0xd8`, and dumping `state.o`
from 9 bytes after this offset gives

```
000000e1: 7374 6174 6500 002e 7379 6d74 6162 002e  state...symtab..
```

This tells us that the name of the last entry in the symbol table is `state` (which
is not a surprise).

The next byte of a symbol table entry defines the `type` and `binding` properties of
the entry with the first 4 bits defining the `type` and the next 4 defining the
`binding`. The `type` property tells use which kind of symbol we dealing with (data
object or function or something else) and the `binding` tells use whether the symbol
is local or global. In our case, this byte is `0x11`, so the `type` is the bits
`0001` and `binding` is the bits `0001`. What does that mean?

The next byte is unused.

The next two bytes is the `section` property which tells us which section contains
the definition of the symbol. In our case, `section` is 3, which is also the index
of the

value, address? What do we mean by the `value` of a symbol?

The last 4 bytes of the entry defines a `long` which indicates the size of the
symbol. In our case, that number is 4 which makes sense since `state` is declared
as a variable of type `int` and `int`s are 4 bytes long.

Where is it defined that the variable `state` is initialized to `1`?

What are the real names of the properties of a symbol table entry?
