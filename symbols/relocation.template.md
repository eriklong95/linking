# Relocation

The source file `mod.c` references the externally defined variable `state`
at the first line of the `modify` function.

```
>>> cat mod.c
```

In the program which results from compiling `mod.c` and linking it with
other object modules, there must be instruction(s) which corresponds to this
reference to `state` in the source code, and that/those instruction(s)
must include a reference to the place in memory where the value for the
variable `state` is stored.

In this document, we investigate how this memory reference is computed
and stored in the final executable program.

## References to Externally Defined Global Variables in Relocatable Object Modules

As noted above, the source file `mod.c` has a reference to the externally
defined global symbol `state` at the first line of its function `modify`. 
The first question we ask is how this is reflected in the relocatable
object file `mod.o` compiled from `mod.c`.

The object file `mod.o` contains among other things a segment with the
code for `modify` (compiled and assembled from the original C source code).
Using the `objdump` tool we dissassemble `mod.o`:

```
>>> objdump -d mod.o | grep 000000 -A5
```

Reading the assembly code of this output, we see that the first
instruction of the code for `modify` is

```
>>> objdump -d mod.o | grep -oE "mov\s+0x0\(%rip\),%eax"
```

which moves a value from a location in memory to the register `%eax`. This
is probably the assembly code corresponding to the first reading of the
variable `state` in `mod.c`. We note the that memory location to read
from is given as `0x0(%rip)`, zero addresses after the PC.

The binary object code corresponding to this assembly code instruction is
a sequence of `6` bytes consisting of the bytes

```
>>> objdump -d mod.o | grep -E "mov\s+0x0\(%rip\),%eax" | grep -oE "([a-f0-9]{2} )+"
```

The four last bytes *probably* indicate the memory location to read from.
We note the offset of this sequence of bytes into the code
for `modify`, and we note that it is a sequence of zero-bytes.

The object module in `mod.o` will be transformed during the process of
creating the final executable object file (`prog`). How does the code
we are looking at here change?

`prog` must also contain code for `modify`. Let's dissassemble `prog` to
find it:

```
>>> objdump -d prog | grep "<modify>:" -A5
```

We see that the `6` bytes long object code segment at the beginning of
`modify` which corresponds to the first move instruction has now changed to

```
>>> objdump -d prog | grep -E "mov\s+0x[a-f0-9]+\(%rip\),%eax" | grep -oE "\s+([a-f0-9]{2} )+"
```

The last four bytes which we claim indicates the memory location to read
from has now changed and the corresponding assembly code is now given as

```
>>> objdump -d prog | grep -oE "mov\s+0x[a-f0-9]+\(%rip\),%eax"
```

If we interpret the new values for the last four bytes of the object code
segment as an 32-bit unsigned integer written with little-endian byte-ordering,
we see that this value is the same as the offset from the PC given in the
corresponding assembly code.

Where did this new value come from? The code was somehow changed by the
linker but what information did the linker use the make this change? It used
the so-called *relocation entries*!

## Relocation Entries

A relocatable object file contains segments with *relocation entries*, data
objects which describe how memory references should be changed during the
linking process. The section `.rela.text` of the relocatable object file
`mod.o` contains the relocation entries for the section `.text` which
contains the machine code compiled from `mod.c`.

The relocation entries in `.rela.text` reference the memory references
they concern using an offset into the `.text` section. By consulting the
symbol table, we can find the offset of `modify` into the `.text` section.
That offset is `0x0`. The memory reference which we are interested in is
at the beginning of `modify` and we can find the exact offset with `objdump`:

```
>>> objdump -d mod.o | grep "<modify>:" -A4
```

The `.rela.text` has a relocation entry for that code location:

```
>>> readelf -r mod.o | grep "Relocation section '.rela.text'" -A1
--
>>> readelf -r mod.o | grep -m 1 state
```

What does the `Info` property mean? The relocation entries that we see here
are instances of the `Elf64_Rela` structure. The `Info` property which we
see in the table is the `r_info` property of this structure. It is a property
of type `Elf64_Xword`, an eight byte long unsigned integer. The high-order 32
bits of this value is the index in the symbol table of the symbol which the 
relocation concerns; the low-order 32 bits is the relocation type.

We see that the symbol table index indeed corresponds the symbol `state`
(an externally defined global symbol from the point of view
of `mod.o`) has in the symbol table of `mod.o`. The value of the relocation
type is `2` which indicates the type `R_X86_64_PC32`. This relocation type
is the relocation type for references which uses 32-bit PC-relative addressing.

It sounds like it could be interesting to know more about the `R_X86_64_PC32`
relocation type. What does PC-relative addressing means? How does the linker
process relocation entries of this type?

## PC-relative addressing

Let's start with answering what PC-relative addressing means: When the CPU
meets an instruction which uses PC-relative addressing for a reference, it
computes the effective address (the place address where it will actually
find the data it is looking for) by adding the value of the PC-relative
address to the current runtime value of the PC. 32-bit PC-relative addressing
just means that the value to be added to the PC is a 32-bit value.

In the instruction, which we have been investigating, the four last bytes
indicate an address. Note that four bytes is 32 bits. These four bytes are
all zero in the relocatable object file `mod.o` but are changed to some value
in the executable object file `prog`. This value is exactly the value used
for PC-relative addressing to compute the effective address. Also, note that
in the assembly code, the address is notated as `0x...(%rip)` - some value
plus the value of the PC.

We now understand what PC-relative addressing is but we still haven't found
out how the value used as the offset from the PC is computed. So next, we want
to answer the questions: How is the 32-bit value in the executable object file
which is used for the PC-relative addressing computed?
How do we find the information in `mod.o` which is needed to compute this
value?

## Updating references

It is important to know that in the relocation step, the linker first decides
on runtime addresses for the sections and symbols defined by the input modules.
For example, before updating the reference to the symbol `state` which we are
investigating, the linker first decides on a runtime address for the `.text`
section of `mod.o` and a runtime address of the symbol `state`.

For the relocation type `R_X86_64_PC32`, the address (that is, the 32-bit
value used as the offset from the PC to find the address of the symbol being
referenced) is computed as follows

```
  (runtime address of symbol) + (addend) - (runtime address of reference)
```

In our case, the addend is `-4`, so the runtime address of the reference minus
the addend will give us the value that the PC will have when executing the
instruction. This means that the value we are computing is actually just

```
  (runtime address of symbol) - (runtime address of PC)
```

which is of course exactly what we need for PC-relative addressing.

How is the addend computed for relocations of type `R_X86_64_PC32`? Is it always
`-4`?

The runtime address of the symbol `state` which the linker decided upon can be
read from the symbol table of the executable object file `prog`:

```
>>> readelf -s prog | grep -E "Symbol table '\.symtab'|state$" -A1 -B1
```

The runtime address is the `Value` property of this table.

The runtime address of the reference is computed by adding the offset from the
relocation entry to the runtime address of the `.text` section of `mod.o`. Can
we see the runtime address of the `.text` section of `mod.o` anywhere? In any
case, we can read the runtime address of the reference by dissassembling the
executable object file `prog`.

```
>>> objdump -d prog | grep "<modify>:" -A4 -B3
```

We can now carry out the computation and we can see that it equals the updated
value.

