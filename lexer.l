%option noyywrap
%{
#include<iostream>
#include<cstdlib>
%}

DELIM "+"|"-"|"*"|"/"|"&"|"|"|"~"|"="|"<>"|"<"|"<="|">"|">="|"."|","|":"|";"|"("|")"|"["|"]"|":="|"%" 

DELIM2 \+\-\*\/\&\|\~\="<>"\<"<="\>">="\.\,\:\;\(\)\[\]":="\% 
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
std::cout << "Identifier(" << yytext << ")" << std::endl;
}

{DELIM} {
std::cout << "Operator/delimeter(" << yytext << ")" << std::endl;
}

(0x[0-9a-fA-F]+)|(0[0-7]+)|([1-9][0-9]*)|(0) {
std::cout << "Integer(" << yytext << ")" << std::endl;
std::cout << std::strtol(yytext, NULL, 0) << std::endl;
}

'.'|'\\.' {
std::cout << "Const-Char(" << yytext << ")" << std::endl;
}

\"([^\\\"\n]|\\.)+\" {
std::cout << "Const-String(" << yytext << ")" << std::endl;
}

$.* {
std::cout << "Comment(" << yytext << ")" << std::endl;
}

\t|" "+ {
std::cout << "Whitespace(" << yytext << ")" << std::endl;
}

\n {
std::cout << "Newline found" << std::endl;
}


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

