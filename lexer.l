%option noyywrap
%{
#include<iostream>
#include<cstdlib>
%}

DELIM "+"|"-"|"*"|"/"|"&"|"|"|"~"|"="|"<>"|"<"|"<="|">"|">="|"."|","|":"|";"|"("|")"|"["|"]"|":="|"%" 

DELIM2 \+\-\*\/\&\|\~\="<>"\<"<="\>">="\.\,\:\;\(\)\[\]":="\% 
%%
"array"|"ARRAY"           {return KW_ARRAY;}
"begin"|"BEGIN"           {return KW_BEGIN;}
"chr"|"CHR"               {return KW_CHR;}
"const"|"CONST"           {return KW_CONST;}
"do"|"DO"                 {return KW_DO;}
"downto"|"DOWNTO"         {return KW_DOWNTO;}
"else"|"ELSE"             {return KW_ELSE;}
"elseif"|"ELSEIF"         {return KW_ELSEIF;}
"end"|"END"               {return KW_END;}
"for"|"FOR"               {return KW_FOR;}
"forward"|"FORWARD"       {return KW_FORWARD;}
"function"|"FUNCTION"     {return KW_FUNCTION;}
"if"|"IF"                 {return KW_IF;}
"of"|"OF"                 {return KW_OF;}
"ord"|"ORD"               {return KW_ORD;}
"pred"|"PRED"             {return KW_PRED;}
"procedure"|"PROCEDURE"   {return KW_PROCEDURE;}
"read"|"READ"             {return KW_READ;}
"record"|"RECORD"         {return KW_RECORD;}
"ref"|"REF"               {return KW_REF;}
"repeat"|"REPEAT"         {return KW_REPEAT;}
"return"|"RETURN"         {return KW_RETURN;}
"stop"|"STOP"             {return KW_STOP;}
"succ"|"SUCC"             {return KW_SUCC;}
"then"|"THEN"             {return KW_THEN;}
"to"|"TO"                 {return KW_TO;}
"type"|"TYPE"             {return KW_TYPE;}
"until"|"UNTIL"           {return KW_UNTIL;}
"var"|"VAR"               {return KW_VAR;} 
"while"|"WHILE"           {return KW_WHILE;} 
"write"|"WRITE"           {return KW_WRITE;}

[a-zA-Z][a-zA-Z0-9_]*     {return ID;}


"+" {return ADD;}
"-" {return SUB;}
"*" {return MULT;}
"/" {return DIV;}
"&" {return AND;}
"|" {return OR;}
"~" {return NOT;}
"=" {return EQ;}
"<>" {return DIAM;}
"<" {return LT;}
"<=" {return LEQ;}
">" {return GT;}
">=" {return GEQ;}
"." {return DOT;}
"," {return COMMA;}
":" {return COLON;}
";" {return SCOLON;}
"(" {return LPAREN;}
")" {return RPAREN;}
"[" {return LBRACK;}
"]" {return RBRACK;}
":=" {return ASSIGN;}
"%" {return MOD;}

(0x[0-9a-fA-F]+)|(0[0-7]+)|([1-9][0-9]*)|(0) {
 return NUMBER; 
}

'.'|'\\.'                 {return CONST_CHAR;}

\"([^\\\"\n]|\\.)+\"      {return CONST_STR;}

$.*                       {return COMMENT;}

\t|" "+                   {return WHITESPACE;}

\n                        {return NEWLINE;}


(_.*)|([0-9][0-9]*([^0-9\t\n \:\;\]\)\]\+\*\,\/\-])) {
throw std::runtime_error("Unidentified expression(" + std::string(yytext) + ")");
}

(0x[0-9a-fA-F]*[^0-9a-fA-F\t\n ])|(0[0-7]*[8-9]) {
throw std::runtime_error("Bad integer(" + std::string(yytext) + ")");
}

\"[^\n\"]*\n {
throw std::runtime_error("Newline char in string(" + std::string(yytext) + ")");
}

'\n' {
throw std::runtime_error("Newline char in const char(" + std::string(yytext) + ")");
}

\"\" {
throw std::runtime_error("Empty string constant");
}

'' {
throw std::runtime_error("Empty char constant");
}

'[^\\]. {
throw std::runtime_error("Bad const char(" + std::string(yytext) + ")");
}
%%


int main()
{
  yylex();
}

