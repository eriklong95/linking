# Dynamic Linking

## Questions

How is *dynamic linking* different from "usual" linking?

What is the output of a dynamic linking process and what
information is created?

What are the advantages of dynamic linking? Are there any
downsides?

With dynamic linking, shared libraries can be used as input.
What is *shared* about a shared library? Who is sharing it?

At what point should the references to a shared library be
supplied? At linktime, at loadtime or at runtime?

When doing dynamic linking with a shared library, are any
parts of or any information from the shared library stored
in the executable?

## Notes

disadvantages of static library

- relink needed when static library is updated

loader notices `.interp`, path to dynamic linker, passes control to
dynamic linker

with loadtime dynamic linking, the executable is changed while in
memory (relocations)

## Goals

find path of dynamic linker in `.interp` section

find relocation and symbol info in partially linked executable

what is the default? how is libc linking into the standard hello-world
program?

## Shared Libraries Saves Space

Many programs use the function `printf` from the C standard library.
When linking with static libraries, the code for the used library
functions is copied into the executable. This means that if we only
only used static libraries, the code for `printf` would be duplicated
in many executables. That would be a waste of space.

With dynamic linking, the code for the library functions is not copied
into the executable. Instead, only some relocation and symbol table
information is copied which allows the linking process to be finished
at loadtime by a program known as a *dynamic
linker*.

In this way, even though many programs use code from a shared library,
the code only needs to exist in one place in the file system.

## Partially Linked Executable

The target `prog` defined in this directory's `Makefile` is an executable
which can be linked dynamically with the shared library `libvector.so` at
loadtime. The size of the executable is significantly smaller than the
executable for an equivalent program being statically linked. On the other
hand, the file `libvector.so` must be present when running `prog`.

If the code, we need from `libvector.so` is not copyied into `prog`, then
what traces of `libvector.so` can we actually find in `prog`?

The executable `prog` is built with the command

```
  gcc -o prog main.o ./libvector.so
```

so `libvector.so` is an input to the usual linker (`ld`). This indicates
that some information from `libvector.so` is stored in `prog`.

The code we use
from `libvector.so` is not copied into `prog` - we don't see the complete
code for `addvec` in the output from `objdump -d prog`. But we do see some
references to `addvec` in that output and to be able to call `addvec` the
executable will need some information to be able to find the address of 
`addvec` at runtime. How does this work?

## Finding a function defined in a shared library

What information about the `addvec` function from `libvector.so` is actually
stored in the executable? How is the code for `addvec` found when it is needed?

Three parts of the executable comes to mind when thinking about these questions.
First, the code sections of the executable must contain some information about
`addvec` at the points where this function is called from the main flow. Second,
`addvec` is a symbol referenced in the source code for the executable so might
very well be a symbol table entry for it. Finally, as `addvec` is defined in a
shared library and not in the executable itself, its address is not known before
loadtime, so there could be some relocation entries for it which can be used by
the dynamic linker.

At the point in `main` where `addvec`
is called, we see the line

```
>>> objdump -d prog | grep -E "call\s+[0-9]+ <addvec@plt>"  
```

If we go the address which is passed as argument to the
`call` instruction, we find the following code:

```
>>> objdump -d prog | grep "<addvec@plt>:" -A3
```

This code clearly has to do with `addvec` but it is not its implementation.
What is this code? What happens when it is executed? Well, it looks like we
jump to the address stored at location

```
>>> objdump -d prog | grep "<addvec@plt>:" -A3 | grep -oE "0x[a-f0-9]+\(%rip\)"
```

This location is actually the entry at index `3` in the *global offset table*,
GOT[3] for short. The entries in the GOT are 8 bytes long, and we dump the GOT
from the executable `prog`, we see the value stored in this entry in the file:

```
>>> readelf -x .got prog
```

We see that the address stored at `GOT[3]` in the file `prog` is an address in the
`.plt` section. Executing the code from that address has the effect of pushing the
value `0x0` as well as the address of `GOT[1]` and then jumping to the address
stored at `GOT[2]`. What is the point in that?

The interesting thing is that if we debug with `gdb` and stand at the point where
`addvec` is about to be called from `main`, the value in `GOT[3]` is actually
different: At that point, the GOT entry holds the runtime address of `addvec`.
When does that change?. See when we change to real address of `addvec`

The symbol tables have the following entries for `addvec`:

```
>>> readelf -s prog | grep -E "Value|addvec" -B2
```

The thing to note here is that `addvec` is declared as an undefined, global
symbol.

With the dynamic linking approach, there is a relocation entry for `addvec`.
That was not the case with static linking.

```
>>> readelf -r prog | grep ".rela.plt" -A2
```

We note that the `Offset` property is exactly `GOT[3]`, the value for the
indirect jump in `addvec@plt`.

What does the relocation type `R_X86_64_JUMP_SLO` mean?



