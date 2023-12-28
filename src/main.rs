use chrono::{DateTime, Datelike, Local};
use std::fmt;

mod test;
fn main() {
    println!("Hello, world!");
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
            PmAndYear::Pm25(pm, year) => write!(f, "{},{}", pm, year.year()),
            PmAndYear::Pm10(pm, year) => write!(f, "{},{}", pm, year.year()),
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
            "{},{},{},{},{},{}",
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
