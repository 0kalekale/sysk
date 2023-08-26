/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_GRAMMAR_H_INCLUDED
# define YY_YY_SRC_GRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_I8 = 258,                    /* T_I8  */
    T_I16 = 259,                   /* T_I16  */
    T_I32 = 260,                   /* T_I32  */
    T_I64 = 261,                   /* T_I64  */
    T_U8 = 262,                    /* T_U8  */
    T_U16 = 263,                   /* T_U16  */
    T_U32 = 264,                   /* T_U32  */
    T_U64 = 265,                   /* T_U64  */
    T_VOID = 266,                  /* T_VOID  */
    T_BOOL = 267,                  /* T_BOOL  */
    T_RETURN = 268,                /* T_RETURN  */
    T_TRUE = 269,                  /* T_TRUE  */
    T_FALSE = 270,                 /* T_FALSE  */
    T_IF = 271,                    /* T_IF  */
    T_ELSE = 272,                  /* T_ELSE  */
    T_VAR = 273,                   /* T_VAR  */
    T_CONST = 274,                 /* T_CONST  */
    T_RIGHT_AB = 275,              /* T_RIGHT_AB  */
    T_LEFT_AB = 276,               /* T_LEFT_AB  */
    T_EQUALCMP = 277,              /* T_EQUALCMP  */
    T_N_EQUALCMP = 278,            /* T_N_EQUALCMP  */
    T_PLUS = 279,                  /* T_PLUS  */
    T_MINUS = 280,                 /* T_MINUS  */
    T_ASTRK = 281,                 /* T_ASTRK  */
    T_FSLASH = 282,                /* T_FSLASH  */
    T_EXPONENET = 283,             /* T_EXPONENET  */
    T_BITW_OR = 284,               /* T_BITW_OR  */
    T_BITW_AND = 285,              /* T_BITW_AND  */
    T_STR = 286,                   /* T_STR  */
    T_CHAR = 287,                  /* T_CHAR  */
    T_LAMBDA = 288,                /* T_LAMBDA  */
    T_PROC = 289,                  /* T_PROC  */
    INT = 290,                     /* INT  */
    IDENTIFER = 291,               /* IDENTIFER  */
    UMINUS = 292                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define T_I8 258
#define T_I16 259
#define T_I32 260
#define T_I64 261
#define T_U8 262
#define T_U16 263
#define T_U32 264
#define T_U64 265
#define T_VOID 266
#define T_BOOL 267
#define T_RETURN 268
#define T_TRUE 269
#define T_FALSE 270
#define T_IF 271
#define T_ELSE 272
#define T_VAR 273
#define T_CONST 274
#define T_RIGHT_AB 275
#define T_LEFT_AB 276
#define T_EQUALCMP 277
#define T_N_EQUALCMP 278
#define T_PLUS 279
#define T_MINUS 280
#define T_ASTRK 281
#define T_FSLASH 282
#define T_EXPONENET 283
#define T_BITW_OR 284
#define T_BITW_AND 285
#define T_STR 286
#define T_CHAR 287
#define T_LAMBDA 288
#define T_PROC 289
#define INT 290
#define IDENTIFER 291
#define UMINUS 292

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_GRAMMAR_H_INCLUDED  */
