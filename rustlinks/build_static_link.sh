cargo build --manifest-path answer/Cargo.toml
gcc -c main.c
gcc -static -o prog-static main.o ./answer/target/debug/libanswer.a

