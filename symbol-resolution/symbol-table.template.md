# Symbol Tables

Compiling and assembling the source file

```state.c
>>> cat state.c
```

by running `gcc -c state.c` yields an object file `state.o` whose symbol
table is

```
>>> readelf -s state.o
```

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
>>> readelf -S state.o | grep -E "Name|symtab" -A 1
```

From this we learn that the symbol starts at offset `0x90` from the beginning of
the file and that it is `0x48` bytes long. We dump this part of `state.o`:

```
>>> xxd -s 0x90 -l 0x48 state.o
```

In its raw form, the symbol table is just an array of symbol table entries. 
With ELF on 64 bit systems, each entry takes up 24 bytes. We know that the
symbol table has 3 symbols, so it makes sense that the symbol takes up `0x48`
bytes since `3 * 24 = 72 = 0x48`.

Let's inspect the last symbol table entry, the one corresponding to the symbol
`state`. The hex-dump of this symbol table entry is

```
>>> xxd -s 0xc0 -l 0x18 state.o
```

Different parts of this byte sequence defines different properties of the symbol.

The first four bytes defines `st_name` property. The data type is `Elf64_Word`, an
unsigned integer. This property is an offset from the beginning of the `.strtab`
(string table) section of the object module which indicates the beginning of the
null-terminated string which is the
name of the symbol. In this case, those four bytes defines the number `9` (little-
endian byte ordering).

To find this string, we first read the section header table to find the offset of
the `.strtab` section.

```
>>> readelf -S state.o | grep -E "Name|\.strtab" -A 1
```

We see that the `.strtab` section starts at offset `0xd8`, and dumping `state.o`
from 9 bytes after this offset gives

```
>>> xxd -s 0xe1 -l 0x10 state.o
```

This tells us that the name of the last entry in the symbol table is `state` (which
is not a surprise).

The next byte of a symbol table entry defines the `st_info` property. This property
tells us the type and the binding (scope) of the symbol. The four high-order bits
define the binding, the four low-order bits define the type. In this case, the byte
is `0x11`. This means that the `state` symbol has value `1` for the binding and also
value `1` for the type.

For symbol bindings, value `1` indicates the binding `STB_GLOBAL` which means that
this symbol is a global symbol i.e. it is visible to all object files. It makes good
sense that the `state` symbol has this binding because we know that, in the program
we are building, it is referenced by another module.

For symbol types, value `1` indicates the type `STT_OBJECT` which means that the
symbol is a data object. Again, this makes good sense because `state` is a variable
holding an `int` value.

The next byte defines the `st_other` property which is unused.

The next two bytes defines the `st_shndx` property which indicates the index of the
section where the symbol is defined. The data type is `Elf64_Half`, a two-byte
long unsigned integer. In our case, the property has the value `2`. We can look up
that this is the index of the `.data` section. This is the section for initialized
variables so this makes sense.

value, address? What do we mean by the `value` of a symbol?

The last 4 bytes of the entry defines a `long` which indicates the size of the
symbol. In our case, that number is 4 which makes sense since `state` is declared
as a variable of type `int` and `int`s are 4 bytes long.

Where is it defined that the variable `state` is initialized to `1`? Because this
is a relocatable object file and the variable is defined in this module, we can just
find the section where it is defined and the `value` property will be the offset
into this section where the symbol is defined. Here we find `1`

What are the real names of the properties of a symbol table entry?

