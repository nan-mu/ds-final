/// 由于我上学期没有做到学生数据这道题目，所以这次的数据来自另一门数据库的课设作业。数据源见src/9.world_pm25.db
/// 数据源为csv，但当时为了

#include <algorithm>
#include <iostream>
#include <random>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

struct Item {
    string region;
    string subregion;
    string country;
    string city;
    int key;
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
    struct recode {       // 用来记录查找次数
        vector<int> hash; // 记录正数为查找成功，负数为失败
        vector<int> half;
    } recode;

    // 哈希函数
    int hash(int key) { return key % table_size; }

    void insert(Item* node) { // 插入元素，也算一种排序
        int index = hash((int)node->pm10.pm10);
        node->key = index;
        bool flag = true;
        for (; index < table_size || flag; index++) {
            if (table[index] == nullptr)
                break;
            if (index == table_size - 1) {
                index = -1;
                flag = false;
            }
        }
        table[index] = node;
    }

    // 查找元素
    int find(int key) {
        int index = hash(key);
        int count = 1; // 记录查找跳数
        Item* node = table[index];
        while (node != nullptr) {
            if (node->key == key) {
                recode.hash.push_back(count);
                cout << "哈希查找" << key << "成功，跳数为" << count << endl;
                cout << node->region << " " << node->subregion << " "
                     << node->country << " " << node->city << " "
                     << node->pm10.pm10 << " " << node->pm10.year << " "
                     << node->pm25.pm25 << " " << node->pm25.year << endl;
                return index;
            }
            count++;
            node = table[++index];
        }
        recode.hash.push_back(0 - count);
        cout << "哈希查找" << key << "失败，跳数为" << count << endl;
        return -1;
    }

    // 折半查找
    int find_half(int key) {
        cout << "来了，key" << key << endl;
        int low = 0, high = table_size - 1, mid, mod, count = 0;
        while (low <= high) {
            count++;
            mid = (low + high) / 2;
            if ((int)table[mid]->pm25.pm25 < key) {
                low = mid + 1;
            } else if ((int)table[mid]->pm25.pm25 > key) {
                high = mid - 1;
            } else {
                if (table[mid] != nullptr) {
                    recode.half.push_back(count);
                    cout << "折半查找" << key << "成功，跳数为" << count
                         << endl;
                    cout << table[mid]->region << " " << table[mid]->subregion
                         << " " << table[mid]->country << " "
                         << table[mid]->city << " " << table[mid]->pm10.pm10
                         << " " << table[mid]->pm10.year << " "
                         << table[mid]->pm25.pm25 << " "
                         << table[mid]->pm25.year << endl;
                    return mid;
                } else {
                    recode.half.push_back(0 - count);
                    cout << "折半查找" << key << "失败，跳数为" << count
                         << endl;
                    return -1;
                }
            }
        }
        recode.half.push_back(0 - count);
        cout << "折半查找" << key << "失败，跳数为" << count << endl;
        // 没有找到元素
        return -1;
    }

    ~HashTable() {
        for (int i = 0; i < table_size; i++) {
            Item* node = table[i];
            if (node != nullptr) {
                // 看看数据是否真的进来了
                // cout << node->region << " " << node->subregion << " "
                //      << node->country << " " << node->city << " "
                //      << node->pm10.pm10 << " " << node->pm10.year << " "
                //      << node->pm25.pm25 << " " << node->pm25.year << endl;
                delete node;
            }
        }
        delete[] table;
    }

    int table_size;
    Item** table;
};

vector<string> split(string str, string token) { // c语言没有sqlit是真奇怪
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

struct Range {
    int start, end;

    Range(int s = 0, int e = 0) { start = s, end = e; }
};

void quick_sort(Item** arr, const int len) { // 快速排序，换成了非递归的写法
    if (len <= 0)
        return;
    Range r[100];
    int p = 0;
    r[p++] = Range(0, len - 1);
    while (p) {
        Range range = r[--p];
        if (range.start >= range.end)
            continue;
        double mid = arr[range.end]->pm25.pm25;
        int left = range.start, right = range.end - 1;
        while (left < right) {
            while (arr[left]->pm25.pm25 < mid && left < right)
                left++;
            while (arr[right]->pm25.pm25 >= mid && left < right)
                right--;
            std::swap(arr[left], arr[right]);
        }
        if (arr[left] >= arr[range.end])
            std::swap(arr[left], arr[range.end]);
        else
            left++;
        r[p++] = Range(range.start, left - 1);
        r[p++] = Range(left + 1, range.end);
    }
}

int main() {
    HashTable table(100);
    for (int i = 0; i < 100; i++) { // 可以认为哈希查找已经进行了一次排序
        string get = "";
        getline(cin, get);
        // 样例：0WprHI,1WprB,2Republic of
        //  Korea,3Gunsan,453,52010,624.33262775,72010
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
    int min = 0, max = 100; // 设定随机生成数组的大小，随机数上下限
    random_device seed;      // 硬件生成随机数种子
    ranlux48 engine(seed()); // 利用种子生成随机数引擎
    uniform_int_distribution<> distrib(min,
                                       max); // 设置随机数范围，并为均匀分布

    for (int index = 0; index < 100; index++) {
        int target = distrib(engine);
        table.find(target);
    }
    double ASL_hash_s = 0, ASL_hash_f = 0, ASL_half_s = 0, ASL_half_f = 0,
           success_count = 0;
    for (auto index : table.recode.hash) {
        if (index > 0) {
            ASL_hash_s += index;
            success_count++;
        } else {
            ASL_hash_f -= index;
        }
    }

    cout << "查找100次统计平均查找速度，哈希查找，成功："
         << ASL_hash_s / success_count << "失败："
         << ASL_hash_f / (100 - success_count) << endl;

    cout << "排序之前：" << endl;
    for (int i = 0; i < 3; i++) {
        cout << table.table[i]->city << " " << table.table[i]->pm25.pm25
             << endl;
    }
    cout << "排序之后：" << endl;
    // 来进行第二种排序，然后会打印前三位pm2.5浓度最低的城市，之前是拿pm10来摆的，所以确实会有不同
    quick_sort(table.table, 100);
    for (int i = 0; i < 3; i++) {
        cout << table.table[i]->city << " " << table.table[i]->pm25.pm25
             << endl;
    }
    for (int index = 0; index < 100; index++) { // 启动折半查找
        int target = distrib(engine);
        table.find_half(target);
    }
    success_count = 0;
    for (auto index : table.recode.half) { // 统计查找跳数
        if (index > 0) {
            ASL_half_s += index;
            success_count++;
        } else {
            ASL_half_f -= index;
        }
    }
    cout << "查找100次统计平均查找速度，折半查找，成功："
         << ASL_half_s / success_count << "失败："
         << ASL_half_f / ((double)100 - success_count) << endl;
}