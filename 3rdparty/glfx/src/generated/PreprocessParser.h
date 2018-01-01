/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_PREPRO_SRC_GENERATED_PREPROCESSPARSER_H_INCLUDED
# define YY_PREPRO_SRC_GENERATED_PREPROCESSPARSER_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int prepro_debug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER = 258,
     FLOAT = 259,
     DEFINE = 260,
     IF = 261,
     IFDEF = 262,
     IFNDEF = 263,
     ELSE = 264,
     ENDIF = 265,
     IDENTIFIER = 266,
     IS_EQUAL = 267,
     BOOLEAN_NOT = 268,
     DEFINED = 269,
     AND = 270,
     OR = 271,
     WHITESPACE = 272,
     EOL = 273,
     MACRO_NAME = 274,
     TIMES_EQUALS = 275,
     OVER_EQUALS = 276,
     PERCENT_EQUALS = 277,
     PLUS_EQUALS = 278,
     MINUS_EQUALS = 279,
     LEFT_SHIFT_EQUALS = 280,
     RIGHT_SHIFT_EQUALS = 281,
     AND_EQUALS = 282,
     XOR_EQUALS = 283,
     OR_EQUALS = 284,
     LESS_EQ = 285,
     GRTR_EQ = 286,
     LEFT_SHIFT = 287,
     RIGHT_SHIFT = 288,
     PLUS_PLUS = 289,
     MINUS_MINUS = 290,
     EQ_EQ = 291,
     NOT_EQ = 292,
     SYMBOL = 293,
     DOUBLE_HASH = 294
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE prepro_lval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int prepro_parse (void *YYPARSE_PARAM);
#else
int prepro_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int prepro_parse (void);
#else
int prepro_parse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_PREPRO_SRC_GENERATED_PREPROCESSPARSER_H_INCLUDED  */
