#include <iostream>
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <set>
#include "eval.h"


using namespace std;

struct term { //   2x^3     -2y     5
    int coef; //   2        -2      5    
    string name;// x        y       a
    int pow;//     3        1       0
} typedef term;


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
void parse_expr(string& expr, map<string, string, KeySizeComparator<string>> variable);
void replaceAll(std::string& str, const std::string& from, const std::string& to);

string solution(string);

string validator(string);
string to_canon(string);
string find_multiply(string);
string multiply();

int main()
{
    map<string, string, KeySizeComparator<string>> variable;
    variable["A"] = "C+1";
    variable["C"] = "x*5";
    for (const auto& [a, b] : variable)
        cout << a << "\t" << b << endl;
    string check = "A*A";
    cout << "Before : " + check + '\n';

    check_var(variable);
    
    parse_expr(check,variable);
    cout <<"After: " + check << endl;

    cout << solution(check);

    return 0;
}

string validator(string expr){
    std::regex pattern("[A-Za-z][A-Za-z1-9]*"); 
    // Создаем итераторы для поиска всех совпадений
    std::sregex_iterator next(expr.begin(), expr.end(), pattern);
    std::sregex_iterator end;

    set<string> temp;

    while (next != end) {
        std::smatch match = *next;
        temp.insert(match.str());
        next++;
    }
    if (temp.empty())
        return "empty";
    if (temp.size() > 1)
        return "";
    else
        for (string i : temp)   // Не знаю как забрать просто этот 1 ебучий элемент
            return i;
} 

string to_canon(string expr){

}

string solution(string expr){
    string var = validator(expr);
    if (var.empty())
        return "Больше одного операнда";
    else if (var == "empty")
        return (to_string(eval(expr)));
    else{
        return "okey";
    }
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Перемещаем позицию, чтобы избежать зацикливания
    }
}

void parse_expr(string& expr, map<string, string, KeySizeComparator<string>> variable){
    bool check = true;

    while (check){
        set<string> temp = token(expr,variable);
        if (!temp.empty()){
            for (string i : temp){
                replaceAll(expr,i,'(' + variable[i] + ')');
            }
        }
        else
            check = false;
    }
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