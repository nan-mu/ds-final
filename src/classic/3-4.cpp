/// 由于我上学期没有做到学生数据这道题目，所以这次的数据来自另一门数据库的课设作业。数据源见src/9.world_pm25.db
/// 数据源为csv，但当时为了

#include <algorithm>
#include <iostream>
#include <random>
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
    struct recode {       //用来记录查找次数
        vector<int> hash; //记录正数为查找成功，负数为失败
        vector<int> half;
    } recode;

    // 哈希函数
    int hash(int key) { return key % table_size; }

    void insert(Item* node) { // 插入元素，也算一种排序
        int index = hash((int)node->pm10.pm10);
        node->key = index;
        bool flag = true;
        for (; index < table_size && flag; index++) {
            if (table[index] == nullptr)
                break;
            if (index == table_size - 1) {
                index = 0;
                flag = true;
            }
        }
        table[index] = node;
    }

    void quick_sort() { quick_sort(0, table_size - 1); }
    void quick_sort(int left, int right) { //快速排序
        // 递归终止条件
        if (left >= right) {
            return;
        }

        // 选择基准元素
        Item* pivot = table[left];

        // 将数组划分为两个子数组
        int i = left + 1;
        int j = right;
        while (i <= j) {
            // 从左边找到第一个大于基准元素的元素
            while (i <= right && table[i]->pm25.pm25 <= pivot->pm25.pm25) {
                i++;
            }

            // 从右边找到第一个小于基准元素的元素
            while (j >= left && table[j]->pm25.pm25 >= pivot->pm25.pm25) {
                j--;
            }

            // 交换两个元素
            if (i <= j) {
                Item* temp = table[i];
                table[i] = table[j];
                table[j] = temp;
                i++;
                j--;
            }
        }

        // 递归对两个子数组进行排序
        quick_sort(left, j);
        quick_sort(i, right);
    }

    // 查找元素
    int find(int key) {
        int index = hash(key);
        int count = 1; //记录查找跳数
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

    //折半查找
    int find_half(int key) {
        cout << "来了，key" << key << endl;
        int low = 0, high = table_size - 1, mid, mod, count = 0;
        while (low <= high) {
            count++;
            mid = (low + high) / 2;
            if (table[mid] == nullptr)
                mod = mid;
            else
                mod = table[mid]->key;
            if (mod < key) {
                low = mid + 1;
            } else if (mod > key) {
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

int main() {
    string get, tmp;
    HashTable table(100);
    for (int i = 0; i < 100; i++) { //可以认为哈希查找已经进行了一次排序
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
    int min = 0, max = 100,
        temp = -1;      //设定随机生成数组的大小，随机数上下限
    random_device seed; //硬件生成随机数种子
    ranlux48 engine(seed()); //利用种子生成随机数引擎
    uniform_int_distribution<> distrib(min,
                                       max); //设置随机数范围，并为均匀分布

    for (int index = 0; index < 100; index++) {
        int target = distrib(engine);
        temp = table.find_half(target);
        table.find(target);
        target = distrib(engine);
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
    for (auto index : table.recode.half) {
        if (index > 0) {
            ASL_half_s += index;
        } else {
            ASL_half_f -= index;
        }
    }

    cout << "查找100次统计平均查找速度，哈希查找，成功："
         << ASL_hash_s / success_count << "失败："
         << ASL_hash_f / (100 - success_count) << endl;
    cout << "查找100次统计平均查找速度，折半查找，成功："
         << ASL_half_s / success_count << "失败："
         << ASL_half_f / (100 - success_count) << endl;

    //来进行第二种排序，然后会打印前三位pm2.5浓度最高的城市，之前是拿pm10来摆的，所以确实会有不同
    table.quick_sort();
    for (int i = 0; i < 3; i++) {
        cout << table.table[i]->city << " " << table.table[i]->pm25.pm25
             << endl;
    }
}