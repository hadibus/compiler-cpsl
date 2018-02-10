%{
#include <iostream>
extern int yylex();
extern int yylineno;
extern char * yytext;
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

%left ADD
%left SUB
%left MULT
%left DIV
%left AND
%left OR
%right NOT
%nonassoc EQ
%nonassoc DIAM
%nonassoc LT
%nonassoc LEQ
%nonassoc GT
%nonassoc GEQ
%token DOT
%token COMMA
%token COLON
%token SCOLON
%token LPAREN
%token RPAREN
%token LBRACK
%token RBRACK
%token ASSIGN
%left MOD
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

%start Program

%%
ArrayType : KW_ARRAY LBRACK Expression COLON Expression RBRACK KW_OF Type {};
Assignment : LValue ASSIGN Expression {};
Body : Block                               {}
     | ConstantDecl Block                  {}
     | TypeDecl Block                      {}
     | ConstantDecl TypeDecl Block         {}
     | VarDecl Block                       {}
     | ConstantDecl VarDecl Block          {}
     | TypeDecl VarDecl Block              {}
     | ConstantDecl TypeDecl VarDecl Block {}
     ;
Block : KW_BEGIN StatementSeq KW_END {};
ConstantDecl : KW_CONST ConstDeclItems {};
ConstDeclItem : ID EQ Expression SCOLON {};
ConstDeclItems : ConstDeclItems ConstDeclItem {}
               | ConstDeclItem                {}
               ;
ElseifStuff : ElseifStuff ElseifThing {}
            | ElseifThing             {}
            ;
ElseifThing : KW_ELSEIF Expression KW_THEN StatementSeq {};
ElseStuff : KW_ELSE StatementSeq {};
ExpressionsList : ExpressionsList COMMA Expression {}
                | Expression                       {}
                ;
Expression : Expression OR Expression2 {}
           | Expression2               {}
           ;
Expression2 : Expression2 AND Expression3 {}
            | Expression3                 {}
            ;
Expression3 : NOT Expression3 {}
            | Expression4     {}
            ;
Expression4 : Expression4 EQ Expression5   {}
            | Expression4 DIAM Expression5 {}
            | Expression4 LT Expression5   {}
            | Expression4 LEQ Expression5  {}
            | Expression4 GT Expression5   {}
            | Expression4 GEQ Expression5  {}
            | Expression5                  {}
            ;
Expression5 : Expression5 ADD Expression6 {}
            | Expression5 SUB Expression6 {}
            | Expression6                 {}
            ;
Expression6 : Expression6 MULT Expression7 {}
            | Expression6 DIV Expression7  {}
            | Expression6 MOD Expression7  {}
            | Expression7                  {}
            ;
Expression7 : SUB Expression7 {}
            | Expression8     {}
            ;
Expression8 : LPAREN Expression RPAREN         {}
            | NUMBER                           {}
            | CONST_STR                        {}
            | CONST_CHAR                       {}
            | ID LPAREN RPAREN                 {}
            | ID LPAREN ExpressionsList RPAREN {}
            | KW_CHR LPAREN Expression RPAREN  {}
            | KW_ORD LPAREN Expression RPAREN  {}
            | KW_PRED LPAREN Expression RPAREN {}
            | KW_SUCC LPAREN Expression RPAREN {}
            | LValue                           {}
            ;
FormalParameter : IdList COLON Type        {}
                | KW_VAR IdList COLON Type {}
                | KW_REF IdList COLON Type {}
                ;
FormalParameters : FormalParameters SCOLON FormalParameter {}
                 | FormalParameter                         {}
                 ;
ForStatement : KW_FOR ID ASSIGN Expression KW_TO Expression KW_DO StatementSeq KW_END     {}
             | KW_FOR ID ASSIGN Expression KW_DOWNTO Expression KW_DO StatementSeq KW_END {}
             ;
FuncsAndProceds : FuncsAndProceds FuncOrProced {}
                | FuncOrProced                 {}
                ;
FuncOrProced : FunctionDecl  {}
             | ProcedureDecl {}
             ;
FunctionDecl : KW_FUNCTION ID LPAREN RPAREN COLON Type SCOLON KW_FORWARD SCOLON                  {}
             | KW_FUNCTION ID LPAREN RPAREN COLON Type SCOLON Body SCOLON                        {}
             | KW_FUNCTION ID LPAREN FormalParameters RPAREN COLON Type SCOLON KW_FORWARD SCOLON {}
             | KW_FUNCTION ID LPAREN FormalParameters RPAREN COLON Type SCOLON Body SCOLON       {}
             ;
IdList : IdList COMMA ID {}
       | ID              {}
       ;
IfStatement : KW_IF Expression KW_THEN StatementSeq KW_END                       {}
            | KW_IF Expression KW_THEN StatementSeq ElseifStuff KW_END           {}
            | KW_IF Expression KW_THEN StatementSeq ElseStuff KW_END             {}
            | KW_IF Expression KW_THEN StatementSeq ElseifStuff ElseStuff KW_END {}
            ;
LValue : ID           {}
       | ID LValStuff {}
       ;
LValStuff : LValStuff LValStuffItem {}
          | LValStuffItem           {}
          ;
LValStuffItem : DOT ID                   {}
              | LBRACK Expression RBRACK {}
              ;
LValuesList : LValuesList LValue COMMA {}
            | LValue COMMA             {}
            ;
NullStatement : {};
ProcedureCall : ID LPAREN ExpressionsList RPAREN {}
              | ID LPAREN RPAREN                 {}
              ;
ProcedureDecl : KW_PROCEDURE ID LPAREN RPAREN SCOLON KW_FORWARD SCOLON                  {}
              | KW_PROCEDURE ID LPAREN RPAREN SCOLON Body SCOLON                        {}
              | KW_PROCEDURE ID LPAREN FormalParameters RPAREN SCOLON KW_FORWARD SCOLON {}
              | KW_PROCEDURE ID LPAREN FormalParameters RPAREN SCOLON Body SCOLON       {}
              ;
Program : Block DOT                                               {}
        | ConstantDecl Block DOT                                  {}
        | TypeDecl Block DOT                                      {}
        | ConstantDecl TypeDecl Block DOT                         {}
        | VarDecl Block DOT                                       {}
        | ConstantDecl VarDecl Block DOT                          {}
        | TypeDecl VarDecl Block DOT                              {}
        | ConstantDecl TypeDecl VarDecl Block DOT                 {}
        | FuncsAndProceds Block DOT                               {}
        | ConstantDecl FuncsAndProceds Block DOT                  {}
        | TypeDecl FuncsAndProceds Block DOT                      {}
        | ConstantDecl TypeDecl FuncsAndProceds Block DOT         {}
        | VarDecl FuncsAndProceds Block DOT                       {}
        | ConstantDecl VarDecl FuncsAndProceds Block DOT          {}
        | TypeDecl VarDecl FuncsAndProceds Block DOT              {}
        | ConstantDecl TypeDecl VarDecl FuncsAndProceds Block DOT {}
        ;
ReadStatement : KW_READ LPAREN LValue RPAREN             {}
              | KW_READ LPAREN LValuesList LValue RPAREN {}
              ;
RecordAndVarTypeItem : IdList COLON Type SCOLON {};
RecordAndVarTypeItems : RecordAndVarTypeItems RecordAndVarTypeItem {}
                      | RecordAndVarTypeItem                       {}
                      ;
RecordType : KW_RECORD RecordAndVarTypeItems KW_END {}
           | KW_RECORD KW_END                       {}
           ;
RepeatStatement : KW_REPEAT StatementSeq KW_UNTIL Expression {};
ReturnStatement : KW_RETURN Expression {}
                | KW_RETURN            {}
                ;
SimpleType : ID {};
Statement : Assignment      {}
          | IfStatement     {}
          | WhileStatement  {}
          | RepeatStatement {}
          | ForStatement    {}
          | StopStatement   {}
          | ReturnStatement {}
          | ReadStatement   {}
          | WriteStatement  {}
          | ProcedureCall   {}
          | NullStatement   {}
          ;
StatementSeq : StatementSeq SCOLON Statement {}
             | Statement                     {}
             ;
StopStatement : KW_STOP {};
Type : SimpleType {}
     | RecordType {}
     | ArrayType  {}
     ;
TypeDecl : KW_TYPE TypeDeclItems {};
TypeDeclItem : ID EQ Type SCOLON {};
TypeDeclItems : TypeDeclItems TypeDeclItem {}
              | TypeDeclItem               {}
              ;
VarDecl : KW_VAR RecordAndVarTypeItems {};
WhileStatement : KW_WHILE Expression KW_DO StatementSeq KW_END {};
WriteStatement : KW_WRITE LPAREN ExpressionsList RPAREN {};
%%

void yyerror(const char* s)
{
  std::cout << s << " at line " << yylineno;
  std::cout << " near \"" << yytext << "\"" << std::endl;
}
