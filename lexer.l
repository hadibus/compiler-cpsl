%option noyywrap
%{
#include<iostream>
%}



%%
"array"|"ARRAY" {std::cout << "Keyword \"array\" found" << std::endl;}
"begin"|"BEGIN" {std::cout << "Keyword \"begin\" found" << std::endl;}
"chr"|"CHR" {std::cout << "Keyword \"chr\" found" << std::endl;}
"const"|"CONST" {std::cout << "Keyword \"const\" found" << std::endl;}
"do"|"DO" {std::cout << "Keyword \"do\" found" << std::endl;}
"downto"|"DOWNTO" {std::cout << "Keyword \"downto\" found" << std::endl;}
"else"|"ELSE" {std::cout << "Keyword \"else\" found" << std::endl;}
"elseif"|"ELSEIF" {std::cout << "Keyword \"elseif\" found" << std::endl;}
"end"|"END" {std::cout << "Keyword \"end\" found" << std::endl;}
"for"|"FOR" {std::cout << "Keyword \"for\" found" << std::endl;}
"forward"|"FORWARD" {std::cout << "Keyword \"forward\" found" << std::endl;}
"function"|"FUNCTION" {std::cout << "Keyword \"function\" found" << std::endl;}
"if"|"IF" {std::cout << "Keyword \"if\" found" << std::endl;}
"of"|"OF" {std::cout << "Keyword \"of\" found" << std::endl;}
"ord"|"ORD" {std::cout << "Keyword \"ord\" found" << std::endl;}
"pred"|"PRED" {std::cout << "Keyword \"pred\" found" << std::endl;}
"procedure"|"PROCEDURE" {std::cout << "Keyword \"procedure\" found" << std::endl;}
"read"|"READ" {std::cout << "Keyword \"read\" found" << std::endl;}
"record"|"RECORD" {std::cout << "Keyword \"record\" found" << std::endl;}
"ref"|"REF" {std::cout << "Keyword \"ref\" found" << std::endl;}
"repeat"|"REPEAT" {std::cout << "Keyword \"repeat\" found" << std::endl;}
"return"|"RETURN" {std::cout << "Keyword \"return\" found" << std::endl;}
"stop"|"STOP" {std::cout << "Keyword \"stop\" found" << std::endl;}
"succ"|"SUCC" {std::cout << "Keyword \"succ\" found" << std::endl;}
"then"|"THEN" {std::cout << "Keyword \"then\" found" << std::endl;}
"to"|"TO" {std::cout << "Keyword \"to\" found" << std::endl;}
"type"|"TYPE" {std::cout << "Keyword \"type\" found" << std::endl;}
"until"|"UNTIL" {std::cout << "Keyword \"until\" found" << std::endl;}
"var"|"VAR" {std::cout << "Keyword \"var\" found" << std::endl;}
"while"|"WHILE" {std::cout << "Keyword \"while\" found" << std::endl;}
"write"|"WRITE" {std::cout << "Keyword \"write\" found" << std::endl;}







[a-zA-Z][a-zA-Z0-9_]* {std::cout << "Identifier found" << std::endl;}







"+" {std::cout << "Operator/delimeter found" << std::endl;}
"-" {std::cout << "Operator/delimeter found" << std::endl;}
"*" {std::cout << "Operator/delimeter found" << std::endl;}
"/" {std::cout << "Operator/delimeter found" << std::endl;}
"&" {std::cout << "Operator/delimeter found" << std::endl;}
"|" {std::cout << "Operator/delimeter found" << std::endl;}
"~" {std::cout << "Operator/delimeter found" << std::endl;}
"=" {std::cout << "Operator/delimeter found" << std::endl;}
"<>" {std::cout << "Operator/delimeter found" << std::endl;}
"<" {std::cout << "Operator/delimeter found" << std::endl;}
"<=" {std::cout << "Operator/delimeter found" << std::endl;}
">" {std::cout << "Operator/delimeter found" << std::endl;}
">=" {std::cout << "Operator/delimeter found" << std::endl;}
"." {std::cout << "Operator/delimeter found" << std::endl;}
"," {std::cout << "Operator/delimeter found" << std::endl;}
":" {std::cout << "Operator/delimeter found" << std::endl;}
";" {std::cout << "Operator/delimeter found" << std::endl;}
"(" {std::cout << "Operator/delimeter found" << std::endl;}
")" {std::cout << "Operator/delimeter found" << std::endl;}
"[" {std::cout << "Operator/delimeter found" << std::endl;}
"]" {std::cout << "Operator/delimeter found" << std::endl;}
":=" {std::cout << "Operator/delimeter found" << std::endl;}
"%" {std::cout << "Operator/delimeter found" << std::endl;}







0x[a-fA-F0-9]+ {std::cout << "Hex constant found" << std::endl;}
0[0-7]+ {std::cout << "Octal constant found" << std::endl;}
0|[1-9][0-9]* {std::cout << "Decimal constant found" << std::endl;}






'.+' {std::cout << "Char constant found" << std::endl;}








\"([^\\\"]|\\.)+\" {std::cout << "String constant found" << std::endl;}







$.* {std::cout << "Comment found" << std::endl;}







\t|" " {std::cout << "Whitespace found" << std::endl;}
\n {std::cout << "Newline found" << std::endl;}



[0-9_][^0-7][8-9a-zA-Z]* {
/* This throws error on:
 * 0x (Incomplete hex, invalid identifier)
 * 08 (too large to be octal)
 * _
 */
throw std::runtime_error("Unidentified expression");
}


\"([^\\\"]|\\.|\n)+\" {
/* This throws error on a string containing a newline character */
throw std::runtime_error("String constant contains newline char");
}

'\n' {
/* This throws when char constant contains newline character */
throw std::runtime_error("Char constant contains newline char");
}

\"\" {
/* This throws error on empty string */
throw std::runtime error("Empty string constant");
}

'' {
/* This throws error on empy char */
throw std::runtime error("Emply char constant");
}
%%


int main(){
  yylex();
}

