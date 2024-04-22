#include <iostream>
#include <string>
#include "lex.yy.c"

extern int yyparse(); // Прототип функции yyparse() из сгенерированного файла


int main(int argc, char *argv[]) {
    FILE *file;
    if(argc!=2) {
        file = fopen("app/test.txt","r");
    }
    else{
        file = fopen(argv[1],"r");
    }

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