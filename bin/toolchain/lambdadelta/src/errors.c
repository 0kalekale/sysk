/*
    Error handler
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

#include <stdio.h>
#include <stdlib.h>
#include <grammar.h>
#include <error.h>

#define RED "\033[0;31m"
#define NC "\033[0m"

int rc = 0;

/*
PROOOGRAMMERRR-SHAWNNNN!!
KORE WOOO, MITE KUDASAIIIII
NAAAAAAAAAAAAN DAAAAAAAAAA??

KORE KORE KORE
KORE WA NANI? KORE WA NANI? GERTRUUUUUUDE????
"....Let it be known that there has been a syntax error."
https://youtu.be/jfNaaqm8xPU
*/

void
yyerror (char *s) 
{
	// say it in red.
	fprintf(stderr, RED"Error: "NC"line %d: %s\n", yylineno, s);
	rc = 1;
}
