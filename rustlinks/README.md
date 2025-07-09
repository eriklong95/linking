# rustlinks

This repository serves as a minimal example of how to
link object modules build from Rust source code into a C program.

The code has been written to work on Linux and it will need modifications to work
on other platforms.

The Rust crate `answer` defined in the directory with the same name has the following
section in its Cargo manifest:

```
  [lib]
  crate-type = [ "staticlib", "cdylib" ]
```

Adding the `"staticlib"` crate type has the effect that a *static library* is produced
when we run `cargo build`. A static library is stored as a `.a`-file on Linux systems.


Adding the crate type `"cdylib"` has the effect that a *shared library* is produced when we
run `cargo build`. A shared library is stored as a `.so`-file on Linux systems.

For more information on linkage of Rust crates, see https://doc.rust-lang.org/reference/linkage.html.

## Linking with a static library

The script `build_static_link.sh` links an object file `main.o` compiled from a C source file with
the static library `libanswer.a` produced by a `cargo build` process. The result is a fully linked
executable object file called `prog-static`.

## Dynamic linking with a shared library

The script `build_dynamic_link.sh` produces an executable object file `prog-dynamic` which can be linked with `libanswer.so` at runtime.
