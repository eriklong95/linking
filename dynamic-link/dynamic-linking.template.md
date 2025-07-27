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
- frequently used library functions, like `printf`, is duplicated
in many executable, waste of memory

shared library linked at loadtime or runtime

single shared library shared among all executables that reference
it. on the other hand, shared library must be present on system

partially linked executable, no code or data is copied from shared
library into executable - only relocation and symbol table info
which allows the linking to happen later

loader notices `.interp`, path to dynamic linker, passes control to
dynamic linker

with loadtime dynamic linking, the executable is changed while in
memory (relocations)

## Goals

find path of dynamic linker in `.interp` section

find relocation and symbol info in partially linking executable

what is the default? how is libc linking into the standard hello-world
program?

## Shared Libraries saves space

Many programs use the function `printf` from the C standard library.
When linking with static libraries, the code for the used library
functions is copied into the executable. This means that if we only
only used static libraries, the code for `printf` would be duplicated
in many executables. That would be a waste of space.

With dynamic linking, the code for the library functions is not copied
into the executable. Instead, only some relocation and symbol table
information is copied which allows the linking process to be finished
after the executable has been loaded by a program known as a *dynamic
linker*.

The target `prog` defined in this directory's `Makefile` is an executable
which can be linked dynamically with the shared library `libvector.so` at
loadtime. The size of the executable is significantly smaller than the
executable for an equivalent program being statically linked. On the other
hand, the file `libvector.so` must be present when running `prog`.

What information about the `addvec` function from `libvector.so` is actually
stored in the executable? How is the code for `addvec` found when it is needed? 
