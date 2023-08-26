/*
    This program contains the tokenizer 
    Copyright (C) 2022  0kalekale

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

%{
#include <math.h>
#include <grammar.h>

%}
%option yylineno  
%%

%{
/*
signed int types
*/
%}
"i8"		{ return T_I8; }
"i16"		{ return T_I16; }
"i32"		{ return T_I32; }
"i64"		{ return T_I64; }

%{
/*
unsigned int types
*/
%}
"u8"		{ return T_U8; }
"u16"		{ return T_U16; }
"u32"		{ return T_U32; }
"u64"		{ return T_U64; }

%{/*
some other types, will have more fancy things in the future 
*/%}
"void"		{ return T_VOID; }	
"bool"		{ return T_BOOL; }

%{
/*
some commonly used keywords
*/
%}
"return"	{ return T_RETURN; }

"True"		{ return T_TRUE; }
"False"		{ return T_FALSE; }

"if"		{ return T_IF; }
"else"		{ return T_ELSE; }

"var"		{ return T_VAR; }
"const"		{ return T_CONST; }

%{
/*
operations 
*/
%}
">"		{ return T_RIGHT_AB; }
"<"		{ return T_LEFT_AB; }
"=="		{ return T_EQUALCMP; }
"!="		{ return T_N_EQUALCMP; }

"+"		{ return T_PLUS; }
"-"		{ return T_MINUS; }
"*"		{ return T_ASTRK; }
"/"		{ return T_FSLASH; }
"^"		{ return T_EXPONENET; }

"&&"		{ return T_BITW_AND; }
"||"		{ return T_BITW_OR; }

%{
/*
chars and string literals
*/
%}
["].*["] 	{ return T_STR; }
["].["]		{ return T_CHAR; }

%{
/*		
lambda function and procedure keywords
*/
%}
		
"\\"		{ return T_LAMBDA; } 
"proc"		{ return T_PROC; }

([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) {
		return INT;
	}

[ \t]	;		 
	
[A-Za-z][A-Za-z0-9]*	{	
		return IDENTIFER;
	}

"$"	{ return 0; }

\n	|
.	return yytext[0];
%%
