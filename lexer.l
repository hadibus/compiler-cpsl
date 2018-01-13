%option noyywrap
%{
#include<iostream>
#include<cstdlib>
%}



%%
"array"|"ARRAY" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"begin"|"BEGIN" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"chr"|"CHR"  {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"const"|"CONST" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"do"|"DO" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"downto"|"DOWNTO" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"else"|"ELSE" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"elseif"|"ELSEIF" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"end"|"END" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"for"|"FOR" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"forward"|"FORWARD" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"function"|"FUNCTION" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"if"|"IF" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"of"|"OF" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"ord"|"ORD" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"pred"|"PRED" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"procedure"|"PROCEDURE" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"read"|"READ" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"record"|"RECORD" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"ref"|"REF" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"repeat"|"REPEAT" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"return"|"RETURN" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"stop"|"STOP" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"succ"|"SUCC" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"then"|"THEN" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"to"|"TO" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"type"|"TYPE" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"until"|"UNTIL" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"var"|"VAR" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"while"|"WHILE" {std::cout << "Keyword(" << yytext << ")" << std::endl;}
"write"|"WRITE" {std::cout << "Keyword(" << yytext << ")" << std::endl;}







[a-zA-Z][a-zA-Z0-9_]* {
/* Matches identifiers
 * First character is alph
 * later optional chars are alphanumeric and _
 */
std::cout << "Identifier(" << yytext << ")" << std::endl;}







"+"|"-"|"*"|"/"|"&"|"|"|"~"|"="|"<>"|"<"|"<="|">"|">="|"."|","|":"|";"|"("|")"|"["|"]"|":="|"%" {
std::cout << "Operator/delimeter(" << yytext << ")" << std::endl;
}







(0x[0-9a-fA-F]+)|(0[0-7]+)|([0-9]+) {std::cout << "Integer(" << std::strtol(yytext, NULL, 0) << ")" << std::endl;}






'.+' {std::cout << "Const-Char(" << yytext << ")" << std::endl;}








\"([^\\\"]|\\[^n])+\" {
/* This matches a string constant 
 * \" opening quote
 * [^\\\"] not a \" 
 * \\[^n] any excaped character except for newline 
 * ( ... )+ one or more
 * \" closing quote */
std::cout << "Const-Char(" << yytext << ")" << std::endl;
}







$.* {std::cout << "Comment(" << yytext << ")" << std::endl;}







\t|" "+ {std::cout << "Whitespace(" << yytext << ")" << std::endl;}
\n {std::cout << "Newline found" << std::endl;}



[0-9_][^0-7][8-9a-zA-Z]* {
/* This throws error on:
 * 0x (Incomplete hex, invalid identifier)
 * 08 (too large to be octal)
 * _
 */
throw std::runtime_error("Unidentified expression(" + std::string(yytext) + ")");
}

\"([^\\\"]|\\.|\n)+\" {
/* This throws error on a string containing a newline character 
 * \" opening quote
 * [^\\\"] not a \" 
 * \x where x is any character
 * ( ... )+ one or more
 * \" closing quote */
throw std::runtime_error("Newline char in string(" + std::string(yytext) + ")");
}

'\n' {
/* This throws when char constant contains newline character */
throw std::runtime_error("Newline char in const char(" + std::string(yytext) + ")");
}

\"\" {
/* This throws error on empty string */
throw std::runtime_error("Empty string constant");
}

'' {
/* This throws error on empy char */
throw std::runtime_error("Empty char constant");
}

%%


int main(){
  yylex();
}

