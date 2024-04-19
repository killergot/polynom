#include <iostream>
#include "lex.yy.c"

extern int yyparse(); // Прототип функции yyparse() из сгенерированного файла


FILE *file;

int main() {

    file = fopen("app/test.txt","r");

    if (!file) {
        perror("fopen");
        return 1;
    }

    yyin = file;



    std::cout << "Starting parsing" << std::endl;
    int e = yyparse(); // Вызываем сгенерированный парсер
    std::cout << "Parsing finished." << std:: endl;

    fclose(yyin);

    return 0;
}