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

Dumping a segment of `libvector.a` starting at the offset reported for `addvec.o`
in the archive index

```
>>> xxd -s 0x76 -l 0x100 libvector.a
```

and comparing it to a dump of the beginning of `addvec.o`

```
>>> xxd -l 0x100 addvec.o
```

we get the idea that the segment starting `0x3c` bytes after the offset `0x76`

But how much of `libvector.a` should we dump to get `addvec.o`? A chunk equal
to the file size of `addvec.o`, of course! And can get that with `ls -l`

```
>>>  ls -l addvec.o
```

Dumping the appriate segment of `libvector.a` and comparing it to `addvec.o`
shows that indeed there is a segment of `libvector.a` which is equal to
`addvec.o`. The same can be done for `multvec.o`.

We know that the offset for the segment of `libvector.a` concerning `addvec.o`
was `0x76` and that there was a segment with metadata of length `0x3c` bytes
from that offset before the actual data from `addvec.o` started. Similarly,
for `multvec.o` there is a metadata segment of `0x3c` bytes from the offset
of the segment concerning `multvec.o` before the actual data starts.

This means that `libvector.a` starts with a "archive header" of `0x76` bytes
and that it has a metadata section of `0x3c` for each of the object modules
it contains. Comparing file sizes, one finds that `libvector.a` contains
nothing more than these metadata sections and the data from `addvec.o` and
`multvec.o`.

## Finding used code in executable

remember that relocations have been applied
