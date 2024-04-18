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
var             { yylval = yytext;
                  return VAR_token;
                }
[A-Za-z][A-Za-z0-9]*        { yylval = yytext;
                              return NAME;
                            }
[-+]            { yylval = yytext;
                  return SIGN_LOWER;  
                }
[*]             { yylval = yytext;
                  return SIGN_UP;
                }
[,]                {  yylval = yytext;
                  return SEP;
                }
[;]             {  yylval = yytext;
                  return END_OP;
                }
[ \t\r\n]       ; // whitespace
[^()] { return *yytext; }
.               yyerror("Invalid character");

%%

int yywrap() {
    return 1;
}