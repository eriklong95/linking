# Static libraries

The target `libvector.a` in the Makefile in this directory is a so-called
*static library* built
from the object files `addvec.o` and `multvec.o` which have been compiled
from the C source files `addvec.c` and `multvec.c` respectively.

A static library is a package containing several object modules. On Linux
systems, a static library is stored in a file format called an *archive*.
These files have the `.a` extension. An archive is merely a concatenation
of relocatable object files plus a little bit of metadata.

When linking with a static library to create an executable (as we do to
create the `prog` target in this directory's Makefile), only the object
modules which are actually needed are copied into the executable.

The smart thing about static libraries is that on the one hand we don't
need to know exactly what object modules are used by our program to write
the build commands (as long as we know a big static library which contains
everything we could possibly need) and on the other hand we don't get
anything unnecessary copied into the executable.

## Questions

Are the object modules stored in `addvec.o` and `multvec.o` part of `libvector.a`?

Is `addvec.o` part of executable? because it is used

Is `multvec.o` NOT part of executable?

Can we delete archive and still run executable?

The function `addvec` defined in `addvec.c` is used by the `main` function
of the program.

The object modules compiled from `addvec.c` and `multvec.c` are packaged
in a static library and stored in the archive `libvector.a`

## Finding object modules inside a static library

Let's try to prove that the file `libvector.a` is nothing but a concatenation
of the object files `addvec.o` and `multvec.o` plus a little bit of metadata.

Using `readelf`, we can display the archive index for `libvector.a`

```
>>> readelf -c libvector.a
```

This index shows us that `libvector.a` contains two object modules, `addvec.o`
and `multvec.o`, and lists the symbols that they define. Also, the archive
index displays the offsets into `libvector.a` where `addvec.o` resp. `multvec.o`
begins.

Dump part of `libvector.a` to get exactly `addvec.o`

Both `addvec.o` and `multvec.o` are there?

## Finding used code in executable

remember that relocations have been applied
