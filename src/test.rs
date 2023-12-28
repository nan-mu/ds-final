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

#[test]
fn test_2_4() {
    use crate::{Build, Item, PmAndYear};
    use chrono::{Datelike, Local};
    use rusqlite::Connection;
    use std::io::Read;
    use std::io::Write;
    use std::process::{Command, Stdio};
    use std::string::String;

    let conn = Connection::open("res/air_quality.db").expect("Could not open database");
    let mut stmt = conn//随机取100条数据
        .prepare("select * from air_quality where pm10 is not null and pm25 is not null order by random() limit 100;")
        .unwrap();
    let res = stmt
        .query_map([], |row| {
            Ok(Item::new(
                &(row.get::<usize, String>(1).unwrap()),
                &(row.get::<usize, String>(2).unwrap()),
                &(row.get::<usize, String>(3).unwrap()),
                &(row.get::<usize, String>(4).unwrap()),
                PmAndYear::Pm10(
                    row.get::<usize, f64>(5).unwrap(),
                    Local::now()
                        .with_year(row.get::<usize, i32>(6).unwrap())
                        .unwrap(),
                ),
                PmAndYear::Pm10(
                    row.get::<usize, f64>(7).unwrap(),
                    Local::now()
                        .with_year(row.get::<usize, i32>(8).unwrap())
                        .unwrap(),
                ),
            ))
        })
        .unwrap();
    println!("数据准备完毕");
    let res: String = res
        .map(|s| s.unwrap().to_string())
        .collect::<Vec<String>>()
        .join("\n");
    // println!("{}", res);
    let mut target = Command::new("target/debug/2-4");
    let mut program = target
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .unwrap();
    let mut stdin = program.stdin.take().expect("Failed to open stdin");
    let input = std::thread::spawn(move || {
        stdin
            .write_all(res.as_bytes())
            .expect("Failed to write to stdin");
    });
    input.join().unwrap();
    let mut output = program.stdout.take().unwrap();
    let mut buf = String::new();
    output.read_to_string(&mut buf).unwrap();
    println!("{}", buf);
}
