gcc -DRUNTIME -shared -fpic -o rmalloc.so rmalloc.c -ldl
gcc -g -o intr int.c
