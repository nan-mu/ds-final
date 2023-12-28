/// 由于我上学期没有做到学生数据这道题目，所以这次的数据来自另一门数据库的课设作业。数据源见src/9.world_pm25.db
/// 数据源为csv，但当时为了

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Item {
    string region;
    string subregion;
    string country;
    string city;
    struct PmAndYear10 {
        double pm10;
        int year;
    } pm10;
    struct PmAndYear25 {
        double pm25;
        int year;
    } pm25;
};

class HashTable {
  public:
    HashTable(int size) {
        table_size = size;
        table = new Item*[table_size];
        for (int i = 0; i < table_size; i++) {
            table[i] = nullptr;
        }
    }

    // 哈希函数
    int hash(int key) { return key % table_size; }

    // 插入元素
    void insert(Item* node) {
        int index = hash(node->pm10.pm10);
        table[index] = node;
    }

    // 查找元素
    int find(double key) {
        int index = hash(key);
        int count = 0; //记录查找跳数
        Item* node = table[index];
        while (node != nullptr) {
            count++;
            if (node->pm10.pm10 == key) {
                cout << "查找" << key << "成功，查找跳数为" << count << endl;
                return index;
            }
            node = table[++index];
        }
        return -1;
    }

    ~HashTable() {
        for (int i = 0; i < table_size; i++) {
            Item* node = table[i];
            if (node != nullptr) {
                cout << node->region << " " << node->subregion << " "
                     << node->country << " " << node->city << " "
                     << node->pm10.pm10 << " " << node->pm10.year << " "
                     << node->pm25.pm25 << " " << node->pm25.year << endl;
                delete node;
            }
        }
        delete[] table;
    }

  private:
    int table_size;
    Item** table;
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
    HashTable table(100);
    for (int i = 0; i < 100; i++) {
        getline(cin, get);
        //样例：0WprHI,1WprB,2Republic of
        // Korea,3Gunsan,453,52010,624.33262775,72010
        vector<string> tokens = split(get, "\"");
        Item* item = (Item*)malloc(sizeof(Item));
        item->region = tokens[0];
        item->subregion = tokens[1];
        item->country = tokens[2];
        item->city = tokens[3];
        item->pm10.year = std::stoi(tokens[5]);
        item->pm10.pm10 = std::stod(tokens[4]);
        item->pm25.pm25 = std::stod(tokens[6]);
        item->pm25.year = std::stoi(tokens[7]);
        table.insert(item);
    }

    cout << "得到所有" << endl;
}