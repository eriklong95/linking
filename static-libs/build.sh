gcc -Og -c addvec.c multvec.c
ar rcs libvector.a addvec.o multvec.o
gcc -Og -c main.c
gcc -static -o prog main.o ./libvector.a
