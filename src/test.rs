#[test]
fn test_command() {
    use std::io::Read;
    use std::io::Write;
    use std::process::{Command, Stdio};
    use std::string::String;
    let mut target = Command::new("target/debug/2-4");
    let mut program = target
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .unwrap();
    let mut stdin = program.stdin.take().expect("Failed to open stdin");
    std::thread::spawn(move || {
        stdin
            .write_all("11$ %^&*1".as_bytes())
            .expect("Failed to write to stdin");
    });

    let mut output = program.stdout.take().unwrap();
    let mut buf = String::new();
    output.read_to_string(&mut buf).unwrap();
    output.read_to_string(&mut buf).unwrap();
    println!("{}", buf);
}
