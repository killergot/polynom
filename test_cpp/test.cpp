#include <iostream>
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <set>

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
set<string> token(string expression,map<string,string,KeySizeComparator<string>> variable);

int check_var(map<string, string, KeySizeComparator<string>> variable);
int parse_expr(string expr, map<string, string, KeySizeComparator<string>> variable);

int main()
{
    
    map<string, string, KeySizeComparator<string>> variable;
    variable["A"] = "C+1";
    variable["B"] = "A-y";
    variable["C"] = "D-1";
    variable["D"] = "B-1";
    for (const auto& [a, b] : variable)
        cout << a << "\t" << b << endl;
    string check = "A+B+x+1-5-A+a";

    check_var(variable);
    
    

    return 0;
}


set<string> token(string expression,map<string,string,KeySizeComparator<string>> variable) {
    // Создаем регулярное выражение для разделения строки по арифметическим знакам
    regex regex("[-+*()^]");

    // Создаем вектор для хранения разделенных подстрок
    set<string> tokens;

    // Используем regex_token_iterator для разделения строки
    regex_token_iterator<string::iterator> it(expression.begin(), expression.end(), regex, -1);
    regex_token_iterator<string::iterator> end;

    // Добавляем все подстроки в вектор
    while (it != end) {
        tokens.insert(*it);
        ++it;
    }

    set<string> res = set<string>(tokens);
    for (const string& a : tokens)
        if (variable.count(a) == 0)
            res.erase(a);

    return res;
} 


int check_var(map<string, string, KeySizeComparator<string>> variable){
    map<string,set<string>> var_to_vars; 
    
    for (const auto& [var,mean] : variable){
        var_to_vars[var] = token(mean,variable);
        if (var_to_vars[var].empty())
            var_to_vars.erase(var);
    }

    map<string,set<string>> copy = map<string,set<string>>(var_to_vars);
    for (const auto& [var_copy,sets_copy] : copy){
        if (sets_copy.count(var_copy)){
            cout << var_copy <<  " Recursion is banned!"<< endl;
            break;
        }
        for (auto& [var,sets] : copy){
            if (sets.count(var_copy)){
                sets.erase(var_copy);
                for (string i : sets_copy)
                    sets.insert(i);
            }
        }
    }
    // for (const auto& [a,b] : copy){
    //     cout << a <<':';
    //     for (string c : b)
    //         cout << c << ' ';
    //     cout << ' ';
    // }
    

    return 0;
}