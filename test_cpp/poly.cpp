#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <cmath>

using namespace std;

// Структура для представления одночлена
struct Term {
    double coefficient; // Коэффициент
    int exponent;       // Степень

    // Конструктор для создания одночлена
    Term(double coef, int exp) : coefficient(coef), exponent(exp) {}
};


// Функция для разбора строки с полиномом и создания вектора одночленов
vector<Term> parsePolynomial(const string& polynomialStr) {
    vector<Term> polynomial;

    // Регулярное выражение для сопоставления членов полинома
    regex termRegex(R"(([-+]?\d*\.?\d*)?x\^?(\d+)?)");
    smatch matches;

    // Итератор для поиска членов полинома в строке
    auto termBegin = sregex_iterator(polynomialStr.begin(), polynomialStr.end(), termRegex);
    auto termEnd = sregex_iterator();

    for (auto it = termBegin; it != termEnd; ++it) {
        double coefficient = 1.0;
        int exponent = 0;

        if ((*it)[1].matched && !(*it)[1].str().empty() && (*it)[1] != "x")
            //coefficient = stod((*it)[1]);
            coefficient = 1;

        if ((*it)[2].matched)
            exponent = stoi((*it)[2]);

        polynomial.push_back(Term(coefficient, exponent));
    }

    return polynomial;
}


// Функция для умножения двух полиномов
vector<Term> multiplyPolynomials(const vector<Term>& poly1, const vector<Term>& poly2) {
    vector<Term> result;

    // Умножение каждого члена из первого полинома на каждый член из второго полинома
    for (const Term& term1 : poly1) {
        for (const Term& term2 : poly2) {
            double coef = term1.coefficient * term2.coefficient;
            int exp = term1.exponent + term2.exponent;
            result.push_back(Term(coef, exp));
        }
    }

    return result;
}

// Функция для сложения двух полиномов
vector<Term> addPolynomials(const vector<Term>& poly1, const vector<Term>& poly2) {
    vector<Term> result;
    int index1 = 0, index2 = 0;

    // Объединение полиномов, пока не закончится один из них
    while (index1 < poly1.size() && index2 < poly2.size()) {
        if (poly1[index1].exponent > poly2[index2].exponent) {
            result.push_back(poly1[index1]);
            index1++;
        } else if (poly1[index1].exponent < poly2[index2].exponent) {
            result.push_back(poly2[index2]);
            index2++;
        } else {
            double coef = poly1[index1].coefficient + poly2[index2].coefficient;
            if (std::fabs(coef) > 0.00001) // Добавить только ненулевые коэффициенты
                result.push_back(Term(coef, poly1[index1].exponent));
            index1++;
            index2++;
        }
    }

    // Добавление оставшихся членов, если есть
    for (; index1 < poly1.size(); index1++)
        result.push_back(poly1[index1]);
    for (; index2 < poly2.size(); index2++)
        result.push_back(poly2[index2]);

    return result;
}

// Функция для вычисления полинома, заданного в виде строки
vector<Term> evaluatePolynomial(const string& polynomialStr) {
    vector<Term> result;

    // Регулярное выражение для поиска полиномов внутри скобок
    regex expressionRegex(R"(\(([^()]+)\))");

    // Итератор для поиска выражений в скобках
    auto expressionBegin = sregex_iterator(polynomialStr.begin(), polynomialStr.end(), expressionRegex);
    auto expressionEnd = sregex_iterator();

    for (auto it = expressionBegin; it != expressionEnd; ++it) {
        string subExpression = it->str(1); // Получаем выражение внутри скобок
        vector<Term> subPolynomial = parsePolynomial(subExpression); // Разбираем его на части

        if (result.empty()) {
            result = subPolynomial;
        } else {
            // Умножаем текущий результат на новый полином
            vector<Term> multipliedPolynomial = multiplyPolynomials(result, subPolynomial);
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

// Функция для вывода полинома
void printPolynomial(const vector<Term>& polynomial) {
    bool isFirstTerm = true;

    for (const Term& term : polynomial) {
        if (!isFirstTerm && term.coefficient > 0)
            cout << " + ";
        else if (term.coefficient < 0)
            cout << " - ";

        isFirstTerm = false;

        if (std::fabs(term.coefficient) != 1 || term.exponent == 0)
            cout << std::fabs(term.coefficient);

        if (term.exponent > 0)
            cout << "x";

        if (term.exponent > 1)
            cout << "^" << term.exponent;
    }

    cout << endl;
}

int main() {
    string polynomialStr = "((2+x)+4)*(x^2+5)*(x*3+4)";

    // Вычисляем полином
    vector<Term> result = evaluatePolynomial(polynomialStr);

    cout << "Результат умножения: ";
    printPolynomial(result);

    return 0;
}