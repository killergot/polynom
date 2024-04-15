%{
    #include <iostream>
    extern int yylineno;
    extern int yylex();
    extern char* yytext;
    void yyerror(char *s) {
      std::cout << s << ", line " << yylineno << " in " << yytext << std::endl;
      exit(1);
    }
    #define YYSTYPE std::string
%}

%token NUM FLOAT NAME

%%


%%