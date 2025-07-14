# Relocation

How is the symbol `state` which is referenced in `mod.c` "found"?

In the symbol table for `prog`, the symbol `state` has value `0x4010`.

Effect of relocation: updated addresses

What relocation entries does `mod.o` have?

relocation entry for first reference to `state`

find `.rela.text` segment
inspect first relocation entry
does symbol table index in relocation entry match entry for `state`
in symbol table for `mod.o`?


The source file `mod.c` has a reference to the externally defined global
symbol `state` at the first line of its function `modify`. Where is this
reference in the relocatable object file `mod.o`? Using the `objdump` tool
to dissassemble `mod.o`, we see that there is a memory reference at `0xa`
bytes into the code for `modify` function which could very well be that
reference to `state`:

```
>>> objdump -d mod.o | grep 000000 -A5
```

Dumping the code of the output executable `prog`, we see that this memory
reference has been changed.

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
