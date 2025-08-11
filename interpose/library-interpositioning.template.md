# Library Interpositioning

*Library interpositioning* means altering a program to the effect that
calls to library functions are intercepted and replaced with custom
implementations. Library interpositioning can be done at compile-time,
link-time or run-time.

For each of the different interpositioning methods, how is the program
changed to make the interception and redirection of library function
calls happen? At what steps in the build process are these changes made?

## Standard program

The example program which we will be using for this demonstration is built
from the following C source file:

```
>>> cat int.c
```

This program calls the functions `malloc` and `free` from the C standard
library. These two functions play the role of target functions which we
will be *interposing on*. So, this is the terminology: to interpose on some
library function means doing something to replace calls to that function
with a custom implementation. In our case, we want to replace calls to
`malloc` and `free` with custom implementations. More specifically, we will
create some *wrapper functions* which will print a trace to the standard
output and then call the real `malloc` or `free`.

The program calls `malloc` and receives a pointer to some allocated memory.
It then does something with the pointer. The purpose of this is to prevent
the calls to `malloc` and `free` from being optimized away.

This C source file will be our starting point for all the different library
interpositioning methods which we will explore. The differences between these
methods lie in at what point in the built process, we must alter the program.

The target `int` from the `Makefile` is the program built in the standard way
without any interpositioning. This program is built by first compiling `int.s`
from `int.c`, then assembling this to `int.o` and finally building the executable.
Comparing these intermediate artifacts to the artifacts created by the build
processes which use interpositioning will help us understand how interpositioning
works.

## Compile-time interpositioning

Interpositioning can be achieved by changing the way the source file is compiled.

The file `cmalloc.c` contains custom implementations of `malloc` and `free`. We
want to change the way we build an executable from `int.c` such that it calls
our custom implementations instead of the ones from the C standard library.

To achieve this, we pass the option `-include malloc.h` to the command which
compiles assembly code from `int.c` thus producing the alternative assembly code
file `intc.s`. The header file `malloc.h` redefines `malloc(size)` to mean a call
to our alternative implementation.

Comparing the assembly code file `intc.s` to the file `int.s` produced by the
standard compilation process, we see that the names of our custom implementations
now appear in the assembly code for the main function:

```
>>> cat intc.s | grep "main:" -A10
```

Put differently, the assembly code is tightly coupled to our custom implementations.

To achieve this kind of interpositioning, we must have access to the source code
because we must control the compilation process. What if we only have the object
module `int.o`? Then we can do the interpositioning at link-time.

## Link-time interpositioning

pass options to static linker

we use standard object file `int.o` as input to linker

what does the executable look like? Any traces of custom impls

## Run-time interpositioning

uses standard program

why do we need to use `dlsym`?
