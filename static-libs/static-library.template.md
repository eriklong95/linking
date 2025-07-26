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

When linking with a static library, the object modules which are actually
used by the program being built are copied into the output executable.
This means that after completing the linking process, the static library
can be deleted and the executable will still run.

In our case, the program uses the object module `addvec.o` but not `multvec.o`
so we expect that the executable will contains a copy of `addvec.o` but not
of `multvec.o`. It could be interesting if we could verify this.


But can we really expect that there is an exact copy of `addvec.o` inside
of `prog`? No, because the memory references in the code for the `addvec`
function has been relocated, for instance. Maybe we should rather investigate
the question: in what way is `addvec.o` present in `prog`? This is of course
a more vague question so we have to make it more precise.

Let's list some questions that we can work with:

### Does the symbol `addcnt` appear in the symbol table of `prog`?

Yes, it does:

```
>>> readelf -s prog | grep -E "Value|addcnt"
```

The section indicated by the `Ndx` property is the `.bss` section. That
section actually has a significant size:

```
>>> readelf -S prog | grep -E "Name|\.bss"
```

Is any information about the `addcnt` symbol stored in that section?

### Does the symbol `multcnt` appear in the symbol table of `prog`?

No.

### Does the code for the function `addvec` appear in `prog`?

If we dissassemble `prog`, we find the `addvec` function:
```
>>> objdump -d prog | grep "<addvec>:" -A18
```

If we compare this to a dissassembly of `addvec.o`, we see that it
is the same code except for a memory reference that has been relocated
and some extra instructions at the end:

```
>>> objdump -d addvec.o
```

In the symbol table of `prog`, we can find the absolute runtime address
of `addvec` as the `Value` property:

```
>>> readelf -s prog | grep -E "Value|addvec$"
```

In the section header table, we can find runtime address of the `.text`
section as well as its offset into the file `prog`:

```
>>> readelf -S prog | grep -E "Name|\.text"
```

Using this information, we can make a hex-dump of `prog` which shows us
the relocated code for `addvec`. We could do the same for the function
`main` from `main.o`. The thing to note here is that the `.text` of `prog`
contains the code for functions from the different object modules which
have been linked together to form it.

### Does the code for the function `multvec` appear in `prog`?

It does not appear in the dissassembly of `prog`.

### Conclusion

We conclude that code from `addvec.o` exists in `prog` (in a slightly
changed form) but since the sections of the different input object
modules have been merged, we cannot find exact copies of them inside the
executable.
