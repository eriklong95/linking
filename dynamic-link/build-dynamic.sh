gcc -Og -S -fpic addvec.c multvec.c
gcc -g -fpic -c addvec.s multvec.s
gcc -shared -fpic -o libvector.so addvec.o multvec.o
gcc -Og -S main.c
gcc -g -c main.s
gcc -g -o prog-dyn main.o ./libvector.so
gcc -Og -S main1.c
gcc -g -c main1.s
gcc -g -o prog1-dyn main1.o ./libvector.so
