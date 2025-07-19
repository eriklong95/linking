# Relocation


In the symbol table for `prog`, the symbol `state` has value `0x4010`.

Effect of relocation: updated addresses

What relocation entries does `mod.o` have?

relocation entry for first reference to `state`

find `.rela.text` segment
inspect first relocation entry
does symbol table index in relocation entry match entry for `state`
in symbol table for `mod.o`?

---

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
Using the `objdump` tool we dissassemble `mod.o`

```
>>> objdump -d mod.o | grep 000000 -A5
```

Reading the assembly code of this output, we find the instruction

```
>>> objdump -d mod.o | grep -oE "mov\s+0x0\(%rip\),%eax"
```

which moves a value from a location in memory to the register `%eax`. This
is probably the assembly code corresponding to the first reading of the
variable `state` in `mod.c`. We note the that memory location to read
from is given as `0x0(%rip)`, zero addresses after the PC.

The binary object code corresponding to this assembly code instruction is
a sequence of `6` bytes starting at offset `0x8` into the code for `modify`
and consisting of the bytes

```
>>> objdump -d mod.o | grep -E "mov\s+0x0\(%rip\),%eax" | grep -oE "([a-f0-9]{2} )+"
```

The four last bytes *probably* indicate the memory location to read from.
We note that this sequence of bytes start at offset `0xa` into the code
for `modify`, and that it is a sequence of zero-bytes.

The object module in `mod.o` will be transformed during the process of
creating the final executable object file (`prog`). How does the code
we are looking at here change?

`prog` must also contain code for `modify`. Let's dissassemble `prog` to
find it:

```
>>> objdump -d prog | grep "<modify>:" -A5
```

We see that the `6` bytes long object code segment starting at offset
`0x8` into the code for `modify` has now changed to

```
>>> objdump -d prog | grep 118a: | grep -oE "\s+([a-f0-9]{2} )+"
```

The last four bytes which we claim indicates the memory location to read
from has now changed and the corresponding assembly code is now given as

```
>>> objdump -d prog | grep 118a: | grep -oE "mov.*%eax"
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

Recall that the memory reference which we are investigating here is located
at offset `0xa` into the code for `modify`. By consulting the symbol table,
we can find the offset of `modify` into the `.text` section. That offset is
`0x0` so the memory reference of interest is located at offset `0xa` into
the `.text` section.

The `.rela.text` has a relocation entry for that code location:

```
>>> readelf -r mod.o | grep 0000000a -B1
```

We can also view the raw data for this relocation entry by dumping the
relevant part of `mod.o`

```
>>> xxd -s 0x1c0 -l 0x90 mod.o
```

What does the `Info` property mean? The relocation entries that we see here
are instances of the `Elf64_Rela` structure. The `Info` property which we
see in the table is the `r_info` property of this structure. It is a property
of type `Elf64_Xword`, an eight byte long unsigned integer. The high-order 32
bits of this value is the index in the symbol table of the symbol which the 
relocation concerns; the low-order 32 bits is the relocation type.

We see that the symbol table index is `6`, while the relocation type is `2`.
The symbol `state` (an externally defined global symbol from the point of view
of `mod.o`) indeed has index `6` in the symbol table of `mod.o`. The value
`2` for the relocation type indicates the type `R_X86_64_PC32`, which is the
relocation type for references which uses 32-bit PC-relative addressing.

It sounds like it could be interesting to know more about the `R_X86_64_PC32`
relocation type. What does PC-relative addressing means? How does the linker
process relocation entries of this type?

Let's start with answering what PC-relative addressing means: When the CPU
meets an instruction which uses PC-relative addressing for a reference, it
computes the effective address (the place address where it will actually
find the data it is looking for) by adding the value of the PC-relative
address to the current runtime value of the PC. 32-bit PC-relative addressing
just means that the value to be added to the PC is a 32-bit value.

If we look at the dissambled code for `modify`, we that for the reference
we are investigating (the one at offset `0xa` into the function's code)
there are 4 bytes (placeholder) for the reference - that 32 bits - and in the
assembly code the reference is notated as an address relative to `%rip` which
is exactly the PC.

How is the value which it is changed to computed?
How do we find the information in `mod.o` which is needed to compute this
value?

Guess: first relocation entry corresponds to first symbol reference

relocation entries are 24 bytes long - dump the first one

our object file has `.rela.text` section

first 8 bytes: offset (always into `.text`)
next 8 bytes: symbol table index (high-order 32 bits of value) and relocation
type (low-order 32 bits of value)
last 8 bytes: addend (what is this value used for)
