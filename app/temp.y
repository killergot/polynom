%{
    #include <iostream>
    #include <map>
    #include "../eval/evalPoly.h"
    using namespace std;

    extern int yylineno;
    extern int yylex();
    extern char* yytext;
    void yyerror(char *s) {
      std::cout << s << ", line " << yylineno << " in " << yytext << std::endl;
    }
    #define YYSTYPE std::string

    map<string, string,KeySizeComparator<string>> variable;

%}

%token NUM NAME SIGN_LOWER SIGN_UP SEP END_OP VAR_token 

%%
PROGRAM: OPS   {for (const auto& [product, price] : variable)
                cout << product << "\t" << price << endl;};

OPS: OP                        
|     OPS OP                      ;

OP: INIT
| EXPRS                           ;

INIT: VAR_INIT                    ;

VAR_INIT: VAR_token VARS END_OP   ;

VARS: VAR                      
|     VARS SEP VAR;

VAR: NAME '=' EXPR                {variable[$1] = $3;};

EXPRS: EXPR  END_OP               { cout << $1 << " = " << solution($1,variable) << endl;};

EXPR: TERM                        {$$ = $1;}
|     EXPR SIGN_LOWER TERM        {$$ = $1 + $2 + $3;};

TERM: UNTERM                      {$$ = $1;}
|     TERM SIGN_UP UNTERM         {$$ = $1 + $2 + $3;};

UNTERM: VAL POW                    {$$ = $1 + $2;}
|       SIGN_LOWER VAL  POW            {$$ = $1 + $2 + $3;};

VAL:    NUM                       {$$ = $1;}
|       '('EXPR')'                {$$ = '(' + $2 + ')';}
|       COEF_NAME_POW                  {$$ = $1;};

COEF_NAME_POW: COEF NAME              {$$ = $1 + $2;};

COEF: NUM                         {$$ = $1 + '*';}
|                                 {$$ = "";};

POW: '^' NUM                      {$$ = '^' + $2;}
|                                 {$$ = "";};
%%