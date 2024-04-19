%{
    #include <iostream>
    #include <map>
    using namespace std;

    extern int yylineno;
    extern int yylex();
    extern char* yytext;
    void yyerror(char *s) {
      std::cout << s << ", line " << yylineno << " in " << yytext << std::endl;
    }
    #define YYSTYPE std::string

    map<string, string> variable;

%}

%token NUM NAME SIGN_LOWER SIGN_UP SEP END_OP VAR_token

%%
PROGRAM: OPS   {for (const auto& [product, price] : variable)
                std::cout << product << "\t" << price << std::endl;};

OPS: OP                        
|     OPS OP                      ;

OP: INIT
| EXPRS   { cout << $1 << endl;};

INIT: VAR_INIT                    ;

VAR_INIT: VAR_token VARS END_OP   ;

VARS: VAR                      
|     VARS SEP VAR;

VAR: NAME '=' EXPR                {variable[$1] = $3;};

EXPRS: EXPR  END_OP               {$$ = $1 + $2;};

EXPR: TERM                        {$$ = $1;}
|     EXPR SIGN_LOWER TERM        {$$ = $1 + $2 + $3;};

TERM: UNTERM                      {$$ = $1;}
|     TERM SIGN_UP UNTERM         {$$ = $1 + $2 + $3;};

UNTERM: VAL                       {$$ = $1;}
|       SIGN_LOWER VAL                    {$$ = $1 + $2;};

VAL:    NUM                       {$$ = $1;}
|       '('EXPR')'                {$$ = $1 + $2 + $3;}
|       NAME_POW                  {$$ = $1;};

NAME_POW: NAME POW                {$$ = $1 + $2;};

POW: '^' NUM                      {$$ = '^' + $2;}
|                                 {$$ = "";};
%%