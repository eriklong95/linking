# Loading and Linking a Shared Library from a Running App

The program `progr` interprets the argument passed as `arg[1]` (i.e. the first
argument passed by the user) as the name of a function defined in `libvector.so`
and tries to call that function. In order to do that, it must load and link the
shared library and then get a pointer to the function.

This is done via the interface to the dynamic linker defined in `dlfcn.h`.

## Dynamic Linker Interface

to load and link the shared library: `dlopen`

to get pointer to function: `dlsym`

unload shared library. what happens if you don't?


## Different kinds of errors

delete libvector.so - error: cannot open shared object file

pass argument not corresponding to function - error: undefined symbol

## Questions

Where does the loaded code go?
