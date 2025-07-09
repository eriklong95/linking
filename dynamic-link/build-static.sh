gcc -Og -S addvec.c multvec.c
gcc -g -c addvec.s multvec.s
ar rcs libvector.a addvec.o multvec.o
gcc -Og -S main.c
gcc -g -c main.s
gcc -static -o prog-st main.o ./libvector.a
gcc -Og -S main1.c
gcc -g -c main1.s
gcc -static -o prog1-st main1.o ./libvector.a
