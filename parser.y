%{
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "CodeGenerator.hpp"
extern int yylex();
void yyerror(const char*);
CodeGenerator cg;
%}

%union
{
  char * str_val;
  int int_val;
  char char_val;
}

%error-verbose
%token ARRAYSY 
%token ASSIGNSY 
%token BEGINSY 
%token CHARCONSTSY 
%token CHRSY 
%token COLONSY 
%token COMMASY 
%token CONSTSY 
%token DIVSY 
%token DOSY 
%token DOTSY 
%token DOWNTOSY 
%token ELSEIFSY 
%token ELSESY 
%token ENDSY 
%token EQSY 
%token FORSY 
%token FORWARDSY 
%token FUNCTIONSY 
%token GTESY 
%token GTSY 
%token IDENTSY
%token IFSY 
%token INTSY 
%token LBRACKETSY 
%token LPARENSY 
%token LTESY 
%token LTSY 
%token MINUSSY 
%token MODSY 
%token MULTSY
%token NOTSY 
%token OFSY
%token ORDSY 
%token PLUSSY 
%token PREDSY 
%token PROCEDURESY 
%token RBRACKETSY
%token READSY 
%token RECORDSY 
%token REFSY 
%token REPEATSY 
%token RETURNSY 
%token RPARENSY 
%token SCOLONSY 
%token STOPSY 
%token STRINGSY 
%token SUCCSY
%token THENSY 
%token TOSY 
%token TYPESY 
%token UNTILSY 
%token VARSY 
%token WHILESY 
%token WRITESY

%left ANDSY ORSY
%right NOTSY
%nonassoc EQSY LTESY GTESY GTSY LTSY NEQSY 
%left PLUSSY MINUSSY 
%left DIVSY MODSY MULTSY
%right UMINUSSY 

%type <char_val> CHARCONSTSY
%type <int_val> Arguments 
%type <int_val> ArrayType 
%type <int_val> Assignment
%type <int_val> Block 
%type <int_val> Body  
%type <int_val> ElseClause 
%type <int_val> ElseIfHead 
%type <int_val> ElseIfList 
%type <int_val> Expression 
%type <int_val> FSignature 
%type <int_val> FieldDecl 
%type <int_val> FieldDecls
%type <int_val> ForHead 
%type <int_val> ForStatement 
%type <int_val> FormalParameter
%type <int_val> FormalParameters  
%type <int_val> FunctionCall 
%type <int_val> INTSY 
%type <int_val> IdentList 
%type <int_val> OptVar 
%type <int_val> IfHead 
%type <int_val> IfStatement 
%type <int_val> LValue 
%type <int_val> OptArguments 
%type <int_val> OptFormalParameters  
%type <int_val> PSignature 
%type <int_val> ProcedureCall
%type <int_val> ReadArgs
%type <int_val> ReadStatement 
%type <int_val> RecordType 
%type <int_val> RepeatStatement 
%type <int_val> ReturnStatement 
%type <int_val> SimpleType 
%type <int_val> Statement 
%type <int_val> StatementList 
%type <int_val> StopStatement 
%type <int_val> ThenPart 
%type <int_val> ToHead 
%type <int_val> Type 
%type <int_val> WhileHead 
%type <int_val> WhileStatement 
%type <int_val> WriteArgs 
%type <int_val> WriteStatement  
%type <str_val> IDENTSY 
%type <str_val> STRINGSY 

%%
Program : ProgramHead Block DOTSY {}
				;

ProgramHead : OptConstDecls OptTypeDecls OptVarDecls PFDecls
            ;
OptConstDecls : CONSTSY ConstDecls
	      |
	      ;

ConstDecls : ConstDecls ConstDecl
	   | ConstDecl
           ;

ConstDecl : IDENTSY EQSY Expression SCOLONSY {cg.storeConst($1,$3);}
	  ;

PFDecls : PFDecls ProcedureDecl
        | PFDecls FunctionDecl
        |
        ;

ProcedureDecl : PSignature SCOLONSY FORWARDSY SCOLONSY {}
              | PSignature SCOLONSY Body SCOLONSY {}
	      ;

PSignature : PROCEDURESY IDENTSY LPARENSY OptFormalParameters RPARENSY {}
           ;

FunctionDecl : FSignature SCOLONSY FORWARDSY SCOLONSY {}
	     | FSignature SCOLONSY Body SCOLONSY {}
             ;

FSignature : FUNCTIONSY IDENTSY LPARENSY OptFormalParameters RPARENSY COLONSY Type {}
           ;

OptFormalParameters : FormalParameters {}
                    | {}
                    ;

FormalParameters : FormalParameters SCOLONSY FormalParameter {}
                 | FormalParameter {}
                 ;

FormalParameter : OptVar IdentList COLONSY Type {}
                ;

OptVar : VARSY {}
       | REFSY {}
       | {}
       ;


Body : OptConstDecls OptTypeDecls OptVarDecls Block {}
     ;

Block : BEGINSY StatementList ENDSY {}
      ;

StatementList : StatementList SCOLONSY Statement {}
              | Statement {}
              ;

OptTypeDecls : TYPESY TypeDecls
             |
             ;

TypeDecls    : TypeDecls TypeDecl
             | TypeDecl
             ;

TypeDecl : IDENTSY EQSY Type SCOLONSY {cg.storeType($1, $3);}
         ;

Type : SimpleType {}
     | RecordType {}
     | ArrayType {}
     ;

SimpleType : IDENTSY {$$ = cg.lookupType($1);}
           ;

RecordType : RECORDSY FieldDecls ENDSY {$$ = $2;}
           ;

FieldDecls : FieldDecls FieldDecl {$$ = cg.mergeRecords($1,$2);}
           | FieldDecl {}
           ;

FieldDecl : IdentList COLONSY Type SCOLONSY {$$ = cg.makeRecordVars($1,$3);}
          ;

IdentList : IdentList COMMASY IDENTSY {$$ = cg.appendStrList($1,$3);}
          | IDENTSY {$$ = cg.makeStrList($1);}
          ;

ArrayType : ARRAYSY LBRACKETSY Expression COLONSY Expression RBRACKETSY OFSY Type {$$ = cg.buildArray($3,$5,$8);}
          ;

OptVarDecls : VARSY VarDecls
            |
            ;

VarDecls    : VarDecls VarDecl
            | VarDecl
            ;

VarDecl : IdentList COLONSY Type SCOLONSY {cg.makeVars($1,$3);}
        ;

Statement : Assignment {}
          | IfStatement {}
          | WhileStatement {}
          | RepeatStatement {}
          | ForStatement {}
          | StopStatement {}
          | ReturnStatement {}
          | ReadStatement {}
          | WriteStatement {}
          | ProcedureCall {}
          | {}
          ;

Assignment : LValue ASSIGNSY Expression {cg.assignExprToLval($1, $3);}
           ;

IfStatement : IfHead ThenPart ElseIfList ElseClause ENDSY {}
            ;

IfHead : IFSY Expression {}
       ;

ThenPart : THENSY StatementList {}
         ;

ElseIfList : ElseIfList ElseIfHead ThenPart {}
           | {}
           ;

ElseIfHead : ELSEIFSY Expression {}
           ;

ElseClause : ELSESY StatementList {}
           | {}
           ;

WhileStatement : WhileHead DOSY StatementList ENDSY {}
               ;

WhileHead : WHILESY Expression {}
          ;

RepeatStatement : REPEATSY StatementList UNTILSY Expression {}
                ;

ForStatement : ForHead ToHead DOSY StatementList ENDSY{}
             ;

ForHead : FORSY IDENTSY ASSIGNSY Expression {}
        ;

ToHead : TOSY Expression {}
       | DOWNTOSY Expression {}
       ;

StopStatement : STOPSY {cg.doStop();}
              ;

ReturnStatement : RETURNSY Expression {}
                | RETURNSY {}
                ;


ReadStatement : READSY LPARENSY ReadArgs RPARENSY {cg.clearExpressions();}
              ;

ReadArgs : ReadArgs COMMASY LValue {cg.readToLval($3);}
         | LValue                  {cg.readToLval($1);}
         ;

WriteStatement : WRITESY LPARENSY WriteArgs RPARENSY {cg.clearExpressions();}
               ;

WriteArgs : WriteArgs COMMASY Expression {cg.unOp($3,&CodeGenerator::writeExpression);}
          | Expression                   {cg.unOp($1,&CodeGenerator::writeExpression);}
          ;

ProcedureCall : IDENTSY LPARENSY OptArguments RPARENSY {}
              ;
OptArguments : Arguments {}
             |           {}
             ;
Arguments : Arguments COMMASY Expression {}
          | Expression                   {}
          ;

Expression : CHARCONSTSY                         {$$ = cg.charLiteral(yylval.char_val);}
           | CHRSY LPARENSY Expression RPARENSY  {$$ = cg.charCast($3);}
           | Expression ANDSY Expression         {$$ = cg.binOp($1,$3,&CodeGenerator::binOpAnd);}
           | Expression DIVSY Expression         {$$ = cg.binOp($1,$3,&CodeGenerator::binOpDiv);}
           | Expression EQSY Expression          {$$ = cg.binOp($1,$3,&CodeGenerator::binOpEq);}
           | Expression GTESY Expression         {$$ = cg.binOp($1,$3,&CodeGenerator::binOpGteq);}
           | Expression GTSY Expression          {$$ = cg.binOp($1,$3,&CodeGenerator::binOpGt);}
           | Expression LTESY Expression         {$$ = cg.binOp($1,$3,&CodeGenerator::binOpLteq);}
           | Expression LTSY Expression          {$$ = cg.binOp($1,$3,&CodeGenerator::binOpLt);}
           | Expression MINUSSY Expression       {$$ = cg.binOp($1,$3,&CodeGenerator::binOpSub);}
           | Expression MODSY Expression         {$$ = cg.binOp($1,$3,&CodeGenerator::binOpMod);}
           | Expression MULTSY Expression        {$$ = cg.binOp($1,$3,&CodeGenerator::binOpMult);}
           | Expression NEQSY Expression         {$$ = cg.binOp($1,$3,&CodeGenerator::binOpNeq);}
           | Expression ORSY Expression          {$$ = cg.binOp($1,$3,&CodeGenerator::binOpOr);}
           | Expression PLUSSY Expression        {$$ = cg.binOp($1,$3,&CodeGenerator::binOpAdd);}
           | FunctionCall                        {}
           | INTSY                               {$$ = cg.intLiteral($1);}
           | LPARENSY Expression RPARENSY        {$$ = $2;}
           | LValue                              {}
           | MINUSSY Expression %prec UMINUSSY   {$$ = cg.unOp($2,&CodeGenerator::unOpNeg);}
           | NOTSY Expression                    {$$ = cg.unOp($2,&CodeGenerator::unOpNot);}
           | ORDSY LPARENSY Expression RPARENSY  {$$ = cg.intCast($3);}
           | PREDSY LPARENSY Expression RPARENSY {$$ = cg.unOp($3,&CodeGenerator::unOpDecr);}
           | STRINGSY                            {$$ = cg.stringLiteral($1);}
           | SUCCSY LPARENSY Expression RPARENSY {$$ = cg.unOp($3,&CodeGenerator::unOpIncr);}
           ;

FunctionCall : IDENTSY LPARENSY OptArguments RPARENSY {}
             ;

LValue : LValue DOTSY IDENTSY {$$ = cg.getLvalRec($1,$3);}
       | LValue LBRACKETSY Expression RBRACKETSY {$$ = cg.getLvalArr($1,$3);}
       | IDENTSY {$$ = cg.getLval($1);}
       ;
%%

void yyerror(const char* msg)
{
  std::cerr << msg << std::endl;
  exit(1);
}
