/*
    cat: concatinate file/s
    Copyright (C) 2022 0kalekale

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

use std::env;
use std::io;

fn help() {
    print!("cat: concatinate file/s\n");
    print!("sleep [FILE/s]\n");
    print!("uses stdin for input if no args\n\n");
    print!("sysk userspace programs <https://gitlab.com/0kalekale/sysk/bin>\n");
    print!("licensed under AGPL3\n");
}

fn main() -> std::io::Result<()> {
    let mut argv: Vec<String> = env::args().collect();
    let argc = argv.len();

    if argc==1 {
	_ = io::copy(&mut io::stdin().lock(), &mut io::stdout().lock());
    }

    else if argv[1] == "-h" {
	help();
    }
    
    else {
	argv.remove(0);
	for arg in argv {
	    let mut fd = std::fs::File::open(arg)?;
	    _ = io::copy(&mut fd, &mut io::stdout().lock());
	}
    }
    Ok(())
}
