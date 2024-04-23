#define FUNCTIONS_H
#include <string>
#include <map>
using namespace std;

template<typename T>
struct KeySizeComparator {
    bool operator()(const T& lhs, const T& rhs) const {
        if (lhs.size() == rhs.size()) {
            return lhs < rhs; // Если размеры равны, сравниваем по самим ключам
        }
        return lhs.size() > rhs.size(); // Иначе сравниваем по размеру
    }
};

// Объявление функции
int eval(string);

string solution(string,map<string, string, KeySizeComparator<string>>);