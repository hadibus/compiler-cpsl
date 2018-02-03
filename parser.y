%{
#include <iostream>
extern int yylex();
%}

%union
{
float val;
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
%token OPEN
%token CLOSE
%token DONE
%token NUMBER
%token LVALUE


%token CONST_CHAR
%token CONST_STR
%token COMMENT
%token WHITESPACE
%token NEWLINE

%type <val> NUMBER
%type <val> Expression
%type <val> Factor
%type <val> Term

%%

Program : ConstantDecl Program2
	| Program2
Program2 : TypeDecl Program3
         | Program3
Program3 : VarDecl Program4
         | Program4
Program4 : Funkies Block

ConstantDecl : KW_CONST ConstDeclList

ConstDeclList : ConstDeclListMore ConstDeclListItem

ConstDeclListMore : ConstDeclListMore ConstDeclListItem
                  |

ConstDeclListItem : ID EQ Expression SCOLON

Expression : Expression OR Expression2 { $$ = $1 | $3;}
           | Expression2 { $$ = $1;}
           ;

Expression2 : Expression2 AND Expression3 { $$ = $1 & $3;}
            | Expression3 { $$ = $1;}
            ;

Expression3 : NOT Expression { $$ = !$2;}
	    | Expression4 { $$ = $1;}
            ;

Expression4 : Expression4 EQ Expression5 { $$ = $1 == $3;}
            | Expression4 DIAM Expression5 { }
            | Expression4 LT Expression5 {}
            | Expression4 LEQ Expression5 {}
            | Expression4 GT Expression5 {}
            | Expression4 GEQ Expression5 {}
            | Expression5 {}
            ;

Expression5 : Expression5 ADD Expression6 {}
            | Expression5 SUB Expression6 {}
            | Expression6 {}
            ;

Expression6 : Expression6 MULT Expression7 {}
            | Expression6 DIV Expression7 {}
            | Expression6 MOD Expression7 {}
            | Expression7 {}
            ;

Expression7 : SUB Expression8 {}
            | Expression8
            ;

Expression8 : OPEN Expression CLOSE {}
            | NUMBER {}
            ;

Expression2 : LPAREN Expression RPAREN

ExprList : 
Statement : Expression DONE {std::cout << $1 << std::endl;};
Expression : Expression ADD Term {$$ = $1 + $3;}
           | Expression SUB Term {$$ = $1 - $3;}
           | Term {$$ = $1;};

Term : Term MULT Factor { $$ = $1 * $3;}
     | Term DIV Factor { $$ = $1 / $3;}
     | Factor {$$ = $1;}
     ;
Factor : OPEN Expression CLOSE {$$ = $2;}
       | NUMBER {$$ = $1;}
       ;

%%

void yyerror(const char* s)
{
  std::cout << s << std::endl;
}
