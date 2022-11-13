use std::env;
use std::io;

// TODO: write better docs
fn help() {
    print!("cat: concatinate files\n");
    print!("stdin on argc 1, argv[1..x] files on argc < 1\n");
}

fn main() {
    let argv: Vec<String> = env::args().collect();
    let argc = argv.len();

    if argc==1 {
	io::copy(&mut io::stdin().lock(), &mut io::stdout().lock());
    }
}
