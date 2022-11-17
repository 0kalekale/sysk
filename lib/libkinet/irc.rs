mod irc {
    use std::net::{TcpListener, TcpStream};
    use std::io::Read;
    use std::io::Write;

    pub fn connect_irc(addr: String) -> std::io::Result<(TcpStream)>  {
	Ok(TcpStream::connect(addr)?)
    }
    pub fn readstream(mut s: TcpStream, b: &mut Vec<u8>) {
	s.read(b);
    }
    pub fn writestream(mut s: TcpStream, b: &mut Vec<u8>) {
	s.write(b);
    }
}

fn main() {
    let conn = irc::connect_irc("irc.rizon.net:6660".to_string()).unwrap();
    let mut bf: Vec<u8> = vec![0; 1024];

    while true {
	let tmp = conn.try_clone().expect("Cannot clone stream");
	irc::readstream(tmp, &mut bf);
	print!("{}", std::str::from_utf8(&bf).unwrap());
    }
}
