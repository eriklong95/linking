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

assembly code is different, int.s vs intc.s

## Link-time interpositioning

pass options to static linker

we use standard object file `int.o` as input to linker

what does the executable look like? Any traces of custom impls

## Run-time interpositioning

uses standard program

why do we need to use `dlsym`?
