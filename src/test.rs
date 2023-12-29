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
fn test_3_4() {
    use crate::{Build, Item, PmAndYear};
    use chrono::{Datelike, Local};
    use rusqlite::Connection;
    use std::io::Write;
    use std::process::{Command, Stdio};
    use std::str::from_utf8;
    use std::string::String;

    let conn = Connection::open("res/air_quality.db")
        .expect("数据库文件丢失，在./res/下应该有一个air_quality.db");
    let mut stmt = conn//随机取100条数据
        .prepare("select * from air_quality where pm10 is not null and pm25 is not null order by random() limit 100;")
        .expect("构建查询语句失败");
    let res = stmt
        .query_map([], |row| {
            Ok(Item::new(
                &(row.get::<usize, String>(1).expect("拆解地区数据失败")),
                &(row.get::<usize, String>(2).expect("拆解子地区数据失败")),
                &(row.get::<usize, String>(3).expect("拆解国家数据失败")),
                &(row.get::<usize, String>(4).expect("拆解城市数据失败")),
                PmAndYear::Pm10(
                    row.get::<usize, f64>(5).expect("拆解PM10数据失败"),
                    Local::now()
                        .with_year(row.get::<usize, i32>(6).expect("拆解PM10年份数据失败"))
                        .expect("解析PM10年份数据失败"),
                ),
                PmAndYear::Pm10(
                    row.get::<usize, f64>(7).expect("拆解PM2.5数据失败"),
                    Local::now()
                        .with_year(row.get::<usize, i32>(8).expect("拆解PM2.5年份数据失败)"))
                        .unwrap(),
                ),
            ))
        })
        .expect("迭代数据库回显失败");
    println!("数据准备完毕");
    let res: String = res
        .map(|s| s.unwrap().to_string())
        .collect::<Vec<String>>()
        .join("\n");
    // println!("{}", res); //调试查看数据库输出

    let mut target = Command::new("target/debug/3-4");
    let mut program = target
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .expect("启动c程序失败");
    let mut stdin = program.stdin.take().expect("打开程序stdin失败");
    let input = std::thread::spawn(move || {
        stdin.write_all(res.as_bytes()).expect("标准输入失败");
    });
    input.join().unwrap();
    let output = program.wait_with_output().expect("打开程序stdout失败");
    println!(
        "{}",
        from_utf8(&output.stdout).expect("解析c程序stdout失败")
    );
}
