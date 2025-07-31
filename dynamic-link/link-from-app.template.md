# Loading and Linking a Shared Library from a Running App

The program `progr` interprets the argument passed as `arg[1]` (i.e. the first
argument passed by the user) as the name of a function defined in `libvector.so`
and tries to call that function. In order to do that, it must load and link the
shared library and then get a pointer to the function.

This is done via the interface to the dynamic linker defined in `dlfcn.h`.

## Dynamic Linker Interface

The dynamic linker interface defined in `dlfcn.h` exposes four functions that
user programs can call to load and link shared libraries and call functions
defined in them.

These four functions are: `dlopen`, `dlsym`, `dlclose` and `dlerror`.

The function `dlopen` is called to load and link a shared library. The path to
the library is provided as an argument. A pointer is returned which can be passed
to `dlsym` when we want get a symbol from the library (e.g. a function). `dlsym`
needs this pointer (aka a handle) and the name of the symbol we want. The handle
is also needed when we want to unload the shared library.

What information is actually stored in the handle?

What happens if you don't unload the shared library?

Finally, the function `dlerror` function is used to retrieve descriptions of errors
in calls to the other functions. It returns the error message of the most recent
error. This can only work if some state is kept. How does this work if the functions
are called from multiple places concurrently?

## Different kinds of errors

We can try to trigger different kinds of errors from the dynamic linker interface.

First, we expect an error if the shared library which is requested is not present.
To trigger this error, we delete `libvector.so` and then run `./progr`.

```
  ./libvector.so: cannot open shared object file: No such file or directory
```

Second, we could try to pass in an invalid symbol name. If we run `./progr blah`,
we will be asking for a pointer to a symbol named `blah` defined in `libvector.so`
but no such symbol exists. If we try, we get the error

```
  ./libvector.so: undefined symbol: blah  
```

## Questions

Where does the loaded code go?

What errors can you get from unloading a shared library?
