%{
#include <iostream>
extern int yylex();
void yyerror(const char*);
%}

%union
{
int val;
}

%token KW_ARRAY
%token KW_BEGIN
%token KW_CHR
%token KW_CONST
%token KW_DO
%token KW_DOWNTO
%token KW_ELSE
%token KW_ELSEIF
%token KW_END
%token KW_FOR
%token KW_FORWARD
%token KW_FUNCTION
%token KW_IF
%token KW_OF
%token KW_ORD
%token KW_PRED
%token KW_PROCEDURE
%token KW_READ
%token KW_RECORD
%token KW_REF
%token KW_REPEAT
%token KW_RETURN
%token KW_STOP
%token KW_SUCC
%token KW_THEN
%token KW_TO
%token KW_TYPE
%token KW_UNTIL
%token KW_VAR
%token KW_WHILE
%token KW_WRITE

%token ID

%token ADD
%token SUB
%token MULT
%token DIV
%token AND
%token OR
%token NOT
%token EQ
%token DIAM
%token LT
%token LEQ
%token GT
%token GEQ
%token DOT
%token COMMA
%token COLON
%token SCOLON
%token LPAREN
%token RPAREN
%token LBRACK
%token RBRACK
%token ASSIGN
%token MOD
%token NUMBER


%token CONST_CHAR
%token CONST_STR
%token COMMENT
%token WHITESPACE
%token NEWLINE

%type <val> NUMBER
%type <val> Expression
%type <val> Expression2
%type <val> Expression3
%type <val> Expression4
%type <val> Expression5
%type <val> Expression6
%type <val> Expression7
%type <val> Expression8


%%
Program : Statement {}
        ;

Statement : Expression SCOLON {std::cout << $1 << std::endl;}
          ;

Expression : Expression OR Expression2 {$$ = $1 || $3;}
           | Expression2 {$$ = $1;}
           ;

Expression2 : Expression2 AND Expression3 {$$ = $1 && $3;}
            | Expression3 {$$ = $1;}
            ;

Expression3 : NOT Expression3 {$$ = !$2;}
            | Expression4 {$$ = $1;}
            ;

Expression4 : Expression4 EQ Expression5 {$$ = $1 == $3;}
            | Expression4 LT Expression5 {$$ = $1 < $3;}
            | Expression4 LEQ Expression5 {$$ = $1 <= $3;}
            | Expression4 GT Expression5 {$$ = $1 > $3;}
            | Expression4 GEQ Expression5 {$$ = $1 >= $3;}
            | Expression5 {$$ = $1;}
            ;

Expression5 : Expression5 ADD Expression6 {$$ = $1 + $3;}
            | Expression5 SUB Expression6 {$$ = $1 - $3;}
            | Expression6 {$$ = $1;}
            ;

Expression6 : Expression6 MULT Expression7 {$$ = $1 * $3;}
            | Expression6 DIV Expression7 {$$ = $1 / $3;}
            | Expression6 MOD Expression7 {$$ = $1 % $3;}
            | Expression7 {$$ = $1;}
            ;

Expression7 : SUB Expression7 {$$ = - $2;}
            | Expression8 {$$ = $1;}
            ;

Expression8 : LPAREN Expression RPAREN {$$ = $2;}
            | NUMBER {$$ = $1;}
            ;
%%

void yyerror(const char* s)
{
  std::cout << s << std::endl;
}
