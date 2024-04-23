#include "eval.h"
#include <iostream>
#include <math.h>

using namespace std;
typedef struct znac {
	char value;
	struct znac* next;
} znac;

typedef struct number {
	int value;
	struct number* next;
} number;

number* create_number(int data) {
	number* a = (number*)malloc(sizeof(number));
	a->value = data;
	a->next = NULL;
	return a;
}
znac* create_znac(char data) {
	znac* a = (znac*)malloc(sizeof(znac));
	a->value = data;
	a->next = NULL;
	return a;
}

void push_znac(znac** a, char data) {
	znac* new_node = create_znac(data);
	new_node->next = *a;
	*a = new_node;
}

void push_number(number** a, int data) {
	number* new_node = create_number(data);
	new_node->next = *a;
	*a = new_node;
}

void pop_znac(znac** a) {
	znac* del = *a;
	*a = del->next;
	free(del);
}

void pop_number(number** a) {
	number* del = *a;
	*a = del->next;
	free(del);
}

int act(number* chisla, char c) { // функция действия 
	number* copy = chisla;
	if (copy->next == NULL) { // проверка на верность ввода
		printf("ERROR");
		_Exit(EXIT_SUCCESS);
	}
	copy = copy->next;
	int a = copy->value;    // первое число
	int b = chisla->value;	// второе число
	//printf("%d%c%d  ",a,c,b);
	if (c == '^')			// выбор действия
		return pow(a, b);
	if (c == '-')
		return a - b;
	if (c == '+')
		return a + b;
	if (c == '/') {
		if (b == 0) {  // деление на 0
			printf("ERROR");
			_Exit(EXIT_SUCCESS);
		}
		return a / b;
	}
	if (c == '%')
		return a % b;
	if (c == '*')
		return a * b;
}

int priora(char a) {    // выбор приоритета выполнения у знаков
	char gg[] = "+-*/%^";
	int i = 0;
	while (gg[i] != a)
		i++;
	if (i < 2)
		return 1;
	if (i < 5)
		return 2;
	if (i < 6)
		return 3;
	return 0;
}

int solution(number* a) {   // вывод
	number* copy = a;
	while (copy->next != NULL) {
		return copy->value;
		copy = copy->next;
	}
}

int eval(string expr)
{
	znac* bottom = create_znac(NULL);
	number* dno = create_number(NULL);

	int new_num = 0;  // счетчик для понимания есть-ли число, и для считывания не только цифр 
    char c;
    int index_expr = 0;
	c = expr[index_expr];
	if (c == '-') {   // вдруг выражение начинается с минуса
		push_number(&dno, 0);
		push_znac(&bottom, '-');
        index_expr++;
		c = expr[index_expr];
	}
	while (index_expr != expr.size()) {  // считываем файл до конца
		if (c == '(') {    //открывающая скобка ставится независимо от других знаков
			if (new_num > 0)  // есть-ли число перед скобой на которое надо умножить скобку
				push_znac(&bottom, '*');
			push_znac(&bottom, c);  // закидываем скобку в стек
			new_num = 0;
            index_expr++;
			c = expr[index_expr];
			continue;// что-бы прога дальше не шла лишний раз
		}
		if (c == ')') {                     // опа, поймали скобку
			while (bottom->value != '(') {   // выполняем все действия до открывающей скобки
				int res = act(dno, bottom->value);  //вызов функции действия
				pop_number(&dno); pop_number(&dno); // удаление цифр из стека
				pop_znac(&bottom);  //удаление знака из стека
				if (bottom == NULL) {  // проверка на верность ввода данных
					printf("ERROR");
					_Exit(EXIT_SUCCESS);
				}
				push_number(&dno, res);  //в стек закидываем результат действия
			}
			pop_znac(&bottom); // удалаяем оставшуюся открывающую скобку
			new_num = 1; // если скобки стоят так '()()'  , что-бы поставился знак умножения
			index_expr++;
            c = expr[index_expr];
			continue;// что-бы прога дальше не шла лишний раз
		}
		if (c >= '0' && c <= '9') {  // опа, поймали цифру
			if (new_num == 0)  // если это цифра (-10 > что-то > 10)
				push_number(&dno, (c - 48));
			else  // если это число
				dno->value = dno->value * 10 + c - 48;
			new_num += 1;
            index_expr++;
			c = expr[index_expr];
			continue;// что-бы прога дальше не шла лишний раз
		}

		if (bottom->value == '(' && c == '-') {  //представим отрицательно число как 0-что-то
			push_number(&dno, 0);
			push_znac(&bottom, '-');
            index_expr++;
			c = expr[index_expr];
			continue; // что-бы прога дальше не шла лишний раз
		}

		new_num = 0;

		int kol = 0;
		while (!kol) {   // выполняется до того, пока не поставится знак
			if (priora(bottom->value) >= priora(c)) { // если приоритет знака в стеке больше, то выполняем действие знака в стеке
				int res = act(dno, bottom->value);
				pop_number(&dno); pop_number(&dno);
				pop_znac(&bottom);
				push_number(&dno, res);
			}

			if (priora(bottom->value) < priora(c)) { // если приоритет знака в стеке меньше, то просто закидываем знак в стек
				push_znac(&bottom, c);
				kol = 1;
			}
		}
        index_expr++;
		c = expr[index_expr];
	}
	
	while (bottom->next != NULL) {			// делаем оствашиеся действия когда поток данных кончился
		int res = act(dno, bottom->value);
		pop_number(&dno); pop_number(&dno);
		pop_znac(&bottom);
		push_number(&dno, res);
	}
	return solution(dno);;
}

// int main(){
//     cout << eval(string("2+2"));
//     return EXIT_SUCCESS;
// }