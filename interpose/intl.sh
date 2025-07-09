gcc -DLINKTIME -c lmalloc.c
gcc -c int.c
gcc -Wl,--wrap,malloc -Wl,--wrap,free -o intl int.o lmalloc.o

