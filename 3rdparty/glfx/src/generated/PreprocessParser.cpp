/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         prepro_parse
#define yylex           prepro_lex
#define yyerror         prepro_error
#define yylval          prepro_lval
#define yychar          prepro_char
#define yydebug         prepro_debug
#define yynerrs         prepro_nerrs

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 6 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"

    #ifdef _MSC_VER
		// MS's C++ compiler is more strict than gcc
		// No unistd.h
		#define YY_NO_UNISTD_H
		// Direct IO functions (read and such)
		#include <io.h>
		// Disable the warning for yywrap
		#pragma warning( disable: 4003 )
		// Disable the warning about depracted naming of _read and friends
		#pragma warning( disable: 4996 )
		#define YY_NO_UNISTD_H
		#define YYERROR_VERBOSE
	#endif
	#include <stdio.h>
	#include <iostream>		// For std::cout and cerr
    #include <sstream>		// For std::ostringstream
    #include <vector>		// For std::vector
	#include "Preprocessor.h"
	extern void prepro_error(const char *s);
	#include "PreprocessLexer.h"
	#include "glfxErrorCheck.h"
	#define YYSTYPE PreprocessorType
	#define YYDEBUG 1
	using namespace std;
	bool preproError=false;
	// We use a stack of booleans to handle #if #else #endif.
	// At the top of the stack is a value that is TRUE is we are in an active block, and FALSE otherwise.
	// So: ifdef, if, and ifndef all add either TRUE or FALSE to the stack.
	// else switches the value at the top of the stack
	// and endif pops the stack.
	vector<bool> activeBlockStack;
	vector<string> blockTestStack;
	extern int preproBracket;
	// We pass-through #if blocks.
	//vector<bool> passthroughBlockStack;
	void ClearParserState()
	{
		activeBlockStack.clear();
		blockTestStack.clear();
		preproError=false;
		preproBracket=0;
	}
	bool inActiveBlock()
	{
		if(!activeBlockStack.size())
			return true;
		return activeBlockStack.back();
	}
	void ifdef(string id)
	{
		bool result	=IsDefined(id)&&inActiveBlock();
		activeBlockStack.push_back(result);
		blockTestStack.push_back(("#ifdef ")+id);
		Skip();
	}
	void ifndef(string id)
	{
		bool result=(!IsDefined(id))&&inActiveBlock();
		activeBlockStack.push_back(result);
		blockTestStack.push_back(("#ifndef ")+id);
//		passthroughBlockStack.push_back(false);
		Skip();
	}
	bool hash_if(string id,Test test,int num=0)
	{
		if(test==IS_DEFINED)
		{
			return IsDefined(id);
		}
		if(test==NOT_DEFINED)
		{
			return !IsDefined(id);
		}
		const MacroDefinition *m=GetMacro(id);
		if(!m)
		{
			string str=string("Macro ")+id+string(" not defined");
			prepro_error(str.c_str());
			return false;
		}
		int value=1;
		if(m->definition.size())
			value=atoi(m->definition[0].c_str());
		bool result=false;
		if(test==EQUALS)
		{
			result=(value==num);
		}
		else if(test==NONZERO)
		{
			result=(value!=0);
		}
		else
		{
			prepro_error("Can't handle this condition");
			return false;
		}
		return result;
	}
		//
	void apply_condition(string var,bool result)
	{
		activeBlockStack.push_back(result&&inActiveBlock());
		blockTestStack.push_back(var);
	}

	void endif()
	{
		//if(passthroughBlockStack.back())
		//	preproOutput<<"#endif\n";
		if(activeBlockStack.size()==0)
			prepro_error("Mismatched #endif");
		else
		{
			activeBlockStack.pop_back();
			blockTestStack.pop_back();
		//	passthroughBlockStack.pop_back();
		}
		Skip();
	}
	void hash_else()
	{
		//if (passthroughBlockStack.back())
		//	preproOutput << "#else\n";
		//else
		{
			if(activeBlockStack.size()==0)
			{
				prepro_error("Mismatched #else");
				return;
			}
			else
			{
				bool current = activeBlockStack.back();
				activeBlockStack.pop_back();
				activeBlockStack.push_back((!current)&&inActiveBlock());
				if(current)
					Skip();
			}
		}
		blockTestStack.back() += " #else";
	}

/* Line 371 of yacc.c  */
#line 220 "src/generated/PreprocessParser.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "PreprocessParser.h".  */
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 325 "src/generated/PreprocessParser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   173

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNRULES -- Number of states.  */
#define YYNSTATES  161

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    63,     2,     2,     2,    61,    60,     2,
      40,    41,    51,    52,    42,    53,    43,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    57,    48,
      44,    49,    45,    56,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    54,     2,    55,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,    58,    47,    62,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,    10,    13,    16,    19,    24,
      26,    28,    30,    32,    34,    36,    38,    40,    42,    44,
      46,    48,    50,    52,    54,    56,    58,    60,    62,    64,
      66,    68,    70,    72,    76,    77,    81,    83,    85,    87,
      93,    95,    99,   101,   105,   107,   111,   113,   117,   119,
     123,   125,   129,   133,   135,   139,   143,   147,   151,   153,
     157,   161,   163,   167,   171,   173,   177,   181,   185,   187,
     189,   192,   195,   198,   200,   202,   204,   206,   208,   210,
     212,   217,   222,   226,   230,   232,   234,   238,   240,   244,
     246,   250,   251,   255,   257,   260,   263,   265,   270,   272,
     273,   276,   277,   280,   281,   284,   286,   287,   289,   291,
     293,   295,   298,   300
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      65,     0,    -1,    65,    66,    -1,    -1,     6,    90,    -1,
      19,    89,    -1,     8,    11,    -1,     7,    11,    -1,     5,
      11,    68,    95,    -1,     9,    -1,    10,    -1,    18,    -1,
       3,    -1,    40,    -1,    15,    -1,    41,    -1,    67,    -1,
      42,    -1,    43,    -1,    44,    -1,    45,    -1,    46,    -1,
      47,    -1,    42,    -1,    48,    -1,    49,    -1,    50,    -1,
      51,    -1,    52,    -1,    53,    -1,    54,    -1,    55,    -1,
      38,    -1,    40,    69,    41,    -1,    -1,    69,    42,    70,
      -1,    70,    -1,    71,    -1,    72,    -1,    72,    56,    70,
      57,    71,    -1,    73,    -1,    72,    16,    73,    -1,    74,
      -1,    73,    15,    74,    -1,    75,    -1,    74,    58,    75,
      -1,    76,    -1,    75,    59,    76,    -1,    77,    -1,    76,
      60,    77,    -1,    78,    -1,    77,    36,    78,    -1,    77,
      37,    78,    -1,    79,    -1,    78,    44,    79,    -1,    78,
      45,    79,    -1,    78,    30,    79,    -1,    78,    31,    79,
      -1,    80,    -1,    79,    32,    80,    -1,    79,    33,    80,
      -1,    81,    -1,    80,    52,    81,    -1,    80,    53,    81,
      -1,    82,    -1,    81,    51,    82,    -1,    81,    50,    82,
      -1,    81,    61,    82,    -1,    83,    -1,    85,    -1,    34,
      83,    -1,    35,    83,    -1,    84,    82,    -1,    60,    -1,
      51,    -1,    52,    -1,    53,    -1,    62,    -1,    63,    -1,
      86,    -1,    85,    54,    70,    55,    -1,    85,    40,    88,
      41,    -1,    85,    40,    41,    -1,    85,    43,    11,    -1,
      11,    -1,    87,    -1,    40,    70,    41,    -1,     3,    -1,
      88,    42,    71,    -1,    71,    -1,    40,    88,    41,    -1,
      -1,    90,    16,    91,    -1,    91,    -1,    11,    94,    -1,
      93,    92,    -1,     3,    -1,    14,    40,    11,    41,    -1,
      13,    -1,    -1,    12,     3,    -1,    -1,    17,    96,    -1,
      -1,    96,    97,    -1,    97,    -1,    -1,    11,    -1,    40,
      -1,    41,    -1,     3,    -1,    39,    11,    -1,    67,    -1,
      17,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   159,   159,   161,   165,   172,   185,   190,   195,   216,
     220,   224,   227,   231,   235,   239,   243,   247,   251,   255,
     259,   263,   267,   271,   275,   279,   283,   287,   291,   295,
     299,   303,   307,   311,   316,   319,   326,   330,   334,   338,
     343,   347,   351,   355,   359,   363,   367,   371,   375,   379,
     384,   388,   392,   396,   400,   404,   408,   412,   416,   420,
     424,   428,   432,   436,   440,   444,   448,   452,   456,   460,
     464,   468,   472,   476,   476,   476,   476,   476,   476,   480,
     484,   488,   492,   496,   501,   505,   509,   513,   517,   525,
     533,   539,   544,   549,   554,   562,   570,   577,   583,   589,
     594,   602,   607,   613,   617,   625,   633,   636,   640,   644,
     648,   652,   656,   660
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMBER", "FLOAT", "DEFINE", "IF",
  "IFDEF", "IFNDEF", "ELSE", "ENDIF", "IDENTIFIER", "IS_EQUAL",
  "BOOLEAN_NOT", "DEFINED", "AND", "OR", "WHITESPACE", "EOL", "MACRO_NAME",
  "TIMES_EQUALS", "OVER_EQUALS", "PERCENT_EQUALS", "PLUS_EQUALS",
  "MINUS_EQUALS", "LEFT_SHIFT_EQUALS", "RIGHT_SHIFT_EQUALS", "AND_EQUALS",
  "XOR_EQUALS", "OR_EQUALS", "LESS_EQ", "GRTR_EQ", "LEFT_SHIFT",
  "RIGHT_SHIFT", "PLUS_PLUS", "MINUS_MINUS", "EQ_EQ", "NOT_EQ", "SYMBOL",
  "DOUBLE_HASH", "'('", "')'", "','", "'.'", "'<'", "'>'", "'{'", "'}'",
  "';'", "'='", "'/'", "'*'", "'+'", "'-'", "'['", "']'", "'?'", "':'",
  "'|'", "'^'", "'&'", "'%'", "'~'", "'!'", "$accept", "prog", "prepr",
  "symbol", "optional_param_decls", "param_decl_list", "expression",
  "conditional_exp", "logical_or_exp", "logical_and_exp",
  "inclusive_or_exp", "exclusive_or_exp", "and_exp", "equality_exp",
  "relational_exp", "shift_expression", "additive_exp", "mult_exp",
  "cast_exp", "unary_exp", "unary_operator", "postfix_exp", "primary_exp",
  "const", "argument_exp_list", "optional_params", "if_conditions",
  "if_condition", "defined_test", "optional_boolean_not", "if_test",
  "optional_definition", "opt_definition", "definition_element", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      40,    41,    44,    46,    60,    62,   123,   125,    59,    61,
      47,    42,    43,    45,    91,    93,    63,    58,   124,    94,
      38,    37,   126,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    64,    65,    65,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    68,    68,    69,    69,    70,    71,    71,
      72,    72,    73,    73,    74,    74,    75,    75,    76,    76,
      77,    77,    77,    78,    78,    78,    78,    78,    79,    79,
      79,    80,    80,    80,    81,    81,    81,    81,    82,    83,
      83,    83,    83,    84,    84,    84,    84,    84,    84,    85,
      85,    85,    85,    85,    86,    86,    86,    87,    88,    88,
      89,    89,    90,    90,    91,    91,    91,    92,    93,    93,
      94,    94,    95,    95,    96,    96,    96,    97,    97,    97,
      97,    97,    97,    97
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     2,     2,     2,     2,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     3,     1,     1,     1,     5,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     3,     3,     1,     1,     3,     1,     3,     1,
       3,     0,     3,     1,     2,     2,     1,     4,     1,     0,
       2,     0,     2,     0,     2,     1,     0,     1,     1,     1,
       1,     2,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,    12,     0,    99,     0,     0,     9,    10,
      14,    11,    91,    32,    13,    15,    17,    18,    19,    20,
      21,    22,    24,    25,    26,    27,    28,    29,    30,    31,
       2,    16,    34,    96,   101,    98,     4,    93,     0,     7,
       6,     0,     5,     0,   103,     0,    94,    99,     0,    95,
      87,    84,     0,     0,     0,    74,    75,    76,    73,    77,
      78,    89,    38,    40,    42,    44,    46,    48,    50,    53,
      58,    61,    64,    68,     0,    69,    79,    85,     0,     0,
      36,    37,   106,     8,   100,    92,     0,    70,    71,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    72,
       0,     0,     0,    90,     0,    33,     0,   110,   107,   113,
       0,   108,   109,    23,   112,   102,   105,     0,    86,    41,
       0,    43,    45,    47,    49,    51,    52,    56,    57,    54,
      55,    59,    60,    62,    63,    66,    65,    67,    82,     0,
      83,     0,    88,    35,   111,   104,    97,     0,    81,    80,
      39
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    30,   124,    44,    79,    80,    81,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    42,    36,    37,    49,    38,
      46,    83,   125,   126
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -73
static const yytype_int8 yypact[] =
{
     -73,   100,   -73,   -73,    -2,    -1,     8,    21,   -73,   -73,
     -73,   -73,   -37,   -73,   -73,   -73,   -73,   -73,   -73,   -73,
     -73,   -73,   -73,   -73,   -73,   -73,   -73,   -73,   -73,   -73,
     -73,   -73,    -5,   -73,    46,   -73,    52,   -73,    58,   -73,
     -73,    36,   -73,    36,    57,    72,   -73,    -1,    37,   -73,
     -73,   -73,    36,    36,    36,   -73,   -73,   -73,   -73,   -73,
     -73,   -73,   -12,    63,    22,    20,    23,   -19,   -24,    -9,
     -22,   -36,   -73,   -73,    36,   -32,   -73,   -73,    -8,     4,
     -73,   -73,   118,   -73,   -73,   -73,    70,   -73,   -73,    41,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,   -73,
       2,    73,    36,   -73,    36,   -73,    36,   -73,   -73,   -73,
      74,   -73,   -73,   -73,   -73,   118,   -73,    45,   -73,    63,
      33,    22,    20,    23,   -19,   -24,   -24,    -9,    -9,    -9,
      -9,   -22,   -22,   -36,   -36,   -73,   -73,   -73,   -73,    10,
     -73,    38,   -73,   -73,   -73,   -73,   -73,    36,   -73,   -73,
     -73
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -73,   -73,   -73,    90,   -73,   -73,   -53,   -41,   -73,     5,
       0,     1,     3,     6,   -56,   -72,   -46,   -44,   -58,    14,
     -73,   -73,   -73,   -73,    -6,   -73,   -73,    55,   -73,   -73,
     -73,   -73,   -73,   -14
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      61,    89,    33,    41,    90,    50,    98,    99,   110,    32,
      34,   111,    35,    51,   106,   107,   109,    96,    97,    39,
     100,   101,   112,   102,   103,   108,   137,   138,   139,   140,
     104,   105,    40,   113,   114,    43,    52,    53,   130,    50,
     135,   136,    54,   148,    91,   115,   116,    51,   145,   146,
     147,   158,   114,    55,    56,    57,   141,   142,    45,   151,
     143,   144,    58,   153,    59,    60,    87,    88,    47,    61,
      52,    53,    48,   152,    82,    84,    54,    86,    92,    94,
      93,   127,   128,    95,   150,   154,   156,    55,    56,    57,
     157,    31,   131,   159,   132,   129,    58,   133,    59,    60,
       2,   134,    85,     3,   149,     4,     5,     6,     7,     8,
       9,   155,     0,     0,     0,    10,   160,     0,    11,    12,
       0,   117,     0,     0,     0,     0,     0,     0,     0,   118,
       0,     0,     0,     0,     0,   119,     0,     0,    13,     0,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    13,   120,   121,   122,
     123,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-73)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      41,    54,     3,    40,    16,     3,    30,    31,    40,    11,
      11,    43,    13,    11,    50,    51,    74,    36,    37,    11,
      44,    45,    54,    32,    33,    61,    98,    99,   100,   101,
      52,    53,    11,    41,    42,    40,    34,    35,    91,     3,
      96,    97,    40,    41,    56,    41,    42,    11,   106,   107,
     108,    41,    42,    51,    52,    53,   102,   103,    12,   112,
     104,   105,    60,   116,    62,    63,    52,    53,    16,   110,
      34,    35,    14,   114,    17,     3,    40,    40,    15,    59,
      58,    11,    41,    60,    11,    11,    41,    51,    52,    53,
      57,     1,    92,    55,    93,    90,    60,    94,    62,    63,
       0,    95,    47,     3,   110,     5,     6,     7,     8,     9,
      10,   125,    -1,    -1,    -1,    15,   157,    -1,    18,    19,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    38,    -1,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    65,     0,     3,     5,     6,     7,     8,     9,    10,
      15,    18,    19,    38,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      66,    67,    11,     3,    11,    13,    90,    91,    93,    11,
      11,    40,    89,    40,    68,    12,    94,    16,    14,    92,
       3,    11,    34,    35,    40,    51,    52,    53,    60,    62,
      63,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    69,
      70,    71,    17,    95,     3,    91,    40,    83,    83,    70,
      16,    56,    15,    58,    59,    60,    36,    37,    30,    31,
      44,    45,    32,    33,    52,    53,    50,    51,    61,    82,
      40,    43,    54,    41,    42,    41,    42,     3,    11,    17,
      39,    40,    41,    42,    67,    96,    97,    11,    41,    73,
      70,    74,    75,    76,    77,    78,    78,    79,    79,    79,
      79,    80,    80,    81,    81,    82,    82,    82,    41,    88,
      11,    70,    71,    70,    11,    97,    41,    57,    41,    55,
      71
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
/* Line 1792 of yacc.c  */
#line 161 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    { // EOL is end of an expression
						//printf("\n= %d\n", $1);
	}
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 166 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			string var=(yyvsp[(2) - (2)]).str;
			int num=(yyvsp[(2) - (2)]).num;
			Test test=(yyvsp[(2) - (2)]).test;
			apply_condition(var,(yyvsp[(2) - (2)]).num!=0);
		}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 173 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			if(inActiveBlock())
			{
				string macro_name=(yyvsp[(1) - (2)]).str;
				string str=ProcessMacro(macro_name,(yyvsp[(2) - (2)]).strs);
				//Write(str.c_str());
				const char *p = str.c_str();
				const char *q = p + strlen(p);
				while(q > p)
					Unput(*--q);
			}
		}
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 186 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			string id=(yyvsp[(2) - (2)]).str;
			ifndef(id);
		}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 191 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			string id=(yyvsp[(2) - (2)]).str;
			ifdef(id);
		}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 196 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
	GLFX_ERRNO_BREAK
			string id=(yyvsp[(2) - (4)]).str;
			vector<string> def=(yyvsp[(4) - (4)]).strs;
			string params=(yyvsp[(3) - (4)]).str;
			if(inActiveBlock())
				DefineMacro(id,params,def);
			std::ostringstream definition;
			definition<<"#define "<<id;
			if(params.length())
				definition<<"("<<params<<")";
		if (def.size())
			definition << " ";
			for(int i=0;i<def.size();i++)
			definition<<def[i];
			definition<<"\n";
	GLFX_ERRNO_BREAK
			if(inActiveBlock())
				Write(definition.str().c_str());
		}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 217 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			hash_else();
		}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 221 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			endif();
		}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 225 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
		}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 228 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			Write((yyvsp[(1) - (1)]).str.c_str());
		}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 232 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			Write((yyvsp[(1) - (1)]).str.c_str());
		}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 236 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			Write((yyvsp[(1) - (1)]).str.c_str());
		}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 240 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			Write((yyvsp[(1) - (1)]).str.c_str());
		}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 244 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			Write((yyvsp[(1) - (1)]).str.c_str());
		}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 248 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			Write((yyvsp[(1) - (1)]).str.c_str());
		}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 252 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 256 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 260 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 264 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 268 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 272 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 276 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 280 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 284 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 288 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 292 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 296 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 300 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 304 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 308 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str=(yyvsp[(1) - (1)]).str;
		}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 312 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(2) - (3)]).str;
				 }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 316 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str="";
				 }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 320 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				string s=(yyvsp[(1) - (3)]).str+",";
				s+=(yyvsp[(3) - (3)]).str;
				(yyval).str=s;
			 }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 327 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				 (yyval).str=(yyvsp[(1) - (1)]).str;
			 }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 331 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str=(yyvsp[(1) - (1)]).str;
					}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 335 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
		(yyval).str=(yyvsp[(1) - (1)]).str;
	}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 339 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
		(yyval).str=(yyvsp[(1) - (5)]).str+(yyvsp[(2) - (5)]).str+(yyvsp[(3) - (5)]).str+(yyvsp[(4) - (5)]).str+(yyvsp[(5) - (5)]).str;
	}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 344 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (1)]).str;
				}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 348 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (3)]).str;
				}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 352 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (1)]).str;
				}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 356 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
				}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 360 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (1)]).str;
				}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 364 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
				}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 368 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (1)]).str;
				}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 372 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
				}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 376 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
	(yyval).str=(yyvsp[(1) - (1)]).str;
}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 380 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
	(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 385 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
			}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 389 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 393 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 397 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
			}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 401 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 405 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 409 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 413 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 417 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
			}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 421 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 425 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 429 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
			}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 433 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 437 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 441 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
			}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 445 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 449 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 453 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 457 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
			}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 461 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
			}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 465 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (2)]).str+(yyvsp[(2) - (2)]).str;
			}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 469 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (2)]).str+(yyvsp[(2) - (2)]).str;
			}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 473 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (2)]).str+(yyvsp[(2) - (2)]).str;
			}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 477 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
			}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 481 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str=(yyvsp[(1) - (1)]).str;
					}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 485 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str=(yyvsp[(1) - (4)]).str+(yyvsp[(2) - (4)]).str+(yyvsp[(3) - (4)]).str+(yyvsp[(4) - (4)]).str;
					}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 489 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str=(yyvsp[(1) - (4)]).str+(yyvsp[(2) - (4)]).str+(yyvsp[(3) - (4)]).str+(yyvsp[(4) - (4)]).str;
					}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 493 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
					}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 497 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
					}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 502 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (1)]).str;
				}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 506 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (1)]).str;
				}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 510 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
				}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 514 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (1)]).str;
				}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 518 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				string &el=(yyvsp[(3) - (3)]).str;
				vector<string> &defs=(yyval).strs;
				defs=(yyvsp[(1) - (3)]).strs;
				defs.push_back(el);
				(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
					}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 526 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				string &el=(yyvsp[(1) - (1)]).str;
				vector<string> &defs=(yyval).strs;
				defs.clear();
				defs.push_back(el);
				(yyval).str=el;
					}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 534 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str=(yyvsp[(1) - (3)]).str+(yyvsp[(2) - (3)]).str+(yyvsp[(3) - (3)]).str;
					(yyval).strs=(yyvsp[(2) - (3)]).strs;
				 }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 539 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
					(yyval).str="";
					(yyval).strs.clear();
				 }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 545 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).num=(yyvsp[(1) - (3)]).num|(yyvsp[(3) - (3)]).num;
				(yyval).str=((yyvsp[(1) - (3)]).str+"|")+(yyvsp[(3) - (3)]).str;
			}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 550 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
				(yyval).str=(yyvsp[(1) - (1)]).str;
				(yyval).num=(yyvsp[(1) - (1)]).num;
			}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 555 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						string id=(yyvsp[(1) - (2)]).str;
						int num=(yyvsp[(2) - (2)]).num;
						Test test=(yyvsp[(2) - (2)]).test;
						(yyval).num=hash_if(id,test,num);
						(yyval).str=(yyvsp[(1) - (2)]).str+(yyvsp[(2) - (2)]).str;
					}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 563 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).test=(yyvsp[(1) - (2)]).test;
						Test test=(yyvsp[(1) - (2)]).test;
						string id=(yyvsp[(1) - (2)]).str+(yyvsp[(2) - (2)]).str;
						(yyval).num=hash_if(id,(yyvsp[(1) - (2)]).test);
						(yyval).str=id;
					}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 571 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).test=NONZERO;
						string id=(yyvsp[(1) - (1)]).str;
						(yyval).num=((yyvsp[(1) - (1)]).num!=0);
						(yyval).str=id;
					}
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 578 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str="defined(";
						(yyval).str+=(yyvsp[(3) - (4)]).str;
						(yyval).str+=")";
					}
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 584 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str="!";
						(yyval).test=NOT_DEFINED;
					}
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 589 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str="";
						(yyval).test=IS_DEFINED;
					}
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 595 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).num=(yyvsp[(2) - (2)]).num;
						(yyval).test=EQUALS;
						(yyval).str="==";
						(yyval).str+=(yyvsp[(2) - (2)]).str;
					}
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 602 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						(yyval).str="";
						(yyval).num=1;
						(yyval).test=NONZERO;
					}
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 608 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).str = "";
			(yyval).strs= (yyvsp[(2) - (2)]).strs;
		}
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 613 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
			(yyval).strs.clear();
			(yyval).str = "";
		}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 618 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
							string &el=(yyvsp[(2) - (2)]).str;
							vector<string> &defs=(yyval).strs;
							defs=(yyvsp[(1) - (2)]).strs;
							defs.push_back(el);
					  }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 626 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
							string &el=(yyvsp[(1) - (1)]).str;
							vector<string> &defs=(yyval).strs;
							defs.clear();
							defs.push_back(el);
					  }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 633 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						  (yyval).strs.clear();
					  }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 637 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						  (yyval).str=(yyvsp[(1) - (1)]).str;
					  }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 641 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						  (yyval).str=(yyvsp[(1) - (1)]).str;
					  }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 645 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						  (yyval).str=(yyvsp[(1) - (1)]).str;
					  }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 649 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
						  (yyval).str=(yyvsp[(1) - (1)]).str;
						}
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 653 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
							(yyval).str = (yyvsp[(1) - (2)]).str+(yyvsp[(2) - (2)]).str;
						}
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 657 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
							(yyval).str=(yyvsp[(1) - (1)]).str;
						}
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 661 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"
    {
							(yyval).str=(yyvsp[(1) - (1)]).str;
						}
    break;


/* Line 1792 of yacc.c  */
#line 2608 "src/generated/PreprocessParser.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 664 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\Preprocessor.ypp"


extern std::string currentFilename;
void prepro_error(const char *errmsg)
{
	using namespace std;
    ostringstream errMsg;
	int lex_linenumber=prepro_get_lineno();
	int true_linenumber=lex_linenumber;//+last_linenumber-global_linenumber;
	const char *txt=prepro_get_text();
	if(txt&&*txt)
		errMsg<<currentFilename<<"("<< true_linenumber<<") : error: \""<< txt<<"\" Preprocessor "<<errmsg;
	else
		errMsg<<currentFilename<<"("<< true_linenumber<<") : error: Preprocessor "<<errmsg;
	std::cerr<<errMsg.str().c_str()<<std::endl;
  //  throw std::runtime_error(errMsg.str());
	preproError=true;
}