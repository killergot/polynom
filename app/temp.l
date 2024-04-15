%{
   #include <string>
   #define YYSTYPE std::string
   #include "y.tab.h"
   void yyerror(char *s);
%}

%option yylineno

%%

[/][/].*\n      ; // comment
[0-9]+          { yylval = yytext;
                  return NUM;
                }
[a-z]           { yylval = yytext;
                  return VAR;
                }
[$][A-Z]+       { yylval = yytext;
                  return NAME;
                }
[ \t\r\n]       ; // whitespace
[-+*^()] { return *yytext; }
.               yyerror("Invalid character");

%%

int yywrap() {
    return 1;
}