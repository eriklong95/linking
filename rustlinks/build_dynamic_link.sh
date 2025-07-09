cargo build --manifest-path answer/Cargo.toml
gcc -o prog-dynamic main.c ./answer/target/debug/libanswer.so
