%{
    #include <iostream>
    extern int yylineno;
    extern int yylex();
    extern char* yytext;
    void yyerror(char *s) {
      std::cout << s << ", line " << yylineno << " in " << yytext << std::endl;
    }
    #define YYSTYPE std::string
%}

%token NUM NAME SIGN_LOWER SIGN_UP SEP END_OP VAR_token

%%
PROGRAM: OPS                      {printf("%d ",1);};

OPS: OP                           {printf("%d ",2);}
|     OPS OP                      {printf("%d ",3);};

OP: INIT
| EXPRS;

INIT: VAR_INIT                    {printf("%d ",5);};

VAR_INIT: VAR_token VARS END_OP   {printf("%d ",7);};

VARS: VAR                         {printf("%d ",8);}
|     VARS SEP VAR                {printf("%d ",9);};

VAR: NAME '=' EXPR                {printf("%d ",10);}

EXPRS: EXPR  END_OP               {printf("%d ",11);};

EXPR: TERM                        {printf("%d ",12);}
|     EXPR SIGN_LOWER TERM        {printf("%d ",13);};

TERM: UNTERM                      {printf("%d ",14);}
|     TERM SIGN_UP UNTERM         {printf("%d ",15);};

UNTERM: VAL                       {printf("%d ",16);}
|       '-'VAL                    {printf("%d ",17);};

VAL:    NUM                       {printf("%d ",18);}
|       '('EXPR')'                {printf("%d ",19);}
|       NAME_POW                  {printf("%d ",20);};

NAME_POW: NAME POW                {printf("%d ",21);};

POW: '^' NUM                      {printf("%d ",22);}
|                                 {printf("%d ",23);};
%%