#include <iostream>
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <set>
#include "eval.h"
#include "evalPoly.h"

using namespace std;

struct Term { //   2x^3     -2y     5
    int coef; //   2        -2      5    
    string name;// x        y       a
    int pow;//     3        1       0

    Term(double coef, int exp) : coef(coef), pow(exp) {}
} typedef Term;

set<string> token(string expression,map<string,string,KeySizeComparator<string>> variable);

int check_var(map<string, string, KeySizeComparator<string>> variable);
void parse_expr(string& expr, map<string, string, KeySizeComparator<string>> variable);
void replaceAll(std::string& str, const std::string& from, const std::string& to);

string solution(string,map<string, string, KeySizeComparator<string>>);

string PolynomialToString(const vector<Term>& polynomial,string var);

string validator(string); // Проверка выражения на кол-во переменных
string to_canon(string); // приведение выражения к виду, который можно перемножать
vector<Term> parsePolynomial(string expr,string); // разбирает полином на термы
vector<Term> evalPolynom(string,string);// решение полинома
vector<Term> multiply(vector<Term>,vector<Term>,string); // перемножение 2-х полиномов

// int main()
// {
//     map<string, string, KeySizeComparator<string>> variable;
//     variable["A"] = "C+1";
//     variable["C"] = "x*5";
//     for (const auto& [a, b] : variable)
//         cout << a << "\t" << b << endl;
//     string check = "(3x+2)(4x-1)";

//     cout << solution(check,variable);

//     return 0;
// }

string PolynomialToString(const vector<Term>& polynomial,string var) {
    bool isFirstTerm = true;
    string result = "";
    for (const Term& term : polynomial) {
        if (!isFirstTerm && term.coef > 0)
            result+="+";

        isFirstTerm = false;

        if (term.coef != 1 || term.coef == 0)
            if (term.coef == -1)
                result+='-';
            else
                result += to_string(term.coef);

        if (term.pow != 0)
            result+= var;

        if (term.pow > 1)
            result+= "^" + to_string(term.pow);
    }
    return result;
}

vector<Term> parsePolynomial(string polynomialStr,string var) {
    vector<Term> polynomial;

    // Регулярное выражение для сопоставления членов полинома
    regex termRegex(R"(([+-]?\d*[A-Za-z][A-Za-z0-9]*\^?\d+|[+-]?\d*[A-Za-z][A-Za-z0-9]*|[+-]?\d+))");
    smatch matches;

    // Итератор для поиска членов полинома в строке
    auto termBegin = sregex_iterator(polynomialStr.begin(), polynomialStr.end(), termRegex);
    auto termEnd = sregex_iterator();

    // Итерируемся по найденным членам полинома
    for (auto it = termBegin; it != termEnd; ++it) {
        string termStr = it->str(); // Получаем строковое представление члена полинома

        // Разбиваем строку на коэффициент и степень
        int coefficient = 1; // Коэффициент члена
        int exponent = 0;     // Степень члена

         size_t xPos = termStr.find(var); // Ищем позицию символа 'x'

        // Если 'x' найдено, разбиваем строку на коэффициент и степень
        if (xPos != string::npos) {
            // Получаем коэффициент
            if (xPos > 0 && termStr[xPos - 1] != '+' && termStr[xPos - 1] != '-') {
                coefficient = stoi(termStr.substr(0, xPos)); // Получаем подстроку до 'x' и конвертируем в число
            }

            // Получаем степень
            size_t caretPos = termStr.find('^'); // Ищем позицию символа '^'
            if (caretPos != string::npos) {
                exponent = stoi(termStr.substr(caretPos + 1)); // Получаем подстроку после '^' и конвертируем в число
            }
            else{
                exponent = 1;
            }
        } else {
            // Если 'x' не найдено, это может быть только свободный член
            coefficient = stoi(termStr);
        }

        // Создаем новый член полинома и добавляем его в вектор
        polynomial.push_back(Term(coefficient, exponent));
    }

    return polynomial;
}


vector<Term> evalPolynom(string expr,string var){
    vector<Term> result;

    regex expressionRegex(R"(\(([^()]+)\))");
    auto expressionBegin = sregex_iterator(expr.begin(), expr.end(), expressionRegex);
    auto expressionEnd = sregex_iterator();

    for (auto it = expressionBegin; it != expressionEnd; ++it) {
        string subExpression = it->str(1); // Получаем выражение внутри скобок
        vector<Term> subPolynomial = parsePolynomial(subExpression,var); // Разбираем его на части

        if (result.empty()) {
            result = subPolynomial;
        } else {
            // Умножаем текущий результат на новый полином
            vector<Term> multipliedPolynomial = multiply(result, subPolynomial,var);
            if (!multipliedPolynomial.empty()) {
                result = multipliedPolynomial;
            } else {
                cout << "Результат умножения пустой" << endl;
                return {};
            }
        }
    }

    return result;
}

vector<Term> multiply(vector<Term> one, vector<Term> two, string) {
    vector<Term> result;

    // Умножение каждого члена из первого полинома на каждый член из второго полинома
    for (const Term& term1 : one) {
        for (const Term& term2 : two) {
            double coef = term1.coef * term2.coef;
            int exp = term1.pow + term2.pow;

            // Проверяем, есть ли уже терм с такой степенью в результате
            bool found = false;
            for (Term& term : result) {
                if (term.pow == exp) {
                    term.coef += coef; // Если найден, обновляем коэффициент
                    found = true;
                    break;
                }
            }

            // Если терм с такой степенью не найден, добавляем новый
            if (!found) {
                result.push_back(Term(coef, exp));
            }
        }
    }

    return result;
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

    return expr;
}

string solution(string expr,map<string, string, KeySizeComparator<string>> variable){
    check_var(variable);
    
    parse_expr(expr,variable);


    string var = validator(expr);

    if (var.empty())
        return "Больше одного операнда";
    else if (var == "empty"){
        //cout << expr << endl;
        return expr + " = " + (to_string(eval(expr)));
    }
    
    string result = to_canon(expr);
    
    result =  PolynomialToString(evalPolynom(result,var),var);
    return expr + " = " + result;
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