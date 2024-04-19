#include <iostream>
#include <map>

using namespace std;

int main()
{
    map<string, unsigned> products;
    // установка значений
    products["bread"] = 30;
    products["milk"] = 80;
    products["apple"] = 60;
 
    // получение значений
    cout << "bread\t" << products["bread"] << endl;
    cout << "milk\t" << products["milk"] << endl;
    cout << "apple\t" << products["apple"] << endl;
    cout << products.count("bread") << endl;

    string a = "123", b = "456";
    string res = a + b;
    cout << res;
    return 0;
}