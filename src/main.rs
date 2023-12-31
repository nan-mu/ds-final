use chrono::{DateTime, Datelike, Local};
use rusqlite::Connection;
use std::{
    fmt,
    io::{Read, Write},
    process::{Command, Stdio},
    str::from_utf8,
    string::String,
};

mod test;

fn main() {
    let input = get_data();
    run(Mode::Release, "1-1");
    run(Mode::Release, "2-2");
    run(Mode::Release, "2-3");
    run_with_stdin(Mode::Release, "3-4", input);
}

pub enum PmAndYear {
    Pm25(f64, DateTime<Local>),
    Pm10(f64, DateTime<Local>),
    None,
}

impl fmt::Display for PmAndYear {
    //服了，数据是大坑，有的数据缺少，需要写trait了
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            PmAndYear::None => write!(f, "缺省数据"),
            PmAndYear::Pm25(pm, year) => write!(f, "{}\"{}", pm, year.year()),
            PmAndYear::Pm10(pm, year) => write!(f, "{}\"{}", pm, year.year()),
        }
    }
}

pub struct Item {
    region: String,
    subregion: String,
    country: String,
    city: String,
    pm10: PmAndYear,
    pm25: PmAndYear,
}

impl fmt::Display for Item {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{}\"{}\"{}\"{}\"{}\"{}",
            self.region, self.subregion, self.country, self.city, self.pm10, self.pm25,
        )
    }
}

pub trait Build {
    fn new(
        region: &str,
        subregion: &str,
        country: &str,
        city: &str,
        pm10: PmAndYear,
        pm25: PmAndYear,
    ) -> Self;
}

impl Build for Item {
    fn new(
        region: &str,
        subregion: &str,
        country: &str,
        city: &str,
        pm10: PmAndYear,
        pm25: PmAndYear,
    ) -> Item {
        Item {
            region: region.to_string(),
            subregion: subregion.to_string(),
            country: country.to_string(),
            city: city.to_string(),
            pm10,
            pm25,
        }
    }
}

pub enum Mode {
    Debug,
    Release,
}

#[doc = "如题，不带输入的启动"]
pub fn run(mode: Mode, name: &str) {
    let mut target = Command::new(format!(
        "target/{}/{}",
        match mode {
            Mode::Debug => "debug",
            Mode::Release => "release",
        },
        name
    ));
    let now = Local::now();
    let mut program = target.stdout(Stdio::piped()).spawn().unwrap();
    let mut output = program.stdout.take().unwrap();
    let mut buf = String::new();
    output.read_to_string(&mut buf).unwrap();
    let dur = Local::now() - now;
    println!("正在运行，{name}：\n{buf}");
    println!("{name}耗时：{dur}");
}

#[doc = "如题，带输入的启动"]
fn run_with_stdin(mode: Mode, name: &str, input: String) {
    let mut target = Command::new(format!(
        "target/{}/{}",
        match mode {
            Mode::Debug => "debug",
            Mode::Release => "release",
        },
        name
    ));
    let start = Local::now();
    println!("正在运行，{name}:");
    let mut program = target
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .expect("启动c程序失败");
    let mut stdin = program.stdin.take().expect("打开程序stdin失败");
    let input = std::thread::spawn(move || {
        stdin.write_all(input.as_bytes()).expect("标准输入失败");
    });
    input.join().unwrap();
    let output = program.wait_with_output().expect("打开程序stdout失败");
    let duration = Local::now() - start;
    println!(
        "{}",
        from_utf8(&output.stdout).expect("解析c程序stdout失败")
    );
    println!("3-4耗时：{}", duration);
}

fn get_data() -> String {
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
    println!("3-4数据准备完毕");
    let res: String = res
        .map(|s| s.unwrap().to_string())
        .collect::<Vec<String>>()
        .join("\n");
    println!("{res}");
    res
}
