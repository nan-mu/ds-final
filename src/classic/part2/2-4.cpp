/// 由于我上学期没有做到学生数据这道题目，所以这次的数据来自另一门数据库的课设作业。数据源见src/9.world_pm25.db
/// 数据源为csv，但当时为了

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Item {
    std::string region;
    std::string subregion;
    std::string country;
    std::string city;
    struct PmAndYear10 {
        double pm25;
        int year;
    } pm10;
    struct PmAndYear25 {
        double pm10;
        int year;
    } pm25;
};

enum class PmAndYear {
    Pm25,
    Pm10,
    None,
};

vector<string> split(string str, string token) {
    vector<string> result;
    while (str.size()) {
        int index = str.find(token);
        if (index != string::npos) {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)
                result.push_back(str);
        } else {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

int main() {
    string get, tmp;
    getline(cin, get);
    // for (int i = 0; i < 100; i++) {
    //     getline(cin, get);
    //     vector<string> tokens = split(get, ",");
    //     Item item;
    //     item.region = tokens[0];
    //     item.subregion = tokens[1];
    //     item.country = tokens[2];
    //     item.city = tokens[3];
    //     item.pm10.pm25 = std::stod(tokens[4]);
    //     item.pm10.year_month_day = std::chrono::year_month_day(
    //         std::stoi(tokens[5]), std::stoi(tokens[6]),
    //         std::stoi(tokens[7]));
    //     item.pm25.pm25 = std::stod(tokens[8]);
    //     item.pm25.year_month_day = std::chrono::year_month_day(
    //         std::stoi(tokens[9]), std::stoi(tokens[10]),
    //         std::stoi(tokens[11]));
    // }

    cout << get;
}