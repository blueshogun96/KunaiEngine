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
#define yyparse         glfxparse
#define yylex           glfxlex
#define yyerror         glfxerror
#define yylval          glfxlval
#define yychar          glfxchar
#define yydebug         glfxdebug
#define yynerrs         glfxnerrs

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
//#line 4 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"

	//#define YYFPRINTF glfxprintf
	#define YYDEBUG 1
    #include <iostream>
    #include <map>
    #include <string>
    #include <vector>
    #include <sstream>
    #include <algorithm>
    
    #include "glfxParser.h"
    #include "glfxClasses.h"
    #include "StringFunctions.h"

    using namespace std;
    using namespace glfxParser;
    
    #ifdef _MSC_VER
		// MS's C++ compiler is more strict than gcc
    
		// bison related
		// Disable the warning about default being present but no other cases in switch
		#pragma warning( disable:4065 )

		// No unistd.h (for flex)
		#define YY_NO_UNISTD_H
		#define YYERROR_VERBOSE
    #endif
    #include "glfxScanner.h"
    #include "glfxEffect.h"

    static void errSyn(const char *e);
    static void errSem(const string& str, int line=-1);

	RasterizerState buildRasterizerState;
	Struct buildStruct;
	PassState buildPassState;
	BlendState buildBlendState;
	DepthStencilState buildDepthStencilState;
	SamplerState buildSamplerState;
	TechniqueGroup buildTechniqueGroup;
	static int last_linenumber=0;
	static int current_filenumber=0;
	static int global_linenumber=0;
	ComputeLayout currentCsLayout;
	int maxVertexCount			=0;
	int last_recorded_linenum	=0;
	int global_linenum			=0;
	int filenum					=0;
	#pragma optimize("",off)
	bool is_equal(const string& A, const char * b)
	{
		string a = A;
		while (a.length()>0 && a[0] == '\n' || a[0] == ' ' || a[0] == '\t')
			a = a.substr(1, a.length() - 1);
		while (a.length()>0&&a[a.length() - 1] == '\n' || a[a.length() - 1] == ' ' || a[a.length() - 1] == '\t')
			a = a.substr(0, a.length() - 1);
		size_t sz = a.size();
		if (strlen(b) != sz)
			return false;
		for (size_t i = 0; i < sz; ++i)
			if (tolower(a[i]) != tolower(b[i]))
				return false;
		return true;
	}
	bool toBool(const string &str)
	{
		if(is_equal(str,"TRUE"))
			return true;
		else if(is_equal(str,"FALSE"))
			return false;
		else errSem("Expected boolean.");
		return false;
	}
	float toFloat(const string &str)
	{
		return (float)atof(str.c_str());
	}
	void WriteLineNumber(std::ostringstream &str,int fileno,int lineno)
	{
		str<<"#line "<< lineno << " " << fileno<<endl;
	}
	void WriteLineNumber(std::ostringstream &str,int lineno)
	{
		WriteLineNumber(str,current_filenumber,lineno);//.c_str()<<"\""<<endl;
	}
	int CurrentFilenumber()
	{
		return current_filenumber;
	}
	int LineNumberFromYYLine(int yylineno)
	{
		return yylineno+last_linenumber-global_linenumber;
	}
	bool write_line_number_next=false;
	void gSkip()
	{
		write_line_number_next=true;
	}
	void stringReplaceAll(std::string& str, const std::string& from, const std::string& to)
	{
		if(from.empty())
			return;
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}
	static string GetLayoutForTemplateType(string s,string textureName)
	{
		// In HLSL, vec4 and float textures are "adaptable" and the actual texture might be a different format.
		//if(s==string("vec4")||s==string("float"))
		{
			// We want to be able to write both, so we will add in a #define rgba16_or_32f rgba32f or rgba16f
			// at the appropriate time.
			return string("layout(format_for_")+textureName+")";
		}
		return "";
	}
	static string GetEquivalentGLTextureType(string texture_type,string template_type)
	{
		stringReplaceAll(texture_type,"RWTexture","image");
		stringReplaceAll(texture_type,"Texture","sampler");
		if(template_type.find("uint")<template_type.length())
		{
			texture_type=string("u")+texture_type;
		}
		else if(template_type.find("int")<template_type.length())
		{
			texture_type=string("i")+texture_type;
		}
		return texture_type;
	}
	int GetIndexOfParameter(const vector<glfxstype::variable> &parameters,const string &name)
	{
		for(int i=0;i<parameters.size();i++)
		{
			if(name==parameters[i].identifier)
				return i;
		}
		return -1;
	}
	string getGLInputSemantic(int s);
	map<std::string,CompilableShader*> compilableShaders;
	
	Function buildFunction;
	TextureSampler *AddTextureSampler(string textureName,string samplerStateName)
	{
		auto i=buildFunction.textureSamplers.find((textureName+"_")+samplerStateName);
		if(i!=buildFunction.textureSamplers.end())
		{
			return i->second;
		}
		TextureSampler *ts=new TextureSampler;
		ts->textureName			=textureName;
		ts->samplerStateName	=samplerStateName;
		ts->global				=false;
		buildFunction.textureSamplers[ts->textureSamplerName()]=(ts);
		buildFunction.textureSamplersByTexture[textureName].insert(ts);
		buildFunction.textureSamplersBySampler[ts->samplerStateName].insert(ts);
		return ts;
	}
	string ComputeLayout::text() const
	{
		string tt=stringFormat("layout(local_size_x=%d,local_size_y=%d,local_size_z=%d) in;",x,y,z);
		return tt;
	}
	string ShaderTypeToString(ShaderType s)
	{
		switch(s)
		{
		case VERTEX_SHADER:
		return "Vertex";
		case TESSELATION_CONTROL_SHADER:		//= Hull shader
		return "Hull (TC)";
		case TESSELATION_EVALUATION_SHADER:	//= Domain Shader
		return "Domain (TE)";
		case GEOMETRY_SHADER:
		return "Geometry";
		case FRAGMENT_SHADER:
		return "Pixel (Fragment)";
		case COMPUTE_SHADER:
		return "Compute";
		default:
		return "Unknown";
		}
	};
	GLTextureType GetTextureType(const Function &function,const string &textureName)
	{
		for(vector<glfxstype::variable>::const_iterator i=function.parameters.begin();i!=function.parameters.end();i++)
		{
			if(textureName==i->identifier)
				return (GLTextureType)i->type_enum;
		}
		auto j = gEffect->GetDeclaredTextures().find(textureName);
		if (j != gEffect->GetDeclaredTextures().end())
			return (GLTextureType)j->second->type_enum;
		return unknwownTextureType;
	}
	GLTextureType ToGLTextureType(int t);
	bool IsRWTexture(GLTextureType glTextureType)
	{
		switch(glTextureType)
		{
			case gimage1D:
			case gimage2D:
			case gimage3D:
			case gimageCube:
			case gimage2DRect:
			case gimage1DArray:
			case gimage2DArray:
			case gimageCubeArray:
			case gimageBuffer:
			case gimage2DMS:
			case gimage2DMSArray:
				return true;
				break;
			default:
				return false;
		break;
		}
	}
	string GetSizeFunction(string textureName,string x,string y,string z)
	{
		GLTextureType glTextureType	=GetTextureType(buildFunction,textureName);
		bool lod_param=false;
		switch(glTextureType)
		{
			case gsampler1D:
			case gsampler2D:
			case gsampler3D:
			case gsamplerCube:
			case sampler1DShadow:
			case sampler2DShadow:
			case samplerCubeShadow:
			case gsamplerCubeArray:
			case gsampler1DArray:
			case gsampler2DArray:
			case sampler1DArrayShadow:
			case sampler2DArrayShadow:
				lod_param=true;
			default:
				break;
		}
		int return_size=GetTextureDimension(glTextureType);
		ostringstream full_expr;
		// NOTE the spaces around the textureName. We might need to replace it with a sampler name, so we make sure we can find it
		// as a whole word easily.
		full_expr<<"{ivec"<<return_size<<" iv=";
		if(IsRWTexture(glTextureType))
		{
			full_expr<<"imageSize";
		}
		else
			full_expr<<"textureSize";
		full_expr<<"( "<<textureName<<" ";
		if(lod_param)
			full_expr<<",0";
		full_expr<<"); "<<x<<"=iv.x;";
		if(return_size>1&&y.size()>0)
			full_expr<<y<<"=iv.y;";
		if(return_size>2&&z.size()>0)
			full_expr<<z<<"=iv.z;";
		full_expr<<"}\n";
		return full_expr.str();
	}
	bool IsDeclared(string name)
	{
		if(gEffect->IsDeclared(name))
			return true;
		for(auto i=buildFunction.parameters.begin();i!=buildFunction.parameters.end();i++)
		{
			if(i->identifier==name)
				return true;
		}
		return false;
	}
	string GetDeclaredType(string name)
	{
		if(gEffect->IsDeclared(name))
			return gEffect->GetDeclaredType(name);
		for(auto i=buildFunction.parameters.begin();i!=buildFunction.parameters.end();i++)
		{
			if(i->identifier==name)
				return i->type;
		}
		return "unknown";
	}
	bool IsBufferType(int t);

/* Line 371 of yacc.c  */
#line 368 "src/generated/glfxLALRParser.cpp"

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
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "glfxLALRParser.hpp".  */
#ifndef YY_GLFX_SRC_GENERATED_GLFXLALRPARSER_HPP_INCLUDED
# define YY_GLFX_SRC_GENERATED_GLFXLALRPARSER_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int glfxdebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     WHITESPACE = 258,
     UNIFORM = 259,
     REGISTER = 260,
     SHADER = 261,
     CONSTANT_BUFFER = 262,
     PROGRAM = 263,
     INTERFACE = 264,
     IDENTIFIER = 265,
     SAMPLE = 266,
     SAMPLE_LOD = 267,
     LOAD = 268,
     GET_DIMS = 269,
     STORAGEQ = 270,
     GL_FRAGDEPTH = 271,
     SHADER_COMMAND = 272,
     SET_RASTERIZER_COMMAND = 273,
     SET_DEPTH_COMMAND = 274,
     SET_BLEND_COMMAND = 275,
     SV_DISPATCHTHREADID = 276,
     SV_GROUPTHREADID = 277,
     SV_GROUPID = 278,
     SV_VERTEXID = 279,
     NUM = 280,
     INCLUDE = 281,
     SAMPLER = 282,
     P_FLOAT = 283,
     LAYOUT = 284,
     LINE = 285,
     TECHNIQUE = 286,
     PASS = 287,
     GROUP = 288,
     RENDER_STATE_DECL = 289,
     QUOTED_STRING = 290,
     SHADER_LINE = 291,
     SHADER_TYPE = 292,
     COMPILE_SHADER = 293,
     CONSTRUCT_GS_WITH_SO = 294,
     STRUCT = 295,
     PRAGMA = 296,
     DEFINE = 297,
     UNDEF = 298,
     MACRO_DEFINITION = 299,
     EOL = 300,
     DECL_SHADER = 301,
     PROFILE = 302,
     WARNING = 303,
     DEF = 304,
     MESSAGE = 305,
     PACK_MATRIX = 306,
     RW_TEXTURE2D = 307,
     RW_TEXTURE3D = 308,
     RW_TEXTURE2DARRAY = 309,
     RW_TEXTURE3D_FLOAT4 = 310,
     TEXTURE2DMS = 311,
     TEXTURE2D = 312,
     TEXTURE2DARRAY = 313,
     TEXTURE3D = 314,
     TEXTURECUBE = 315,
     IMAGE3D = 316,
     KNOWN_TYPE = 317,
     STRUCTUREDBUFFER = 318,
     RW_STRUCTUREDBUFFER = 319,
     RETURN = 320,
     BREAK = 321,
     CONTINUE = 322,
     DO = 323,
     FOR = 324,
     GOTO = 325,
     IF = 326,
     WHILE = 327,
     ELSE = 328,
     TIMES_EQUALS = 329,
     OVER_EQUALS = 330,
     PERCENT_EQUALS = 331,
     PLUS_EQUALS = 332,
     MINUS_EQUALS = 333,
     LEFT_SHIFT_EQUALS = 334,
     RIGHT_SHIFT_EQUALS = 335,
     AND_EQUALS = 336,
     XOR_EQUALS = 337,
     OR_EQUALS = 338,
     AND = 339,
     OR = 340,
     LESS_EQ = 341,
     GRTR_EQ = 342,
     LEFT_SHIFT = 343,
     RIGHT_SHIFT = 344,
     PLUS_PLUS = 345,
     MINUS_MINUS = 346,
     EQ_EQ = 347,
     NOT_EQ = 348,
     SA_MAXVERTEXCOUNT = 349,
     SA_NUMTHREADS = 350,
     SATURATE = 351,
     SHADER_PROFILE = 352,
     COMMENT = 353,
     MIN_MAG_MIP_POINT = 354,
     MIN_MAG_POINT_MIP_LINEAR = 355,
     MIN_POINT_MAG_LINEAR_MIP_POINT = 356,
     MIN_POINT_MAG_MIP_LINEAR = 357,
     MIN_LINEAR_MAG_MIP_POINT = 358,
     MIN_LINEAR_MAG_POINT_MIP_LINEAR = 359,
     MIN_MAG_LINEAR_MIP_POINT = 360,
     MIN_MAG_MIP_LINEAR = 361,
     ANISOTROPIC = 362,
     SRC_ALPHA = 363,
     INV_SRC_ALPHA = 364,
     ZERO = 365,
     ONE = 366,
     SRC_COLOR = 367,
     INV_SRC_COLOR = 368,
     DEST_ALPHA = 369,
     INV_DEST_ALPHA = 370,
     DEST_COLOR = 371,
     INV_DEST_COLOR = 372,
     SRC_ALPHA_SAT = 373,
     BLEND_FACTOR = 374,
     INV_BLEND_FACTOR = 375,
     SRC1_COLOR = 376,
     INV_SRC1_COLOR = 377,
     SRC1_ALPHA = 378,
     INV_SRC1_ALPHA = 379,
     BLEND_OP_ADD = 380,
     BLEND_OP_SUBTRACT = 381,
     BLEND_OP_MAX = 382,
     CULL_FRONT_FACES = 383,
     CULL_BACK_FACES = 384,
     RENDERSTATE_RVALUE_NONE = 385,
     FILL_MODE_SOLID = 386,
     FILL_MODE_WIREFRAME = 387,
     DEPTH_PASS_ALWAYS = 388,
     DEPTH_PASS_NEVER = 389,
     DEPTH_PASS_LESS = 390,
     DEPTH_PASS_GREATER = 391,
     DEPTH_PASS_LESS_EQUAL = 392,
     DEPTH_PASS_GREATER_EQUAL = 393,
     TEXTURE_WRAP = 394,
     TEXTURE_MIRROR = 395,
     TEXTURE_CLAMP = 396
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE glfxlval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int glfxparse (void *YYPARSE_PARAM);
#else
int glfxparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int glfxparse (void);
#else
int glfxparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_GLFX_SRC_GENERATED_GLFXLALRPARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 575 "src/generated/glfxLALRParser.cpp"

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
#define YYLAST   701

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  166
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  110
/* YYNRULES -- Number of rules.  */
#define YYNRULES  319
/* YYNRULES -- Number of states.  */
#define YYNSTATES  614

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   396

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   164,     2,     2,     2,   162,   158,     2,
     142,   144,   154,   159,   148,   160,   165,   161,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   143,   145,
     152,   149,   153,   155,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   146,     2,   147,   157,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   150,   156,   151,   163,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    11,    19,    26,    32,
      37,    44,    55,    60,    63,    67,    71,    75,    79,    84,
      90,    97,   103,   108,   116,   125,   135,   143,   145,   153,
     160,   165,   168,   170,   172,   174,   176,   178,   180,   182,
     184,   186,   188,   190,   194,   195,   203,   205,   207,   210,
     212,   218,   226,   230,   234,   238,   240,   242,   244,   250,
     254,   256,   258,   259,   260,   261,   262,   266,   269,   270,
     274,   277,   279,   281,   283,   285,   287,   294,   296,   297,
     301,   303,   304,   308,   312,   313,   315,   316,   320,   325,
     327,   329,   331,   333,   334,   336,   338,   340,   342,   345,
     348,   351,   352,   354,   357,   359,   365,   367,   370,   372,
     375,   377,   388,   394,   402,   412,   417,   423,   425,   432,
     441,   446,   451,   452,   455,   456,   460,   462,   465,   467,
     468,   473,   476,   478,   482,   484,   488,   490,   494,   499,
     503,   507,   509,   511,   514,   516,   519,   521,   527,   535,
     541,   549,   559,   563,   566,   569,   573,   575,   577,   579,
     580,   582,   583,   587,   589,   593,   595,   599,   601,   603,
     605,   607,   609,   611,   613,   615,   617,   619,   621,   623,
     629,   631,   635,   637,   641,   643,   647,   649,   653,   655,
     659,   661,   665,   669,   671,   675,   679,   683,   687,   689,
     693,   697,   699,   703,   707,   709,   713,   717,   721,   723,
     725,   728,   731,   734,   736,   738,   740,   742,   744,   746,
     748,   750,   752,   756,   760,   764,   766,   771,   778,   787,
     792,   799,   804,   808,   812,   815,   818,   822,   824,   826,
     830,   835,   837,   841,   843,   845,   847,   851,   853,   855,
     858,   861,   862,   868,   869,   872,   875,   876,   880,   881,
     884,   886,   891,   893,   896,   900,   901,   903,   905,   907,
     909,   911,   913,   915,   917,   919,   921,   923,   925,   927,
     929,   931,   933,   935,   937,   939,   941,   943,   945,   947,
     949,   951,   953,   955,   957,   959,   961,   963,   965,   967,
     969,   971,   973,   975,   977,   979,   981,   983,   985,   987,
     989,   991,   993,   995,   997,   999,  1001,  1003,  1005,  1007
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     167,     0,    -1,   167,   168,    -1,    -1,    45,    -1,   169,
      -1,    41,    48,   142,   176,   143,    25,   144,    -1,   197,
     172,   174,    10,   260,   145,    -1,   197,   173,   174,    10,
     145,    -1,   197,    10,    10,   145,    -1,   146,    94,   142,
      25,   144,   147,    -1,   146,    95,   142,    25,   148,    25,
     148,    25,   144,   147,    -1,    29,   180,    10,   145,    -1,
      29,   181,    -1,    55,    10,   145,    -1,    55,    10,   148,
      -1,    42,    44,    45,    -1,    43,    10,    45,    -1,    40,
      10,   193,   145,    -1,    34,    10,   260,   262,   145,    -1,
      47,    97,   142,    25,   144,   145,    -1,    46,    10,   149,
     215,   145,    -1,    29,   187,   175,   190,    -1,     7,    10,
     260,   188,   193,   145,   190,    -1,    10,    10,   142,   198,
     144,   189,   191,   190,    -1,   171,    10,    10,   142,   198,
     144,   192,   191,   190,    -1,     8,   187,    10,   150,   210,
     151,   190,    -1,   179,    -1,    33,   187,    10,   150,   177,
     151,   190,    -1,    30,   187,    25,    25,   190,    45,    -1,
      30,    25,    25,    45,    -1,     6,   188,    -1,    57,    -1,
      59,    -1,    56,    -1,    58,    -1,    61,    -1,    52,    -1,
      53,    -1,    54,    -1,    60,    -1,    63,    -1,    64,    -1,
     152,    10,   153,    -1,    -1,   142,    25,   148,    25,   148,
      25,   144,    -1,    10,    -1,    25,    -1,   177,   178,    -1,
     178,    -1,    31,    10,   150,   207,   151,    -1,    31,   187,
      10,   150,   207,   151,   190,    -1,   142,   184,   144,    -1,
     142,   182,   144,    -1,   182,   148,   183,    -1,   183,    -1,
     185,    -1,    10,    -1,   185,   148,   185,   148,   185,    -1,
      10,   149,   186,    -1,    10,    -1,    25,    -1,    -1,    -1,
      -1,    -1,   150,   220,   151,    -1,   143,    10,    -1,    -1,
     150,   194,   151,    -1,   194,   195,    -1,   195,    -1,   196,
      -1,   170,    -1,    98,    -1,     3,    -1,   197,    10,    10,
     200,   261,   145,    -1,     4,    -1,    -1,   198,   148,   199,
      -1,   199,    -1,    -1,   202,   200,   206,    -1,   146,   201,
     147,    -1,    -1,    25,    -1,    -1,    25,   154,    25,    -1,
     204,   203,   174,    10,    -1,    10,    -1,   172,    -1,   173,
      -1,    15,    -1,    -1,    21,    -1,    23,    -1,    22,    -1,
      24,    -1,   143,    25,    -1,   143,   205,    -1,   143,    10,
      -1,    -1,   208,    -1,   208,   209,    -1,   209,    -1,    32,
      10,   150,   210,   151,    -1,   211,    -1,   211,   214,    -1,
     214,    -1,   211,   213,    -1,   213,    -1,    10,   142,    28,
     148,    28,   148,    28,   148,    28,   144,    -1,    18,   142,
      10,   144,   145,    -1,    19,   142,    10,   148,    25,   144,
     145,    -1,    20,   142,    10,   148,   212,   148,    25,   144,
     145,    -1,   216,   149,   217,   145,    -1,    17,   142,   215,
     144,   145,    -1,    10,    -1,    39,   142,    10,   148,    35,
     144,    -1,    38,   142,    97,   148,    10,   142,   144,   144,
      -1,    37,   142,    25,   144,    -1,    10,   142,   144,   218,
      -1,    -1,   143,   219,    -1,    -1,   219,   148,   258,    -1,
     258,    -1,   220,   226,    -1,   226,    -1,    -1,    30,    25,
      25,    45,    -1,   275,   223,    -1,   224,    -1,   223,   148,
     224,    -1,   225,    -1,   225,   149,   254,    -1,    10,    -1,
     142,   225,   144,    -1,   225,   146,    25,   147,    -1,   225,
     146,   147,    -1,   225,   142,   144,    -1,    98,    -1,     3,
      -1,   230,   145,    -1,   145,    -1,   222,   145,    -1,   229,
      -1,    71,   142,   230,   144,   226,    -1,    71,   142,   230,
     144,   226,    73,   226,    -1,    72,   142,   230,   144,   226,
      -1,    68,   226,    72,   142,   230,   144,   145,    -1,    69,
     142,   227,   145,   228,   145,   228,   144,   226,    -1,    70,
      10,   145,    -1,    67,   145,    -1,    66,   145,    -1,    65,
     228,   145,    -1,   221,    -1,   230,    -1,   222,    -1,    -1,
     230,    -1,    -1,   150,   220,   151,    -1,   231,    -1,   230,
     148,   231,    -1,   233,    -1,   245,   232,   231,    -1,   149,
      -1,    74,    -1,    75,    -1,    76,    -1,    77,    -1,    78,
      -1,    79,    -1,    80,    -1,    81,    -1,    82,    -1,    83,
      -1,   234,    -1,   234,   155,   230,   143,   233,    -1,   235,
      -1,   234,    85,   235,    -1,   236,    -1,   235,    84,   236,
      -1,   237,    -1,   236,   156,   237,    -1,   238,    -1,   237,
     157,   238,    -1,   239,    -1,   238,   158,   239,    -1,   240,
      -1,   239,    92,   240,    -1,   239,    93,   240,    -1,   241,
      -1,   240,   152,   241,    -1,   240,   153,   241,    -1,   240,
      86,   241,    -1,   240,    87,   241,    -1,   242,    -1,   241,
      88,   242,    -1,   241,    89,   242,    -1,   243,    -1,   242,
     159,   243,    -1,   242,   160,   243,    -1,   244,    -1,   243,
     154,   244,    -1,   243,   161,   244,    -1,   243,   162,   244,
      -1,   245,    -1,   252,    -1,    90,   245,    -1,    91,   245,
      -1,   246,   244,    -1,   158,    -1,   154,    -1,   159,    -1,
     160,    -1,   163,    -1,   164,    -1,    11,    -1,    12,    -1,
      13,    -1,   252,   165,   248,    -1,   252,   165,   247,    -1,
     252,   165,    14,    -1,   256,    -1,   252,   146,   230,   147,
      -1,   251,   142,   231,   148,   231,   144,    -1,   251,   142,
     231,   148,   231,   148,   231,   144,    -1,   249,   142,   253,
     144,    -1,   250,   142,   231,   148,   253,   144,    -1,   252,
     142,   253,   144,    -1,   252,   142,   144,    -1,   252,   165,
      10,    -1,   252,    90,    -1,   252,    91,    -1,   253,   148,
     231,    -1,   231,    -1,   231,    -1,   150,   255,   151,    -1,
     150,   255,   148,   151,    -1,   254,    -1,   255,   148,   254,
      -1,    10,    -1,    96,    -1,   257,    -1,   142,   230,   144,
      -1,    25,    -1,    28,    -1,    15,   259,    -1,    16,   259,
      -1,    -1,   143,     5,   142,    10,   144,    -1,    -1,   143,
      10,    -1,   143,   205,    -1,    -1,   150,   263,   151,    -1,
      -1,   263,   264,    -1,   264,    -1,   265,   149,   267,   145,
      -1,   170,    -1,    10,   266,    -1,   146,    25,   147,    -1,
      -1,    10,    -1,    25,    -1,    28,    -1,   268,    -1,   269,
      -1,   270,    -1,   272,    -1,   271,    -1,   130,    -1,   273,
      -1,   274,    -1,    99,    -1,   100,    -1,   101,    -1,   102,
      -1,   103,    -1,   104,    -1,   105,    -1,   106,    -1,   107,
      -1,   108,    -1,   109,    -1,   110,    -1,   111,    -1,   112,
      -1,   113,    -1,   114,    -1,   115,    -1,   116,    -1,   117,
      -1,   118,    -1,   119,    -1,   120,    -1,   121,    -1,   122,
      -1,   123,    -1,   124,    -1,   125,    -1,   126,    -1,   127,
      -1,   132,    -1,   131,    -1,   128,    -1,   129,    -1,   133,
      -1,   134,    -1,   135,    -1,   136,    -1,   137,    -1,   138,
      -1,   139,    -1,   140,    -1,   141,    -1,    10,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   321,   321,   322,   376,   379,   382,   385,   415,   431,
     437,   441,   447,   450,   456,   462,   468,   472,   478,   493,
     525,   532,   541,   544,   561,   580,   616,   620,   623,   629,
     636,   643,   648,   648,   648,   648,   648,   648,   648,   648,
     648,   654,   654,   660,   666,   670,   684,   687,   690,   694,
     697,   701,   706,   710,   715,   719,   723,   727,   731,   739,
     745,   749,   754,   761,   768,   775,   779,   786,   791,   794,
     799,   805,   810,   818,   824,   828,   832,   851,   856,   859,
     873,   889,   894,   926,   932,   936,   942,   947,   952,   978,
     983,   988,   993,   998,  1001,  1006,  1011,  1016,  1021,  1026,
    1032,  1038,  1043,  1050,  1062,  1076,  1083,  1090,  1107,  1128,
    1137,  1146,  1155,  1159,  1165,  1173,  1181,  1254,  1266,  1333,
    1372,  1383,  1390,  1393,  1398,  1402,  1407,  1411,  1420,  1427,
    1431,  1444,  1455,  1459,  1463,  1467,  1517,  1521,  1525,  1532,
    1537,  1541,  1546,  1550,  1556,  1561,  1566,  1572,  1577,  1589,
    1594,  1599,  1605,  1610,  1615,  1620,  1635,  1641,  1645,  1650,
    1652,  1657,  1661,  1667,  1671,  1675,  1679,  1702,  1707,  1708,
    1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,  1721,  1725,
    1730,  1734,  1738,  1742,  1746,  1750,  1754,  1758,  1762,  1766,
    1771,  1775,  1779,  1783,  1787,  1791,  1795,  1799,  1803,  1807,
    1811,  1815,  1819,  1823,  1827,  1831,  1835,  1839,  1843,  1847,
    1854,  1858,  1862,  1866,  1866,  1866,  1866,  1866,  1866,  1870,
    1874,  1878,  1882,  1892,  1902,  1912,  1919,  1948,  1957,  1966,
    2015,  2067,  2155,  2161,  2168,  2172,  2176,  2184,  2192,  2197,
    2204,  2211,  2218,  2225,  2232,  2237,  2241,  2245,  2249,  2253,
    2261,  2270,  2273,  2286,  2290,  2294,  2299,  2302,  2306,  2309,
    2314,  2318,  2436,  2442,  2448,  2454,  2457,  2462,  2468,  2474,
    2479,  2484,  2489,  2494,  2499,  2505,  2510,  2515,  2520,  2525,
    2530,  2535,  2540,  2545,  2550,  2555,  2560,  2565,  2570,  2575,
    2580,  2585,  2590,  2595,  2600,  2605,  2610,  2615,  2620,  2625,
    2630,  2635,  2640,  2646,  2651,  2656,  2662,  2667,  2673,  2678,
    2683,  2688,  2693,  2698,  2703,  2708,  2714,  2719,  2724,  2729
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "WHITESPACE", "UNIFORM", "REGISTER",
  "SHADER", "CONSTANT_BUFFER", "PROGRAM", "INTERFACE", "IDENTIFIER",
  "SAMPLE", "SAMPLE_LOD", "LOAD", "GET_DIMS", "STORAGEQ", "GL_FRAGDEPTH",
  "SHADER_COMMAND", "SET_RASTERIZER_COMMAND", "SET_DEPTH_COMMAND",
  "SET_BLEND_COMMAND", "SV_DISPATCHTHREADID", "SV_GROUPTHREADID",
  "SV_GROUPID", "SV_VERTEXID", "NUM", "INCLUDE", "SAMPLER", "P_FLOAT",
  "LAYOUT", "LINE", "TECHNIQUE", "PASS", "GROUP", "RENDER_STATE_DECL",
  "QUOTED_STRING", "SHADER_LINE", "SHADER_TYPE", "COMPILE_SHADER",
  "CONSTRUCT_GS_WITH_SO", "STRUCT", "PRAGMA", "DEFINE", "UNDEF",
  "MACRO_DEFINITION", "EOL", "DECL_SHADER", "PROFILE", "WARNING", "DEF",
  "MESSAGE", "PACK_MATRIX", "RW_TEXTURE2D", "RW_TEXTURE3D",
  "RW_TEXTURE2DARRAY", "RW_TEXTURE3D_FLOAT4", "TEXTURE2DMS", "TEXTURE2D",
  "TEXTURE2DARRAY", "TEXTURE3D", "TEXTURECUBE", "IMAGE3D", "KNOWN_TYPE",
  "STRUCTUREDBUFFER", "RW_STRUCTUREDBUFFER", "RETURN", "BREAK", "CONTINUE",
  "DO", "FOR", "GOTO", "IF", "WHILE", "ELSE", "TIMES_EQUALS",
  "OVER_EQUALS", "PERCENT_EQUALS", "PLUS_EQUALS", "MINUS_EQUALS",
  "LEFT_SHIFT_EQUALS", "RIGHT_SHIFT_EQUALS", "AND_EQUALS", "XOR_EQUALS",
  "OR_EQUALS", "AND", "OR", "LESS_EQ", "GRTR_EQ", "LEFT_SHIFT",
  "RIGHT_SHIFT", "PLUS_PLUS", "MINUS_MINUS", "EQ_EQ", "NOT_EQ",
  "SA_MAXVERTEXCOUNT", "SA_NUMTHREADS", "SATURATE", "SHADER_PROFILE",
  "COMMENT", "MIN_MAG_MIP_POINT", "MIN_MAG_POINT_MIP_LINEAR",
  "MIN_POINT_MAG_LINEAR_MIP_POINT", "MIN_POINT_MAG_MIP_LINEAR",
  "MIN_LINEAR_MAG_MIP_POINT", "MIN_LINEAR_MAG_POINT_MIP_LINEAR",
  "MIN_MAG_LINEAR_MIP_POINT", "MIN_MAG_MIP_LINEAR", "ANISOTROPIC",
  "SRC_ALPHA", "INV_SRC_ALPHA", "ZERO", "ONE", "SRC_COLOR",
  "INV_SRC_COLOR", "DEST_ALPHA", "INV_DEST_ALPHA", "DEST_COLOR",
  "INV_DEST_COLOR", "SRC_ALPHA_SAT", "BLEND_FACTOR", "INV_BLEND_FACTOR",
  "SRC1_COLOR", "INV_SRC1_COLOR", "SRC1_ALPHA", "INV_SRC1_ALPHA",
  "BLEND_OP_ADD", "BLEND_OP_SUBTRACT", "BLEND_OP_MAX", "CULL_FRONT_FACES",
  "CULL_BACK_FACES", "RENDERSTATE_RVALUE_NONE", "FILL_MODE_SOLID",
  "FILL_MODE_WIREFRAME", "DEPTH_PASS_ALWAYS", "DEPTH_PASS_NEVER",
  "DEPTH_PASS_LESS", "DEPTH_PASS_GREATER", "DEPTH_PASS_LESS_EQUAL",
  "DEPTH_PASS_GREATER_EQUAL", "TEXTURE_WRAP", "TEXTURE_MIRROR",
  "TEXTURE_CLAMP", "'('", "':'", "')'", "';'", "'['", "']'", "','", "'='",
  "'{'", "'}'", "'<'", "'>'", "'*'", "'?'", "'|'", "'^'", "'&'", "'+'",
  "'-'", "'/'", "'%'", "'~'", "'!'", "'.'", "$accept", "prog", "tok",
  "line_statement", "ignore_line_statement", "shader_and_layout",
  "texture_type", "buffer_type", "optional_templateval", "layout_params",
  "warning-specifier", "technique_decls", "group_technique_decl",
  "technique_decl", "cs_layout_spec", "normal_layout_spec", "tx_layout",
  "layout_s", "cs_layout", "layout_el", "layout_def", "lex_fx",
  "shader_fx", "function_fx", "lex_passthrough", "function_content",
  "optional_return_semantic", "struct_definition", "struct_member_list",
  "struct_element", "struct_memb", "optional_uniform",
  "param_declaration_list", "param_declaration", "opt_sq", "opt_num",
  "param", "type_name", "opt_storage", "input_semantic", "location",
  "tech_def", "pass_list", "pass_def", "prog_def", "shaders_list", "vec4",
  "shader_fn", "shader_def", "shader_compile", "shader_lvalue",
  "shader_rvalue", "shader_layout", "shader_layout_list", "statement_list",
  "line_statement_in_shader", "declaration", "init_declarator_list",
  "init_declarator", "declarator", "statement", "for_init_statement",
  "optional_expression", "compound_statement", "expression",
  "assignment_exp", "assignment_operator", "conditional_exp",
  "logical_or_exp", "logical_and_exp", "inclusive_or_exp",
  "exclusive_or_exp", "and_exp", "equality_exp", "relational_exp",
  "shift_expression", "additive_exp", "mult_exp", "cast_exp", "unary_exp",
  "unary_operator", "sampling_command", "tex_load_command", "tex_load_exp",
  "sampling_exp", "get_dims_exp", "postfix_exp", "argument_exp_list",
  "initializer", "initializer_list", "primary_exp", "const", "layout",
  "read_parenthesis", "optional_register_semantic", "optional_semantic",
  "optional_render_state_list", "render_state_list",
  "render_state_command", "render_state_identifier", "optional_index",
  "render_state_rvalue", "filter_mode", "blend_mode", "blend_op",
  "fill_mode", "cull_mode", "depth_func", "texture_addressing_mode",
  "type_spec", YY_NULL
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,    40,    58,    41,    59,    91,    93,    44,    61,
     123,   125,    60,    62,    42,    63,   124,    94,    38,    43,
      45,    47,    37,   126,    33,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   166,   167,   167,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   169,
     170,   171,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   173,   173,   174,   174,   175,   176,   176,   177,   177,
     178,   179,   180,   181,   182,   182,   183,   183,   184,   185,
     186,   186,   187,   188,   189,   190,   191,   192,   192,   193,
     194,   194,   195,   195,   195,   195,   196,   197,   197,   198,
     198,   198,   199,   200,   200,   201,   201,   201,   202,   203,
     203,   203,   204,   204,   205,   205,   205,   205,   206,   206,
     206,   206,   207,   208,   208,   209,   210,   211,   211,   211,
     211,   212,   213,   213,   213,   214,   214,   215,   215,   215,
     216,   217,   217,   218,   218,   219,   219,   220,   220,   220,
     221,   222,   223,   223,   224,   224,   225,   225,   225,   225,
     225,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   227,   227,   227,
     228,   228,   229,   230,   230,   231,   231,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   233,   233,
     234,   234,   235,   235,   236,   236,   237,   237,   238,   238,
     239,   239,   239,   240,   240,   240,   240,   240,   241,   241,
     241,   242,   242,   242,   243,   243,   243,   243,   244,   245,
     245,   245,   245,   246,   246,   246,   246,   246,   246,   247,
     247,   248,   249,   250,   251,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   253,   253,   254,   254,
     254,   255,   255,   256,   256,   256,   256,   257,   257,   258,
     258,   259,   260,   260,   261,   261,   261,   262,   262,   263,
     263,   264,   264,   265,   266,   266,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   270,   270,   270,   271,   271,   272,   272,
     273,   273,   273,   273,   273,   273,   274,   274,   274,   275
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     7,     6,     5,     4,
       6,    10,     4,     2,     3,     3,     3,     3,     4,     5,
       6,     5,     4,     7,     8,     9,     7,     1,     7,     6,
       4,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     7,     1,     1,     2,     1,
       5,     7,     3,     3,     3,     1,     1,     1,     5,     3,
       1,     1,     0,     0,     0,     0,     3,     2,     0,     3,
       2,     1,     1,     1,     1,     1,     6,     1,     0,     3,
       1,     0,     3,     3,     0,     1,     0,     3,     4,     1,
       1,     1,     1,     0,     1,     1,     1,     1,     2,     2,
       2,     0,     1,     2,     1,     5,     1,     2,     1,     2,
       1,    10,     5,     7,     9,     4,     5,     1,     6,     8,
       4,     4,     0,     2,     0,     3,     1,     2,     1,     0,
       4,     2,     1,     3,     1,     3,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     5,     7,     5,
       7,     9,     3,     2,     2,     3,     1,     1,     1,     0,
       1,     0,     3,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     5,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     1,     4,     6,     8,     4,
       6,     4,     3,     3,     2,     2,     3,     1,     1,     3,
       4,     1,     3,     1,     1,     1,     3,     1,     1,     2,
       2,     0,     5,     0,     2,     2,     0,     3,     0,     2,
       1,     4,     1,     2,     3,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,    78,     1,    77,    63,     0,    62,     0,     0,    62,
      62,    62,     0,     0,     0,     0,     0,     4,     0,     0,
       0,     0,     2,     5,     0,    27,     0,    31,   253,     0,
       0,     0,     0,    13,     0,     0,     0,     0,   253,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    34,    32,    35,    33,    40,    36,    41,
      42,    44,    44,     0,    63,     0,    93,    57,     0,    55,
       0,    56,     0,     0,    65,     0,     0,     0,   258,    78,
       0,     0,    16,    17,     0,     0,    14,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
      80,    84,     0,     0,    53,     0,    52,     0,    12,     0,
      22,    65,     0,     0,     0,     0,    75,     0,    74,    73,
      78,    71,    72,     0,    18,    46,    47,     0,   117,     0,
       0,     0,     0,     0,     0,    93,     9,     0,   253,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,   110,
     108,     0,    64,    93,    86,   101,    89,    90,    91,    44,
      60,    61,    59,    54,    56,     0,     0,     0,     0,     0,
       0,   102,   104,     0,     0,    49,   265,   262,     0,   260,
       0,    19,     0,    69,    70,     0,     0,     0,     0,    21,
       0,     0,     0,     0,    43,     0,     8,     0,    65,     0,
       0,     0,     0,     0,    65,   109,   107,   122,     0,    79,
      85,     0,     0,    82,     0,     0,     0,    29,     0,    65,
     103,     0,    65,    48,     0,   263,   257,   259,     0,     0,
      84,     0,     0,     0,    20,    10,     0,    68,     7,   252,
      23,     0,     0,     0,     0,     0,    26,     0,     0,   129,
      65,     0,    83,   100,    94,    96,    95,    97,    98,    99,
      88,    58,     0,     0,    51,     0,    28,     0,   266,   267,
     268,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     308,   309,   274,   307,   306,   310,   311,   312,   313,   314,
     315,   316,   317,   318,     0,   269,   270,   271,   273,   272,
     275,   276,    30,   256,     6,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,     0,   115,   142,   243,   247,
     248,     0,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   244,   141,     0,   144,   129,   214,   213,   215,
     216,   217,   218,     0,   156,     0,   128,   146,     0,   163,
     165,   178,   180,   182,   184,   186,   188,   190,   193,   198,
     201,   204,   208,     0,     0,     0,     0,   209,   225,   245,
       0,    24,    87,     0,     0,     0,   264,   261,     0,     0,
       0,     0,     0,    67,    65,   116,   112,     0,     0,     0,
     124,     0,   243,     0,   160,   154,   153,     0,   159,     0,
       0,     0,   210,   211,     0,     0,    66,   127,   145,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     167,     0,   212,   208,     0,     0,     0,   234,   235,     0,
       0,     0,   136,     0,   131,   132,   134,    45,   105,    50,
     254,   255,    76,     0,   118,     0,    25,     0,     0,     0,
       0,   121,     0,   155,     0,   158,     0,   157,   152,     0,
       0,   246,   162,   164,   181,     0,   183,   185,   187,   189,
     191,   192,   196,   197,   194,   195,   199,   200,   202,   203,
     205,   206,   207,   166,   237,     0,     0,     0,   232,     0,
       0,   233,   219,   220,   221,   224,   223,   222,     0,     0,
       0,     0,     0,     0,    11,   113,     0,     0,   251,   251,
     123,   126,   130,     0,   161,     0,     0,     0,   229,     0,
       0,     0,   231,   226,   137,   133,   140,     0,   139,     0,
     238,   135,   119,     0,     0,   249,   250,     0,     0,     0,
     147,   149,   179,   236,     0,     0,   138,   241,     0,     0,
     114,   125,     0,   161,     0,   230,   227,     0,     0,   239,
       0,   150,     0,   148,     0,   240,   242,     0,     0,   228,
       0,   151,     0,   111
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    22,    23,   119,    24,    61,    62,    93,    74,
     127,   174,   175,    25,    32,    33,    68,    69,    70,    71,
     162,    29,    27,   208,   110,   250,   329,    80,   120,   121,
     122,   123,    99,   100,   155,   211,   101,   159,   102,   259,
     213,   170,   171,   172,   147,   148,   409,   149,   150,   131,
     151,   248,   491,   550,   363,   364,   365,   474,   475,   476,
     366,   496,   413,   367,   368,   369,   461,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   536,   537,   384,   385,   386,   387,   525,   571,   588,
     388,   389,   551,   575,    64,   399,   115,   178,   179,   180,
     225,   314,   315,   316,   317,   318,   319,   320,   321,   390
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -537
static const yytype_int16 yypact[] =
{
    -537,   510,  -537,  -537,  -537,    17,  -537,    30,  -130,  -537,
    -537,  -537,    40,    69,   103,    60,   130,  -537,   160,    83,
     172,   -40,  -537,  -537,   178,  -537,   426,  -537,    47,   198,
      93,   231,   239,  -537,   112,   235,   258,   263,    47,   129,
     138,   237,   238,   135,   146,    92,   153,   154,   287,   288,
    -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,
    -537,   148,   148,   296,  -537,   152,    -6,   158,   -81,  -537,
     164,   165,   171,   284,  -537,   293,   173,   176,   177,    38,
     183,    14,  -537,  -537,    26,   313,  -537,  -537,   314,   316,
     200,   199,   333,   335,   336,   205,   129,   141,  -537,   -46,
    -537,   202,   601,    18,  -537,   231,  -537,   339,  -537,   203,
    -537,  -537,   318,   321,     3,   208,  -537,   332,  -537,  -537,
       2,  -537,  -537,   348,  -537,  -537,  -537,   216,  -537,   219,
     221,   222,   225,   226,   218,    -6,  -537,   223,    47,   236,
     361,   241,   240,   242,   245,   246,   247,   243,   141,  -537,
    -537,   248,  -537,   376,   367,   252,  -537,  -537,  -537,   148,
    -537,  -537,  -537,  -537,  -537,   158,   251,   371,   355,   394,
     254,   318,  -537,   397,   -17,  -537,   265,  -537,    -8,  -537,
     261,  -537,   387,  -537,  -537,   403,   390,   319,   407,  -537,
     274,   273,   396,    -3,  -537,   280,  -537,   282,  -537,    26,
     419,   420,   422,   409,  -537,  -537,  -537,   425,   290,  -537,
     283,   291,   144,  -537,   429,   339,   294,  -537,   295,  -537,
    -537,   300,  -537,  -537,   416,  -537,  -537,  -537,   467,   399,
     202,   302,   303,   309,  -537,  -537,   310,   317,  -537,  -537,
    -537,   315,   323,   320,   325,   330,  -537,   346,   349,   264,
    -537,   440,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,
    -537,  -537,   456,   141,  -537,   318,  -537,   344,  -537,  -537,
    -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,
    -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,
    -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,
    -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,
    -537,  -537,  -537,  -537,   351,  -537,  -537,  -537,  -537,  -537,
    -537,  -537,  -537,   350,  -537,   487,   463,   474,   490,   290,
     356,   357,   478,   494,  -537,   362,  -537,  -537,   495,  -537,
    -537,   483,   365,   364,   366,   264,   370,   503,   379,   380,
     365,   365,  -537,  -537,   365,  -537,   264,  -537,  -537,  -537,
    -537,  -537,  -537,   127,  -537,   381,  -537,  -537,   108,  -537,
    -537,   -70,   446,   378,   385,   377,    -4,   -61,    61,    88,
    -116,  -537,    -5,   365,   404,   405,   406,   -56,  -537,  -537,
      -7,  -537,  -537,   401,   398,   408,  -537,  -537,    37,   393,
     412,   414,   417,  -537,  -537,  -537,  -537,   418,   421,   461,
     469,   535,  -537,   465,   466,  -537,  -537,   492,   373,   468,
     365,   365,  -537,  -537,    39,   161,  -537,  -537,  -537,  -537,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
    -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,
    -537,   365,  -537,  -537,   365,   365,   365,  -537,  -537,   289,
     365,   162,  -537,    -7,   470,  -537,   -43,  -537,  -537,  -537,
    -537,  -537,  -537,   471,  -537,   472,  -537,   475,   588,   592,
     147,  -537,   576,  -537,   480,  -537,   479,   466,  -537,    56,
      57,  -537,  -537,  -537,   446,   -99,   378,   385,   377,    -4,
     -61,   -61,    61,    61,    61,    61,    88,    88,  -116,  -116,
    -537,  -537,  -537,  -537,  -537,    58,   477,   481,  -537,    59,
     115,  -537,  -537,  -537,  -537,  -537,  -537,  -537,    35,    -7,
     482,   -14,   312,   484,  -537,  -537,   485,   486,  -537,  -537,
     488,  -537,  -537,   365,   365,   264,   264,   365,  -537,   365,
     365,   365,  -537,  -537,  -537,  -537,  -537,   476,  -537,   312,
    -537,  -537,  -537,   599,   489,  -537,  -537,   147,    65,   493,
     558,  -537,  -537,  -537,    66,    90,  -537,  -537,   110,   491,
    -537,  -537,   496,   365,   264,  -537,  -537,   365,   214,  -537,
     604,  -537,   498,  -537,   499,  -537,  -537,   497,   264,  -537,
     607,  -537,   500,  -537
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -537,  -537,  -537,  -537,   -94,  -537,   538,   544,   -54,  -537,
    -537,  -537,   473,  -537,  -537,  -537,  -537,   532,  -537,   -84,
    -537,   211,   584,  -537,  -111,   322,  -537,   553,  -537,   530,
    -537,   651,   528,   513,   437,  -537,  -537,  -537,  -537,   270,
    -537,   410,  -537,   501,   411,  -537,  -537,   521,   522,   502,
    -537,  -537,  -537,  -537,   324,  -537,   253,  -537,   134,   204,
    -344,  -537,  -536,  -537,  -338,  -413,  -537,   119,  -537,   250,
     249,   244,   255,   256,  -172,  -196,  -173,  -170,  -352,  -321,
    -537,  -537,  -537,  -537,  -537,  -537,  -537,  -459,  -532,  -537,
    -537,  -537,   102,   136,   -31,  -537,  -537,  -537,   505,  -537,
    -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537,  -537
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -320
static const yytype_int16 yytable[] =
{
     168,   417,   176,   472,   414,   116,     3,    78,    94,    98,
     529,   567,    31,   176,   173,   431,   424,   503,   579,   427,
     177,   164,   117,   166,   125,   439,   440,    28,   160,   422,
     423,   462,   117,   117,   467,   468,   128,   587,   447,   126,
      30,   116,     3,   161,   557,   448,   449,   480,   523,   430,
      38,   524,   526,   527,    46,    47,   524,   602,   254,   255,
     256,   257,   463,   104,   129,   130,   606,   105,   117,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,    39,
     497,   427,   499,   500,   177,   432,   469,   240,   437,   438,
     470,   441,   442,   246,   505,   520,   521,   522,   152,   540,
     118,   584,   153,   541,    41,   214,   542,   195,   264,   471,
     463,   266,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   570,
     337,   261,   530,   568,   222,   473,   118,   338,   -81,   391,
      42,   237,   -81,   226,   460,   153,   583,   524,   585,   443,
     444,    40,   339,   183,   253,   340,   570,   341,   142,   143,
     144,   145,   548,   549,   337,   254,   255,   256,   257,   258,
      43,   338,   531,   532,   533,   534,   535,   540,   146,   564,
      44,   541,    45,   501,   604,   570,   339,   430,    48,   340,
      63,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     555,   556,   558,   562,   430,   430,   559,   559,    65,   592,
     595,   580,   581,   430,   559,   578,   414,   350,   351,    34,
      35,    36,    37,   352,   412,   353,   342,   343,   344,   345,
     346,   347,   348,   349,   596,    66,   463,    86,   597,   339,
      87,    67,   340,   512,   513,   514,   515,   445,   446,    72,
     603,   350,   351,   429,    73,   414,   430,   352,   598,   353,
      75,   599,   563,   430,   611,   510,   511,   337,    76,   354,
     516,   517,   355,    77,   338,   518,   519,   356,   426,    79,
      81,   357,    82,    83,    84,   358,   359,   360,    85,   339,
     361,   362,   340,   486,   341,    88,    89,    90,    91,   412,
      92,    95,    97,   354,   350,   351,   355,   103,   106,   109,
     352,   356,   502,   107,   339,   357,   108,   340,   111,   358,
     359,   360,   412,   112,   361,   362,   113,   114,   124,   342,
     343,   344,   345,   346,   347,   348,   349,   339,   132,   133,
     340,   134,   135,   137,   136,   138,   139,   140,   154,   165,
     169,   167,   173,   181,   350,   351,   354,   182,   185,   186,
     352,   187,   353,   188,   569,   605,   192,   189,   357,   190,
     191,   197,   358,   359,   360,   412,   194,   361,   362,   350,
     351,   196,   199,   338,   200,   352,   198,   201,   202,   203,
     339,    98,   210,   340,   204,   212,   216,   207,   339,   215,
     217,   340,   350,   351,   218,   219,   354,   221,   352,   355,
     228,   224,   229,   230,   356,   231,   232,   233,   357,   234,
     235,   236,   358,   359,   360,   238,   239,   361,   362,   242,
     243,   354,   244,   528,   245,   247,    49,   251,   252,   260,
     249,   267,   262,   357,   322,   263,   324,   358,   359,   360,
     265,   325,   361,   362,   354,   350,   351,   326,   327,   330,
     328,   352,   569,   350,   351,   392,   357,   331,   332,   352,
     358,   359,   360,   333,   334,   361,   362,   268,    50,    51,
      52,   393,    53,    54,    55,    56,    57,    58,   335,    59,
      60,   396,   269,   398,   336,   270,   397,   400,   401,   402,
     403,   405,   406,   407,   408,  -319,   410,   354,   411,   415,
       2,   416,   418,   419,     3,   354,     4,     5,     6,   357,
       7,   420,   421,   358,   359,   360,   428,   357,   361,   362,
     433,   358,   359,   360,   434,   436,   361,   362,   482,     8,
       9,    10,   435,    11,    12,   477,   464,   465,   466,   478,
      13,    14,    15,    16,   483,    17,    18,    19,   484,   479,
     492,   485,   487,   488,   494,    20,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   489,
     493,   156,   490,   498,   430,   543,   546,   547,   539,   544,
     545,   552,   553,   586,   554,   560,   566,   589,   572,   561,
     574,   594,   607,   573,   590,   612,   577,   163,   593,   600,
     157,   601,   608,   609,   613,   610,   158,   223,    96,   141,
     184,   404,    26,    50,    51,    52,    21,    53,    54,    55,
      56,    57,    58,   193,    59,    60,   209,   323,   481,   205,
     206,   495,   220,   565,   394,   395,   582,   538,   507,   591,
     425,   504,   506,   227,     0,   576,     0,     0,     0,     0,
     508,     0,   509,     0,     0,     0,     0,     0,     0,     0,
       0,   241
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-537)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
     111,   345,    10,    10,   342,     3,     4,    38,    62,    15,
     469,    25,   142,    10,    31,    85,   354,   430,   554,   363,
     114,   105,    30,   107,    10,    86,    87,    10,    10,   350,
     351,   383,    30,    30,    90,    91,    10,   569,   154,    25,
      10,     3,     4,    25,   143,   161,   162,    10,   461,   148,
      10,   464,   465,   466,    94,    95,   469,   593,    21,    22,
      23,    24,   383,   144,    38,    39,   598,   148,    30,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    10,
     418,   425,   420,   421,   178,   155,   142,   198,    92,    93,
     146,   152,   153,   204,   432,   447,   448,   449,   144,   142,
      98,   560,   148,   146,    44,   159,   149,   138,   219,   165,
     431,   222,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   542,
       3,   215,   470,   147,   151,   142,    98,    10,   144,   250,
      10,   144,   148,   151,   149,   148,   559,   560,   561,    88,
      89,    48,    25,   151,    10,    28,   569,    30,    17,    18,
      19,    20,    15,    16,     3,    21,    22,    23,    24,    25,
      10,    10,    10,    11,    12,    13,    14,   142,    37,   144,
      97,   146,    10,   144,   597,   598,    25,   148,    10,    28,
     143,    30,    65,    66,    67,    68,    69,    70,    71,    72,
     144,   144,   144,   144,   148,   148,   148,   148,    10,   144,
     144,   555,   556,   148,   148,   553,   554,    90,    91,     8,
       9,    10,    11,    96,    10,    98,    65,    66,    67,    68,
      69,    70,    71,    72,   144,   142,   557,   145,   148,    25,
     148,    10,    28,   439,   440,   441,   442,   159,   160,    10,
     594,    90,    91,   145,   142,   593,   148,    96,   148,    98,
      25,   151,   147,   148,   608,   437,   438,     3,    10,   142,
     443,   444,   145,    10,    10,   445,   446,   150,   151,   150,
     142,   154,    45,    45,   149,   158,   159,   160,   142,    25,
     163,   164,    28,   404,    30,   142,   142,    10,    10,    10,
     152,     5,   150,   142,    90,    91,   145,   149,   144,    25,
      96,   150,   151,   148,    25,   154,   145,    28,    25,   158,
     159,   160,    10,   150,   163,   164,   150,   150,   145,    65,
      66,    67,    68,    69,    70,    71,    72,    25,    25,    25,
      28,    25,   142,    10,   145,    10,    10,   142,   146,    10,
      32,   148,    31,   145,    90,    91,   142,    25,    10,   143,
      96,   142,    98,   142,   150,   151,   148,   145,   154,   144,
     144,    10,   158,   159,   160,    10,   153,   163,   164,    90,
      91,   145,   142,    10,   142,    96,   145,   142,   142,   142,
      25,    15,    25,    28,   151,   143,    25,   149,    25,   148,
      45,    28,    90,    91,    10,   151,   142,    10,    96,   145,
     149,   146,    25,    10,   150,    25,    97,    10,   154,   145,
     147,    25,   158,   159,   160,   145,   144,   163,   164,    10,
      10,   142,    10,   144,    25,    10,    10,   154,   147,    10,
     150,    25,   148,   154,    45,   150,   144,   158,   159,   160,
     150,   148,   163,   164,   142,    90,    91,   148,   148,   144,
     143,    96,   150,    90,    91,    25,   154,   144,   148,    96,
     158,   159,   160,   148,   144,   163,   164,    10,    52,    53,
      54,    25,    56,    57,    58,    59,    60,    61,   142,    63,
      64,   147,    25,   143,   145,    28,   145,    10,    35,    25,
      10,   145,   145,    25,    10,    10,   144,   142,    25,   145,
       0,   145,   142,    10,     4,   142,     6,     7,     8,   154,
      10,   142,   142,   158,   159,   160,   145,   154,   163,   164,
      84,   158,   159,   160,   156,   158,   163,   164,   145,    29,
      30,    31,   157,    33,    34,   144,   142,   142,   142,   151,
      40,    41,    42,    43,   142,    45,    46,    47,   144,   151,
      25,   144,   144,   142,    72,    55,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   148,
     145,    10,   143,   145,   148,   144,    28,    25,   148,   147,
     145,    45,   142,   147,   145,   148,   144,    28,   144,   148,
     144,    73,    28,   148,   145,    28,   148,   105,   145,   148,
     102,   145,   144,   144,   144,   148,   102,   174,    64,    96,
     120,   329,     1,    52,    53,    54,   146,    56,    57,    58,
      59,    60,    61,   135,    63,    64,   153,   230,   398,   148,
     148,   418,   171,   539,   263,   265,   557,   473,   434,   577,
     356,   431,   433,   178,    -1,   549,    -1,    -1,    -1,    -1,
     435,    -1,   436,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   199
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   167,     0,     4,     6,     7,     8,    10,    29,    30,
      31,    33,    34,    40,    41,    42,    43,    45,    46,    47,
      55,   146,   168,   169,   171,   179,   197,   188,    10,   187,
      10,   142,   180,   181,   187,   187,   187,   187,    10,    10,
      48,    44,    10,    10,    97,    10,    94,    95,    10,    10,
      52,    53,    54,    56,    57,    58,    59,    60,    61,    63,
      64,   172,   173,   143,   260,    10,   142,    10,   182,   183,
     184,   185,    10,   142,   175,    25,    10,    10,   260,   150,
     193,   142,    45,    45,   149,   142,   145,   148,   142,   142,
      10,    10,   152,   174,   174,     5,   188,   150,    15,   198,
     199,   202,   204,   149,   144,   148,   144,   148,   145,    25,
     190,    25,   150,   150,   150,   262,     3,    30,    98,   170,
     194,   195,   196,   197,   145,    10,    25,   176,    10,    38,
      39,   215,    25,    25,    25,   142,   145,    10,    10,    10,
     142,   193,    17,    18,    19,    20,    37,   210,   211,   213,
     214,   216,   144,   148,   146,   200,    10,   172,   173,   203,
      10,    25,   186,   183,   185,    10,   185,   148,   190,    32,
     207,   208,   209,    31,   177,   178,    10,   170,   263,   264,
     265,   145,    25,   151,   195,    10,   143,   142,   142,   145,
     144,   144,   148,   198,   153,   260,   145,    10,   145,   142,
     142,   142,   142,   142,   151,   213,   214,   149,   189,   199,
      25,   201,   143,   206,   174,   148,    25,    45,    10,   151,
     209,    10,   151,   178,   146,   266,   151,   264,   149,    25,
      10,    25,    97,    10,   145,   147,    25,   144,   145,   144,
     190,   215,    10,    10,    10,    25,   190,    10,   217,   150,
     191,   154,   147,    10,    21,    22,    23,    24,    25,   205,
      10,   185,   148,   150,   190,   150,   190,    25,    10,    25,
      28,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   267,   268,   269,   270,   271,   272,
     273,   274,    45,   200,   144,   148,   148,   148,   143,   192,
     144,   144,   148,   148,   144,   142,   145,     3,    10,    25,
      28,    30,    65,    66,    67,    68,    69,    70,    71,    72,
      90,    91,    96,    98,   142,   145,   150,   154,   158,   159,
     160,   163,   164,   220,   221,   222,   226,   229,   230,   231,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   249,   250,   251,   252,   256,   257,
     275,   190,    25,    25,   210,   207,   147,   145,   143,   261,
      10,    35,    25,    10,   191,   145,   145,    25,    10,   212,
     144,    25,    10,   228,   230,   145,   145,   226,   142,    10,
     142,   142,   245,   245,   230,   220,   151,   226,   145,   145,
     148,    85,   155,    84,   156,   157,   158,    92,    93,    86,
      87,   152,   153,    88,    89,   159,   160,   154,   161,   162,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
     149,   232,   244,   245,   142,   142,   142,    90,    91,   142,
     146,   165,    10,   142,   223,   224,   225,   144,   151,   151,
      10,   205,   145,   142,   144,   144,   190,   144,   142,   148,
     143,   218,    25,   145,    72,   222,   227,   230,   145,   230,
     230,   144,   151,   231,   235,   230,   236,   237,   238,   239,
     240,   240,   241,   241,   241,   241,   242,   242,   243,   243,
     244,   244,   244,   231,   231,   253,   231,   231,   144,   253,
     230,    10,    11,    12,    13,    14,   247,   248,   225,   148,
     142,   146,   149,   144,   147,   145,    28,    25,    15,    16,
     219,   258,    45,   142,   145,   144,   144,   143,   144,   148,
     148,   148,   144,   147,   144,   224,   144,    25,   147,   150,
     231,   254,   144,   148,   144,   259,   259,   148,   230,   228,
     226,   226,   233,   231,   253,   231,   147,   254,   255,    28,
     145,   258,   144,   145,    73,   144,   144,   148,   148,   151,
     148,   145,   228,   226,   231,   151,   254,    28,   144,   144,
     148,   226,    28,   144
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
#line 322 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    { // Initialization code
	    sharedCode.str("");    // clear the stream
		sharedCode<<"#define GLSL\n";
// Some GLSL compilers can't abide seeing a "discard" in the source of a shader that isn't a fragment shader, even if it's in unused, shared code.
	sharedCode<<"#if !defined(GLFX_FRAGMENT_SHADER)\n"
							"#define discard\n"
						"#endif\n";
		sharedCode<<"float saturate(float x)"
						"{"
						"	return clamp(x,0.0,1.0);"
						"}"
						"\n"
						"vec2 saturate(vec2 x)"
						"{"
						"	return clamp(x,vec2(0.0,0.0),vec2(1.0,1.0));"
						"}"
						"\n"
						"vec3 saturate(vec3 x)"
						"{"
						"	return clamp(x,vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0));"
						"}"
						"\n"
						"vec4 saturate(vec4 x)"
						"{"
						"	return clamp(x,vec4(0.0,0.0,0.0,0.0),vec4(1.0,1.0,1.0,1.0));"
						"}\n";
		

	sharedCode<<"vec4 mul(mat4 m,vec4 v)"
							"{"
							"	return m*v;"
							"}\n"
							"vec3 mul(mat3 m,vec3 v)"
							"{"
							"	return m*v;"
							"}\n"
							"vec2 mul(mat2 m,vec2 v)"
							"{"
							"	return m*v;"
							"}\n"
							"vec4 mul(vec4 v,mat4 m)"
							"{"
							"	return v*m;"
							"}\n"
							"vec3 mul(vec3 v,mat3 m)"
							"{"
							"	return v*m;"
							"}\n"
							"vec2 mul(vec2 v,mat2 m)"
							"{"
							"	return v*m;"
							"}\n";
	    gLexPassthrough=true;
	}
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 377 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 380 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	}
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 383 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 386 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string texture_type=(yyvsp[(2) - (6)]).strs[0];
		GLTextureType type_enum = ToGLTextureType((yyvsp[(2) - (6)]).token);
		bool compute_only		=(texture_type==string("image3D"))||(texture_type.substr(0,2)==string("RW"));
		
		stringReplaceAll(texture_type,"RWTexture","image");
		string template_type	=(yyvsp[(3) - (6)]).strs[0];
		if(template_type.length()==0)
			template_type="vec4";
		string texture_name		=(yyvsp[(4) - (6)]).strs[0];
		string layout;
		if(compute_only)
			layout=GetLayoutForTemplateType(template_type,texture_name)+" ";
		string type=GetEquivalentGLTextureType(texture_type,template_type);

		DeclaredTexture dec;
		dec.variant			=compute_only&&IsRWTexture(type_enum);//(template_type==string("vec4"));
		dec.file_number		=current_filenumber;
		dec.line_number		=(yyvsp[(1) - (6)]).lineno+last_linenumber-global_linenumber;
		dec.type			=type;
		dec.texel_format	=template_type;
		dec.type_enum		=type_enum;
		dec.layout			=layout;
		gEffect->DeclareTexture(texture_name,dec);

		ostringstream decl;

	}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 416 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		//string buffer_type=$2.strs[0];
		bool compute_only=((yyvsp[(2) - (5)]).token==RW_STRUCTUREDBUFFER);
		ostringstream decl;
		if(compute_only)
			sharedCode<<"#ifdef IN_COMPUTE_SHADER\n";
		string template_type=(yyvsp[(3) - (5)]).strs[0];
		string buffer_name=(yyvsp[(4) - (5)]).strs[0];
		decl<<"layout(std430) buffer "<<buffer_name<<"_\n{\n\t";
		decl<<template_type<<" "<<buffer_name<<"[];\n";
		decl<<"};\n";
		sharedCode<<decl.str();
		if(compute_only)
			sharedCode<<"#endif\n";
	}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 432 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		ostringstream decl;
		decl<<(yyvsp[(1) - (4)]).strs[0]<<" "<<(yyvsp[(2) - (4)]).strs[0]<<" "<<(yyvsp[(3) - (4)]).strs[0]<<";";
		sharedCode<<decl.str();
	}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 438 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		maxVertexCount=(yyvsp[(4) - (6)]).num;
	}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 442 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		currentCsLayout.x=(yyvsp[(4) - (10)]).num;
		currentCsLayout.y=(yyvsp[(6) - (10)]).num;
		currentCsLayout.z=(yyvsp[(8) - (10)]).num;
	}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 448 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 451 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string str=(yyvsp[(1) - (2)]).strs[0]+" "+(yyvsp[(2) - (2)]).strs[0];
		glfxWrite(str.c_str());
		glfxPopState();
	}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 457 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string str="layout(rgba32f) uniform image3D ";
		str+=(yyvsp[(2) - (3)]).strs[0]+";";
		glfxWrite(str.c_str());
	}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 463 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string str="layout(rgba32f) image3D ";
		str+=(yyvsp[(2) - (3)]).strs[0]+",";
		glfxWrite(str.c_str());
	}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 469 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string name					=(yyvsp[(2) - (3)]).strs[0];
	}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 473 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string name		=(yyvsp[(2) - (3)]).strs[0];
		string str		=string("#undef ")+name+"\n";
		glfxWrite(str.c_str()); 
	}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 479 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string name					=(yyvsp[(2) - (4)]).strs[0];
		gEffect->DeclareStruct(name,buildStruct);
		sharedCode<<"struct "<<(yyvsp[(2) - (4)]).strs[0]<<"\n{\n";
		for(int i=0;i<(int)buildStruct.m_structMembers.size();i++)
		{
			const StructMember &m=buildStruct.m_structMembers[i];
			sharedCode<<m.type<<" "<<m.name<<";\n";
		}
		WriteLineNumber(sharedCode,current_filenumber,(yyvsp[(4) - (4)]).lineno+last_linenumber-global_linenumber);
		sharedCode<<"};";
		// Set to default:
		buildStruct					=Struct();
	}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 494 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).lineno		=(yyvsp[(4) - (5)]).lineno;
		RenderState t=renderState;
		string name=(yyvsp[(2) - (5)]).strs[0];
		if(t==RASTERIZER_STATE)
		{
			gEffect->DeclareRasterizerState(name,buildRasterizerState);
			// Set to default:
			buildRasterizerState=RasterizerState();
		}
		if(t==BLEND_STATE)
		{
			gEffect->DeclareBlendState(name,buildBlendState);
			// Set to default:
			buildBlendState=BlendState();
		}
		if(t==DEPTHSTENCIL_STATE)
		{
			gEffect->DeclareDepthStencilState(name,buildDepthStencilState);
			// Set to default:
			buildDepthStencilState=DepthStencilState();
		}
		if(t==SAMPLER_STATE)
		{
			gEffect->DeclareSamplerState(name,buildSamplerState);
			// Set to default:
			buildSamplerState=SamplerState();
		}
		int true_linenumber=(yyvsp[(4) - (5)]).lineno+(last_linenumber-global_linenumber);
	
	}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 526 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string profileName		=(yyvsp[(2) - (6)]).strs[0];
		int profileNum			=(yyvsp[(4) - (6)]).num;
		if(!gEffect->SetVersionForProfile(profileNum,profileName))
			glfxWarning("redefining profile ");
	}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 533 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		// We here declare that a named shader should be compiled with the specified profile (e.g. GLSL 4.30)
		// and assign a variable name to the shader/profile combination. Later on, in pass definitions,
		// we can assign the shader to one or more passes.
		string lvalCompiledShaderName		=(yyvsp[(2) - (5)]).strs[0];
		string rvalCompiledShaderName		=(yyvsp[(4) - (5)]).strs[0];
		gEffect->AddCompiledShader((yyvsp[(2) - (5)]).sType,lvalCompiledShaderName,rvalCompiledShaderName);
	}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 542 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 545 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string bufferName	=(yyvsp[(2) - (7)]).strs[0];
		string def			=(yyvsp[(5) - (7)]).strs[0];
		string semantic		=(yyvsp[(3) - (7)]).strs[0];
		int buffer_number	=(yyvsp[(3) - (7)]).num;

		ostringstream str;
		WriteLineNumber(str,current_filenumber,(yyvsp[(1) - (7)]).lineno+(last_linenumber-global_linenumber));
		str<< "layout(std140) uniform "<<(yyvsp[(2) - (7)]).strs[0]<<(yyvsp[(5) - (7)]).strs[0]<<";\n";
		WriteLineNumber(str,current_filenumber,glfxget_lineno()+(last_linenumber-global_linenumber));

		glfxWrite(str.str().c_str());
		// Set to default:
		buildStruct					=Struct();
	}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 562 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string functionName					=(yyvsp[(2) - (8)]).strs[0];
		buildFunction.returnType			=(yyvsp[(1) - (8)]).strs[0];
		buildFunction.content				=(yyvsp[(7) - (8)]).strs[0];
	
		buildFunction.parameters			=*(yyvsp[(4) - (8)]).vars;
		delete (yyvsp[(4) - (8)]).vars;
		
			//WriteLineNumber(sharedCode,current_filenumber,glfxget_lineno()+(last_linenumber-global_linenumber));
		buildFunction.current_filenumber	=current_filenumber;
		buildFunction.main_linenumber		=(yyvsp[(1) - (8)]).lineno;
		buildFunction.content_linenumber	=(yyvsp[(1) - (8)]).lineno+(last_linenumber-global_linenumber);
	buildFunction.name=functionName;
//		buildFunction.source				=str.str();
		Function *f							=gEffect->DeclareFunction(functionName,buildFunction);
		buildFunction.clear();
	}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 581 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		CompilableShader &sh=*(compilableShaders[(yyvsp[(3) - (9)]).strs[0]]=new CompilableShader);
		int line_number			=last_linenumber-global_linenumber;
		sh.shaderName			=(yyvsp[(3) - (9)]).strs[0];
		sh.maxGSVertexCount		=maxVertexCount;

		buildFunction.returnType			=(yyvsp[(2) - (9)]).strs[0];
		buildFunction.content				=(yyvsp[(8) - (9)]).strs[0];
		buildFunction.parameters			=*(yyvsp[(5) - (9)]).vars;
		buildFunction.current_filenumber	=current_filenumber;
		buildFunction.main_linenumber		=(yyvsp[(1) - (9)]).lineno;
		buildFunction.content_linenumber	=(yyvsp[(1) - (9)]).lineno+line_number;

		sh.returnable			=(yyvsp[(8) - (9)]).strs[1];
		sh.main_linenumber		=buildFunction.content_linenumber;
		sh.content_linenumber	=(yyvsp[(8) - (9)]).lineno+line_number;
		sh.current_filenumber	=current_filenumber;
		sh.csLayout				=currentCsLayout;
		
		buildFunction.name=sh.shaderName;

		Function *f				=gEffect->DeclareFunction(sh.shaderName,buildFunction);
		sh.function=f;
		buildFunction.textureSamplers.clear();	// so the pointers won't be freed later.

		for(std::map<std::string,TextureSampler*>::const_iterator i=sh.function->textureSamplers.begin();i!=sh.function->textureSamplers.end();i++)
		{
			if(!gEffect->DeclareTextureSampler(i->second))
				errSem(string("Can't find texture declaration for ")+i->first);
		}

		buildFunction.clear();
		delete (yyvsp[(5) - (9)]).vars;
	//	sharedCode<<"#line "<<(glfxget_lineno()+(last_linenumber-global_linenumber))<<" "<<current_filenumber<<endl;
	}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 617 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyvsp[(5) - (7)]).prog->m_separable=(yyvsp[(1) - (7)]).boolean;
	}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 621 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 624 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string groupName=(yyvsp[(3) - (7)]).strs[0];
		gEffect->AddTechniqueGroup(groupName,buildTechniqueGroup);
		buildTechniqueGroup.m_techniques.clear();
	}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 630 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		last_linenumber		=(yyvsp[(3) - (6)]).num;
		current_filenumber	=(yyvsp[(4) - (6)]).num;
		global_linenumber	=(yyvsp[(1) - (6)]).lineno+1;	// the line AFTER the #line directve.
		WriteLineNumber(sharedCode,current_filenumber,last_linenumber);
	}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 637 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		last_linenumber		=(yyvsp[(2) - (4)]).num;
		current_filenumber	=(yyvsp[(3) - (4)]).num;
		global_linenumber	=(yyvsp[(1) - (4)]).lineno;
	  (yyval).strs[0]	="";
	}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 644 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]	=(yyvsp[(1) - (2)]).strs[0];
		(yyval).lineno	=(yyvsp[(1) - (2)]).lineno;
	}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 649 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]	=(yyvsp[(1) - (1)]).strs[0];
		(yyval).num		=(yyvsp[(1) - (1)]).num;
		(yyval).token	=(yyvsp[(1) - (1)]).token;
	}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 655 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]	=(yyvsp[(1) - (1)]).strs[0];
		(yyval).num		=(yyvsp[(1) - (1)]).num;
		(yyval).token	=(yyvsp[(1) - (1)]).token;
	}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 661 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]	=(yyvsp[(2) - (3)]).strs[0];
		(yyval).num		=1;
	}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 666 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]	="";
		(yyval).num		=0;
	}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 671 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	std::string ret="(";
	ret+=(yyvsp[(2) - (7)]).strs[0];
	ret+=",";
	ret+=(yyvsp[(4) - (7)]).strs[0];
	ret+=",";
	ret+=(yyvsp[(6) - (7)]).strs[0];
	ret+=")";
	(yyval).strs[0]=ret;
	currentCsLayout.x=(yyvsp[(2) - (7)]).num;
	currentCsLayout.y=(yyvsp[(4) - (7)]).num;
	currentCsLayout.z=(yyvsp[(6) - (7)]).num;
}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 685 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			  }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 688 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			  }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 691 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			  }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 695 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			  }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 698 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		buildTechniqueGroup.m_techniques[(yyvsp[(2) - (5)]).strs[0]] = (yyvsp[(4) - (5)]).tech;
	}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 702 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string techname=(yyvsp[(3) - (7)]).strs[0];
		gEffect->AddTechnique(techname,"",(yyvsp[(5) - (7)]).tech);
	}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 707 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
		  }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 711 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).layoutType=NORMAL_LAYOUT_TYPE;
				(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
		 }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 716 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 720 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 724 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
			}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 728 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
			}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 732 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				currentCsLayout.x=(yyvsp[(1) - (5)]).num;
				currentCsLayout.y=(yyvsp[(3) - (5)]).num;
				currentCsLayout.z=(yyvsp[(5) - (5)]).num;
				(yyval).layoutType=CS_LAYOUT_TYPE;
				(yyval).strs[0]=(yyvsp[(1) - (5)]).strs[0]+(yyvsp[(2) - (5)]).strs[0]+(yyvsp[(3) - (5)]).strs[0]+(yyvsp[(4) - (5)]).strs[0]+(yyvsp[(5) - (5)]).strs[0];
		  }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 740 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				(yyval).num=(yyvsp[(3) - (3)]).num;
		   }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 746 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		   }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 750 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		   }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 754 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    { /* Switch lex to fx scanning */
	read_shader=false;
	read_function=false;
    gLexPassthrough=false;
}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 761 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	read_shader=true;
	read_function=false;
    gLexPassthrough=false;
}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 768 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	read_function=true;
	read_shader=false;
    gLexPassthrough=false;
}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 775 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    { /* Switch lex to passthrough mode */
    gLexPassthrough=true;
}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 779 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    { /* Read next block */
    (yyval).strs[0]=(yyvsp[(2) - (3)]).strs[0];
	// string 1 is the return value, if present.
	(yyval).strs[1]= (yyvsp[(2) - (3)]).strs[1];
    (yyval).lineno= (yyvsp[(2) - (3)]).lineno;
}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 787 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
    (yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
 }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 791 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
    (yyval).strs[0]="";
 }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 795 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
    (yyval).strs[0]="{\n";
	(yyval).strs[0]+=((yyvsp[(2) - (3)]).strs[0]+"\n}\n");
}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 800 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	string list			=(yyvsp[(1) - (2)]).strs[0];
	string new_member	=(yyvsp[(2) - (2)]).strs[0];
    (yyval).strs[0]=(list+"\n")+new_member;
}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 806 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	string member	=(yyvsp[(1) - (1)]).strs[0];
    (yyval).strs[0]=member;
}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 811 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	string str=(yyvsp[(1) - (1)]).strs[0];
	//if(write_line_number_next)
//		str=
    (yyval).strs[0]	=str;
}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 819 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
	(yyval).num = -1;
    (yyval).strs[0]	="";
}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 825 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	gSkip();
}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 829 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	gSkip();
}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 833 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).lineno		=(yyvsp[(4) - (6)]).lineno;
	string unif		=(yyvsp[(1) - (6)]).strs[0];
	string type		=(yyvsp[(2) - (6)]).strs[0];
	string name		=(yyvsp[(3) - (6)]).strs[0];
	string sq		=(yyvsp[(4) - (6)]).strs[0];
	string sem		=(yyvsp[(5) - (6)]).strs[0];
	StructMember s;
	s.type		=type;
	s.semantic	=sem;
	s.name		=name+sq;
	buildStruct.m_structMembers.push_back(s);
	(yyval).strs[0]=(unif.length()?(unif+" "):"");
    (yyval).strs[0]+=(type+" ")+name;
	(yyval).strs[0]+=sq;
	(yyval).strs[0]+=sem.length()?(string(": ")+sem):"";
	(yyval).strs[0]+=";";
}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 852 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
    (yyval).strs[0]="uniform";
}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 856 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
    (yyval).strs[0]="";
}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 860 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).vars		=(yyvsp[(1) - (3)]).vars;
		(yyval).strs[0]	=((yyvsp[(1) - (3)]).strs[0]+",")+(yyvsp[(3) - (3)]).strs[0];

		YYSTYPE::variable var;
		var.storage		=(yyvsp[(3) - (3)]).strs[1];
		var.type		=(yyvsp[(3) - (3)]).strs[2];
		var.identifier	=(yyvsp[(3) - (3)]).strs[3];
		var.template_	=(yyvsp[(3) - (3)]).strs[4];
		var.type_enum = ToGLTextureType((yyvsp[(3) - (3)]).token);
		(yyval).vars->push_back(var);
		buildFunction.parameters.push_back(var);
	}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 874 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		YYSTYPE::variable var;
		string s1	=(yyvsp[(1) - (1)]).strs[0];
		(yyval).strs[0]	=s1;
		var.storage		=(yyvsp[(1) - (1)]).strs[1];
		var.type		=(yyvsp[(1) - (1)]).strs[2];
		var.identifier	=(yyvsp[(1) - (1)]).strs[3];
		var.template_	=(yyvsp[(1) - (1)]).strs[4];
		var.type_enum = ToGLTextureType((yyvsp[(1) - (1)]).token);
    
		(yyval).vars = new vector<YYSTYPE::variable>;
		(yyval).vars->push_back(var);
		buildFunction.parameters.push_back(var);
	}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 889 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]	="";
		(yyval).vars = new vector<YYSTYPE::variable>;
	}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 895 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
    ostringstream prepend;
	string template_		=(yyvsp[(1) - (3)]).strs[0];
	string location			=(yyvsp[(3) - (3)]).strs[1];
    switch((yyvsp[(3) - (3)]).rType)
	{
    case REGISTER_INT:
        prepend<<"layout(location="<<(yyvsp[(3) - (3)]).num<<") ";
        break;
    case REGISTER_NAME:
	// GL Doesn't recognize arbitrary text for this.
        //prepend<<"layout(location="<<$3.strs[0]<<") ";
        break;
    case SEMANTIC:
		template_=getGLInputSemantic((yyvsp[(3) - (3)]).num);
        break;
    }
    prepend<<(yyvsp[(1) - (3)]).strs[1];

	string storage	=prepend.str();
	string type		=(yyvsp[(1) - (3)]).strs[2];
	string id		=(yyvsp[(1) - (3)]).strs[3]+(yyvsp[(2) - (3)]).strs[0];
	string all		=(((yyvsp[(1) - (3)]).strs[1]+" ")+(yyvsp[(1) - (3)]).strs[2]+" ")+id+(yyvsp[(3) - (3)]).strs[0];
    (yyval).strs[0]	=all;
    (yyval).strs[1]	=storage;
    (yyval).strs[2]	=type;
    (yyval).strs[3]	=id;
	(yyval).token	=(yyvsp[(1) - (3)]).token;
    (yyval).strs[4]	=template_;
}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 927 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(string("[")+(yyvsp[(2) - (3)]).strs[0])+"]";
					(yyval).num=(yyvsp[(2) - (3)]).num;
				}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 932 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]="";
					(yyval).num=0;
				}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 937 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
					(yyval).num=(yyvsp[(1) - (1)]).num;
				}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 942 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]="";
					(yyval).num=0;
				}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 948 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).num=(yyvsp[(1) - (3)]).num*(yyvsp[(3) - (3)]).num;
					(yyval).strs[0]=stringFormat("%d",(yyval).num);
				}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 953 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		string storage	=(yyvsp[(1) - (4)]).strs[0];
		string type		=(yyvsp[(2) - (4)]).strs[0];
		string template_=(yyvsp[(3) - (4)]).strs[0];
		// TODO: We discard template information in parameters. Should we insert a layout spec?
	
		
		string id		=(yyvsp[(4) - (4)]).strs[0];
		if(template_.length()>=4&&(template_.substr(0,4)==string("uint")||template_.substr(0,4)==string("uvec")))
			type=string("u")+type;
		else if(template_.length()>=4&&template_.substr(0,4)==string("ivec"))
			type=string("i")+type;
		else if(template_.length()>=3&&template_.substr(0,3)==string("int"))
			type=string("i")+type;
		else if(IsBufferType((yyvsp[(2) - (4)]).token))
		{
			if(template_.length()>0)
				type=template_+"[]";
		}
		(yyval).strs[0]		=template_;
		(yyval).strs[1]		=storage;
		(yyval).strs[2]		=type;
		(yyval).strs[3]		=id;
		(yyval).token		=(yyvsp[(2) - (4)]).token;
	}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 979 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]		=(yyvsp[(1) - (1)]).strs[0];
				(yyval).token		=(yyvsp[(1) - (1)]).token;
			}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 984 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]		=(yyvsp[(1) - (1)]).strs[0];
				(yyval).token		=(yyvsp[(1) - (1)]).token;
			}
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 989 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]		=(yyvsp[(1) - (1)]).strs[0];
				(yyval).token		=(yyvsp[(1) - (1)]).token;
			}
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 994 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
	}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 998 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]="";
	}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 1002 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		(yyval).num=SV_DISPATCHTHREADID;
	}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 1007 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		(yyval).num=SV_GROUPID;
	}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 1012 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		(yyval).num=SV_GROUPTHREADID;
	}
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 1017 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		(yyval).num=SV_VERTEXID;
	}
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 1022 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).num=(yyvsp[(2) - (2)]).num;
		(yyval).rType=REGISTER_INT;
	}
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 1027 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(2) - (2)]).strs[0];
		(yyval).rType=SEMANTIC;
		(yyval).num=(yyvsp[(2) - (2)]).num;
	}
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 1033 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(2) - (2)]).strs[0];
		(yyval).rType=REGISTER_NAME;
	}
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 1038 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).rType=REGISTER_NONE;
	}
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 1044 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	Technique *t = new Technique(*((yyvsp[(1) - (1)]).passes));
	(yyval).tech = t;
	delete (yyvsp[(1) - (1)]).passes;
}
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 1051 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).passes				=((yyvsp[(1) - (2)]).passes);
	map<string,Program> &passes=*(yyval).passes;
	string name			=(yyvsp[(2) - (2)]).strs[0];
	Program *p			=(yyvsp[(2) - (2)]).prog;
	if(p)
	{
		passes[name]	=*(p);
	}
	delete (yyvsp[(2) - (2)]).prog;
}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 1063 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	map<string,Program> *m=new map<string,Program>;
	(yyval).passes = m;
	string name = (yyvsp[(1) - (1)]).strs[0];
	Program *p = (yyvsp[(1) - (1)]).prog;
	if (p)
	{
		Program &newp = (*m)[name];
		newp= *p;
	}
	delete p;
}
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 1077 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyvsp[(4) - (5)]).prog->m_separable	= (yyvsp[(1) - (5)]).boolean;
	(yyval).prog					= (yyvsp[(4) - (5)]).prog;
	(yyval).strs[0]				= (yyvsp[(2) - (5)]).strs[0];
}
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 1084 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).prog = new Program(*((yyvsp[(1) - (1)]).shaders), buildPassState, gEffect->GetTextureSamplersByShader(), (yyvsp[(1) - (1)]).strs[2]);
    delete (yyvsp[(1) - (1)]).shaders;
	buildPassState			=PassState();
}
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 1091 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		map<ShaderType, Program::Shader>* map1=(yyvsp[(1) - (2)]).shaders;
		ShaderType sType				=(yyvsp[(2) - (2)]).sType;
		string preamble					=(yyvsp[(2) - (2)]).strs[0];
		string name						=(yyvsp[(2) - (2)]).strs[1];
		string compiledShaderName		=(yyvsp[(2) - (2)]).strs[3];
		(yyval).strs[2]						=(yyvsp[(1) - (2)]).strs[2]+(yyvsp[(2) - (2)]).strs[2];// compute layout
		CompiledShaderMap::const_iterator i	=gEffect->GetCompiledShaders().find(compiledShaderName);
		if(i==gEffect->GetCompiledShaders().end())
			(*map1)[sType].compiledShader	=NULL;
		else
			(*map1)[sType].compiledShader	=i->second;
		(*map1)[sType].preamble			=preamble;
		(*map1)[sType].name				=name;
		(yyval).shaders						=map1;
	}
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 1108 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		map<ShaderType, Program::Shader>* map1=new map<ShaderType, Program::Shader>();
		string preamble				=(yyvsp[(1) - (1)]).strs[0];
		string name					=(yyvsp[(1) - (1)]).strs[1];
		string compiledShaderName	=(yyvsp[(1) - (1)]).strs[3];
		ShaderType sType			=(yyvsp[(1) - (1)]).sType;
		(yyval).strs[0]					=(yyvsp[(1) - (1)]).strs[0];
		(yyval).strs[1]					=(yyvsp[(1) - (1)]).strs[1];
		(yyval).strs[2]					=(yyvsp[(1) - (1)]).strs[2];
		(yyval).strs[3]					=compiledShaderName;
		CompiledShaderMap::const_iterator i=gEffect->GetCompiledShaders().find(compiledShaderName);
		if(i==gEffect->GetCompiledShaders().end())
			(*map1)[sType].compiledShader	=NULL;
		else
			(*map1)[sType].compiledShader	=i->second;
		(yyval).sType							=sType;
		(*map1)[sType].name					=name;
		(*map1)[sType].preamble				=preamble;
		(yyval).shaders=map1;
	}
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 1129 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		//.map<ShaderType, Program::Shader>* map1=$1.shaders;
		(yyval).shaders=(yyvsp[(1) - (2)]).shaders;
		(yyval).strs[0]	="";
		(yyval).strs[1]	="";
		(yyval).strs[2]	=(yyvsp[(1) - (2)]).strs[2];
		(yyval).sType	=(ShaderType)((int)NUM_OF_SHADER_TYPES+1);
	}
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 1138 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		map<ShaderType, Program::Shader>* map1=new map<ShaderType, Program::Shader>();
		(yyval).shaders	=map1;
		(yyval).strs[0]	="";
		(yyval).strs[1]	="";
		(yyval).strs[2]	="";
		(yyval).sType	=(ShaderType)((int)NUM_OF_SHADER_TYPES+1);
	}
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 1147 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		  (yyval).strs[0]=(yyvsp[(1) - (10)]).strs[0]+"(";
		  (yyval).strs[0]+=(yyvsp[(3) - (10)]).strs[0]+",";
		  (yyval).strs[0]+=(yyvsp[(5) - (10)]).strs[0]+",";
		  (yyval).strs[0]+=(yyvsp[(7) - (10)]).strs[0]+",";
		  (yyval).strs[0]+=(yyvsp[(9) - (10)]).strs[0]+")";
	  }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 1156 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				 string rast_name=(yyvsp[(3) - (5)]).strs[0];
			 }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 1160 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				 string depth_name=(yyvsp[(3) - (7)]).strs[0];
				 int index=(yyvsp[(5) - (7)]).num;
				 buildPassState.depthStencilState=depth_name;
			 }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 1166 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				 string blend_name=(yyvsp[(3) - (9)]).strs[0];
				 string vec4text=(yyvsp[(5) - (9)]).strs[0];
				 buildPassState.blendState=blend_name;
				 unsigned mask=(yyvsp[(7) - (9)]).unum;
			 }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 1174 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0] = "";
				(yyval).strs[1] = "";
				(yyval).strs[2] = "";
				errSem("Pass definitions like vs(430)=vs_main are no longer supported. Please use SetVertexShader() etc.");
			}
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 1182 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]	="";
				(yyval).strs[1]	="";
				(yyval).strs[2]	="";
				(yyval).sType	=(yyvsp[(1) - (5)]).sType;//(ShaderType)((int)NUM_OF_SHADER_TYPES+1);
				// the shader_variable is something like vs_main, for which we have a compile command like:
				//												VertexShader vs_layers=CompileShader(430,VS_Layers());
				// i.e. it's a combination of shadertype, profile, and function name.
				// so these are stored in gEffect->GetCompiledShaders().
				string compiledShaderName=(yyvsp[(3) - (5)]).strs[0];
				CompiledShaderMap::const_iterator i=gEffect->GetCompiledShaders().find(compiledShaderName);
				if(i==gEffect->GetCompiledShaders().end())
				{
					if(_stricmp(compiledShaderName.c_str(),"NULL")==0)
					{
					}
					else
					{
						ostringstream errMsg;
						errMsg<<"Unable to find compiled shader \""<<compiledShaderName<<'\"';
						errSem(errMsg.str(), (yyvsp[(3) - (5)]).lineno);
					}
				}
				else
				{
					CompiledShader *compiledShader	=i->second;
					(yyval).num							=compiledShader->version;
					(yyval).sCommand						=(yyvsp[(1) - (5)]).sCommand;
					(yyval).sType						=(yyvsp[(1) - (5)]).sType;
					if((yyval).sType!=(yyvsp[(3) - (5)]).sType)
					{
						if((yyval).sType==GEOMETRY_SHADER&&(yyvsp[(3) - (5)]).sType==VERTEX_SHADER)
						{
							(yyval).strs[0]	="";
							(yyval).strs[1]	="";
							(yyval).strs[3]	="NULL";
						}
						else
						{
							errSem((((string("Shader type mismatch for ")+compiledShaderName+" - can't call ")+(yyvsp[(1) - (5)]).strs[0]+" with ")+ShaderTypeToString((yyvsp[(3) - (5)]).sType)+" shader as input.").c_str());
						}
					}
					else
					{
						ostringstream preamble;
						preamble << "#version " << compiledShader->version << endl;
						glfxParser::ShaderCommand shaderCommand=(yyvsp[(1) - (5)]).sCommand;
						if(shaderCommand<NumShaderCommands)
						{
							string shaderName=compiledShader->m_functionName;
							if(_stricmp(shaderName.c_str(),"NULL")!=0)
							{
								if (shaderCommand == COMPUTE_SHADER)
								{
									preamble << gEffect->GetComputeLayouts().find(shaderName)->second.text() << "\n";
									preamble << "#define IN_COMPUTE_SHADER 1" << endl;
								//	preamble << computeLayout << "\n"; 
								}
								if (shaderCommand ==FRAGMENT_SHADER)
								{
									preamble << "#define GLFX_FRAGMENT_SHADER 1" << endl;
								}
							}
							(yyval).strs[0]	=preamble.str();
							(yyval).strs[1]	=shaderName;
							(yyval).strs[3]	=compiledShaderName;
							//if (shaderCommand == COMPUTE_SHADER)
							//	$$.strs[2] = gEffect->m_shaderLayouts[shaderName].text();
						}
					}
				}
			}
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 1255 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			string compiledShaderName	=(yyvsp[(1) - (1)]).strs[0];
			CompiledShaderMap::const_iterator i=gEffect->GetCompiledShaders().find(compiledShaderName);
			if(i!=gEffect->GetCompiledShaders().end())
				(yyval).sType				=i->second->shaderType;
			else
				(yyval).sType				=NUM_OF_SHADER_TYPES;
			(yyval).strs[0]					=compiledShaderName;
			(yyval).num						=0;
			(yyval).lineno					=(yyvsp[(1) - (1)]).lineno;
		}
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 1267 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).lineno								=(yyvsp[(5) - (6)]).lineno;
			(yyval).sType								=GEOMETRY_SHADER;
			string gsName							=(yyvsp[(3) - (6)]).strs[0];
			string definition						=(yyvsp[(5) - (6)]).strs[0];
			CompiledShaderMap::const_iterator i		=gEffect->GetCompiledShaders().find(gsName);
			string tempName							=gsName+"_GS_with_SO";
			(yyval).strs[0]								=tempName;
		
			if(i==gEffect->GetCompiledShaders().end())
			{
				ostringstream msg;
				msg<<"Compiled shader "<<gsName<<" not found for streamout"<<endl;
				errSem(msg.str().c_str());
			}
			else
			{
				CompiledShader *compiledShader	=i->second;
				const CompiledShader *new_cs	=gEffect->AddCompiledShader(tempName,gsName,GEOMETRY_SHADER,0,compiledShader->outputStruct
																			,compiledShader->outputStructName
																			,compiledShader->source);
				std::vector<std::string> feedbackOutput;
				if(definition.size())
				{
					find_and_replace(definition,"\"","");
					std::vector<std::string> elements=split(definition,';');
					feedbackOutput.clear();
					auto u=gEffect->GetStructs().find(new_cs->outputStruct);
					if(u==gEffect->GetStructs().end())
					{
						errSem(string("can't find output struct ")+new_cs->outputStruct);
					}
					else
					for(int i=0;i<elements.size();i++)
					{
						const Struct *struc=(u->second);
						string decl=elements[i];
						vector<string> parts=split(decl,'.');
						if(parts.size()!=1&&parts.size()!=2)
						{
							errSem("Invalid Streamout definition");
							continue;
						}
						string semantic=parts[0];
						string swizzle;
						if(parts.size()==2)
							swizzle=parts[1];
						for(int i=0;i<(int)struc->m_structMembers.size();i++)
						{
							const StructMember &m=struc->m_structMembers[i];
							if(m.semantic==semantic)
							{
								string modified=(new_cs->outputStructName+".")+m.name;
								if(swizzle.size())
								{
								//	modified+=".";
								//	modified+=swizzle;
								}
								feedbackOutput.push_back(modified);
								break;
							}
						}
					}
				}
			}
		}
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 1334 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).lineno								=(yyvsp[(5) - (8)]).lineno;
			(yyval).sType								=(yyvsp[(3) - (8)]).sType;
			string profileName						=(yyvsp[(3) - (8)]).strs[0];
			(yyval).token								=(yyvsp[(3) - (8)]).token;
			ProfileMap::const_iterator i			=gEffect->GetProfileToVersion().find(profileName);
			int version_num							=0;
			if(i==gEffect->GetProfileToVersion().end())
			{
				ostringstream msg;
				msg<<"Profile "<<profileName<<" not found."<<endl;
				errSem(msg.str().c_str());
			}
			else
				version_num=i->second;
			string shaderName=(yyvsp[(5) - (8)]).strs[0];
			stringstream ss;
			ss << (yyvsp[(5) - (8)]).strs[0]<<"Compiled"<<version_num;
			string compiledShaderName						=ss.str();
			(yyval).strs[0]										=compiledShaderName;
			auto u=compilableShaders.find(shaderName);
			if(u==compilableShaders.end())
			{
				errSem(string("Can't find shader ")+shaderName);
			}
			else
			{
				CompilableShader &sh=*(u->second);
				gEffect->AddComputeLayout(shaderName,sh.csLayout);

				gEffect->MergeTextureSamplers(sh.function->textureSamplers,shaderName);

				const CompiledShader *cs=gEffect->AddCompiledShader(compiledShaderName,(yyvsp[(5) - (8)]).strs[0],(yyvsp[(1) - (8)]).sType,version_num);
				(yyval).num=version_num;
				// "Compile" the shader - in the sense of "concatenate the strings and prepare it as GLSL source":
				gEffect->Compile((yyvsp[(1) - (8)]).sType, sh, compiledShaderName);
			}
		}
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 1373 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).num	=(yyvsp[(3) - (4)]).num;
					(yyval).sType=(yyvsp[(1) - (4)]).sType;
				}
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 1384 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).lineno=(yyvsp[(1) - (4)]).lineno;
					(yyval).strs[0]=(yyvsp[(1) - (4)]).strs[0];
					(yyval).strs[1]=(yyvsp[(4) - (4)]).strs[0];
				}
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 1390 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).num=0;
				}
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 1394 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(2) - (2)]).strs[0];
				}
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 1398 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]="";
				}
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 1403 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0];
					(yyval).strs[0]+=(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 1408 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 1412 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					string statement_list=(yyvsp[(1) - (2)]).strs[0];
					string statement=(yyvsp[(2) - (2)]).strs[0];
					(yyval).strs[0]	=statement_list;
					(yyval).strs[0]	+=statement;
					(yyval).strs[1]	=(yyvsp[(1) - (2)]).strs[1]+(yyvsp[(2) - (2)]).strs[1];
				}
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 1421 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					string statement=(yyvsp[(1) - (1)]).strs[0];
					(yyval).strs[0]	=statement;
					(yyval).strs[1]	=(yyvsp[(1) - (1)]).strs[1];
				}
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 1427 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]	="";
					(yyval).strs[1]	="";
				}
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 1432 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		last_linenumber		=(yyvsp[(2) - (4)]).num;
		current_filenumber	=(yyvsp[(3) - (4)]).num;
		global_linenumber	=(yyvsp[(1) - (4)]).lineno+1;
		(yyval).strs[0]			=(yyvsp[(1) - (4)]).strs[0]+" ";
		(yyval).strs[0]			+=(yyvsp[(2) - (4)]).strs[0]+" ";
		(yyval).strs[0]			+=(yyvsp[(3) - (4)]).strs[0];
		(yyval).strs[0]			+='\n';
	}
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 1445 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			string type_spec=(yyvsp[(1) - (2)]).strs[0];
			(yyval).strs[0]=type_spec+" ";
			string init_declarator_list=(yyvsp[(2) - (2)]).strs[0];
			if(type_spec=="float")
				type_spec="";
			stringReplaceAll(init_declarator_list,"%type%",type_spec);
			stringReplaceAll(init_declarator_list,type_spec+type_spec,type_spec);
			(yyval).strs[0]+=init_declarator_list;
		}
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 1456 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		}
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 1460 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
		}
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 1464 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		}
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 1468 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			// An expression of the form vec2 c=....;
			// Pass-through normally, but most GLSL compilers cannot handle array initializers.
			string declarator	=(yyvsp[(1) - (3)]).strs[0];
			string initializer	=(yyvsp[(3) - (3)]).strs[0];
			if((yyvsp[(3) - (3)]).children.size()<=1)
			{
				(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+initializer;
			}
			else
			{
				ostringstream declare_lines;
				/* Here we convert an expression of the form 
										vec2 poss[2]=
										{
											{ 1.0,-1.0},
											{ 1.0, 1.0},
										};
				to the form:
										vec2 poss[2];
										poss[0]=vec2(1.0,-1.0);
										poss[1]=vec2(1.0, 1.0);
				Only trouble is, we don't yet have the "vec2" or whatever the type is. So we will insert a special %type% wildcard for now.
										*/
				
				string name=(yyvsp[(1) - (3)]).strs[0];
				size_t sq_pos=(int)name.find("[");
				// Ugly test to see if we're initializing a list here:
				if(sq_pos<name.length())
				{
					name=name.substr(0,sq_pos);
					declare_lines<<(yyvsp[(1) - (3)]).strs[1]<<"["<<(yyvsp[(3) - (3)]).children.size()<<"];\n";
					for(int i=0;i<(yyvsp[(3) - (3)]).children.size();i++)
					{
						string c=(yyvsp[(3) - (3)]).children[i].strs[0];
						std::replace( c.begin(), c.end(), '{', '('); 
						std::replace( c.begin(), c.end(), '}', ')'); 
						declare_lines<<"\t"<<name<<"["<<i<<"]=%type%"<<c<<";\n";
					}
				}
				else
				{
					declare_lines<<(yyvsp[(1) - (3)]).strs[0]<<";\n";
					// So we're probably initializing a struct instead.
					errSem("Don't yet know how to handle initialization lists for non-arrays!");
				}
				(yyval).strs[0]=declare_lines.str();
			}
		}
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 1518 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		}
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 1522 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
		}
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 1526 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).num=(yyvsp[(3) - (4)]).num;
			(yyval).strs[0]=(yyvsp[(1) - (4)]).strs[0]+(yyvsp[(2) - (4)]).strs[0]+(yyvsp[(3) - (4)]).strs[0]+(yyvsp[(4) - (4)]).strs[0];
			(yyval).strs[1]=(yyvsp[(1) - (4)]).strs[0];
			(yyval).strs[2]=(yyvsp[(3) - (4)]).strs[0];
		}
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 1533 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
			(yyval).strs[1]=(yyvsp[(1) - (3)]).strs[0];
		}
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 1538 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
		}
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 1542 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				(yyval).strs[1]="";
			}
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 1547 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
			}
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 1551 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
				(yyval).lineno=(yyvsp[(1) - (2)]).lineno;
				(yyval).strs[1]="";
			}
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 1557 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				(yyval).strs[1]="";
			}
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 1562 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
				(yyval).strs[1]="";
			}
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 1567 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				string compound_statement=(yyvsp[(1) - (1)]).strs[0];
				(yyval).strs[0]=compound_statement;
				(yyval).strs[1]="";
			}
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 1573 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (5)]).strs[0]+(yyvsp[(2) - (5)]).strs[0]+(yyvsp[(3) - (5)]).strs[0]+(yyvsp[(4) - (5)]).strs[0]+(yyvsp[(5) - (5)]).strs[0];
					(yyval).strs[1]="";
				}
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 1578 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					string s1=(yyvsp[(1) - (7)]).strs[0];
					string s2=(yyvsp[(2) - (7)]).strs[0];
					string s3=(yyvsp[(3) - (7)]).strs[0];
					string s4=(yyvsp[(4) - (7)]).strs[0];
					string s5=(yyvsp[(5) - (7)]).strs[0];
					string s6=(yyvsp[(6) - (7)]).strs[0];
					string s7=(yyvsp[(7) - (7)]).strs[0];
					(yyval).strs[0]=(s1+s2+s3+s4+s5+s6+"\n")+s7;
					(yyval).strs[1]="";
				}
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 1590 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (5)]).strs[0]+(yyvsp[(2) - (5)]).strs[0]+(yyvsp[(3) - (5)]).strs[0]+(yyvsp[(4) - (5)]).strs[0]+(yyvsp[(5) - (5)]).strs[0];
					(yyval).strs[1]="";
				}
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 1595 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (7)]).strs[0]+(yyvsp[(2) - (7)]).strs[0]+(yyvsp[(3) - (7)]).strs[0]+(yyvsp[(4) - (7)]).strs[0]+(yyvsp[(5) - (7)]).strs[0]+(yyvsp[(6) - (7)]).strs[0]+(yyvsp[(7) - (7)]).strs[0];
					(yyval).strs[1]="";
				}
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 1600 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					string statement=(yyvsp[(9) - (9)]).strs[0];
					(yyval).strs[0]=(yyvsp[(1) - (9)]).strs[0]+(yyvsp[(2) - (9)]).strs[0]+(yyvsp[(3) - (9)]).strs[0]+(yyvsp[(4) - (9)]).strs[0]+(yyvsp[(5) - (9)]).strs[0]+(yyvsp[(6) - (9)]).strs[0]+(yyvsp[(7) - (9)]).strs[0]+(yyvsp[(8) - (9)]).strs[0]+statement;
					(yyval).strs[1]="";
				}
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 1606 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
					(yyval).strs[1]="";
				}
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 1611 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
					(yyval).strs[1]="";
				}
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 1616 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
					(yyval).strs[1]="";
				}
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 1621 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				string optional_expression=(yyvsp[(2) - (3)]).strs[0];
				if(read_shader&&optional_expression.length()>0)
				{
					(yyval).strs[0]="";
					(yyval).strs[1]=optional_expression;
				}
				else
				{
					(yyval).strs[0]=((yyvsp[(1) - (3)]).strs[0]+" ")+optional_expression+(yyvsp[(3) - (3)]).strs[0];
					(yyval).strs[1]=optional_expression;
				}
				(yyval).lineno=(yyvsp[(1) - (3)]).lineno;
			}
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 1636 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				(yyval).strs[1]="";
				(yyval).lineno=(yyvsp[(1) - (1)]).lineno;
			}
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 1642 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
			}
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 1646 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
			}
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 1650 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {}
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 1653 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
			}
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 1657 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).strs[0]="";
			}
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 1662 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					string statement_list=(yyvsp[(2) - (3)]).strs[0];
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+statement_list+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 1668 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 1672 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 1676 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		}
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 1680 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			string unary_exp			=(yyvsp[(1) - (3)]).strs[0];
			string assignment_operator	=(yyvsp[(2) - (3)]).strs[0];
			string assignment_exp		=(yyvsp[(3) - (3)]).strs[0];
			string varName				=(yyvsp[(1) - (3)]).strs[1];
			string index				=(yyvsp[(1) - (3)]).strs[3];
			// Is it actually a texture we're indexing?
			GLTextureType glTextureType			=GetTextureType(buildFunction,varName);
			if(glTextureType==0||!IsRWTexture(glTextureType))
			{
				(yyval).strs[0]=unary_exp+assignment_operator+assignment_exp;
			}
			else
			{
				ostringstream str;
				if((yyvsp[(2) - (3)]).token=='=')
					str<<"imageStore("<<varName<<",ivec"<<GetTextureDimension(glTextureType)<<"("<<index<<"),convertToImageFormatof_"<<varName<<"("<<assignment_exp<<"))";
				else
					errSem(string("Unsupported operator for image store: ")+assignment_operator);
				(yyval).strs[0]=str.str();
			}
		}
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 1703 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
					(yyval).token=(yyvsp[(1) - (1)]).token;
				}
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 1717 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		(yyval).token=(yyvsp[(1) - (1)]).token;
	}
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 1722 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
	}
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 1726 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (5)]).strs[0]+(yyvsp[(2) - (5)]).strs[0]+(yyvsp[(3) - (5)]).strs[0]+(yyvsp[(4) - (5)]).strs[0]+(yyvsp[(5) - (5)]).strs[0];
		}
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 1731 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 1735 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 1739 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 1743 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1747 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1751 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1755 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1759 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1763 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
}
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1767 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1772 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
}
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1776 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1780 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1784 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
}
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1788 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1792 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1796 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1800 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1804 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
}
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1808 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1812 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1816 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
}
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1820 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1824 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1828 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
}
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1832 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1836 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1840 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
}
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1844 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
}
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1848 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
	(yyval).strs[1]=(yyvsp[(1) - (1)]).strs[1];
	(yyval).strs[2]=(yyvsp[(1) - (1)]).strs[2];
	(yyval).strs[3]=(yyvsp[(1) - (1)]).strs[3];
}
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1855 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
}
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1859 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
}
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1863 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
}
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1867 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
}
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1871 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]="texture";
}
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1875 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]="textureLod";
}
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1879 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
	(yyval).strs[0]="texelFetch";
}
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1883 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string texture=(yyvsp[(1) - (3)]).strs[0];
						string command=(yyvsp[(3) - (3)]).strs[0];
						(yyval).strs[0]=texture+(yyvsp[(2) - (3)]).strs[0]+command;
						// Put the thing we're sampling into strs[1]
						(yyval).strs[1]=texture;
						// We put the command into strs[2]
						(yyval).strs[2]=command;
					}
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1893 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string texture=(yyvsp[(1) - (3)]).strs[0];
						string command=(yyvsp[(3) - (3)]).strs[0];
						(yyval).strs[0]=texture+(yyvsp[(2) - (3)]).strs[0]+command;
						// Put the thing we're sampling into strs[1]
						(yyval).strs[1]=texture;
						// We put the command into strs[2]
						(yyval).strs[2]=command;
					}
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1903 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string texture=(yyvsp[(1) - (3)]).strs[0];
						string command=(yyvsp[(3) - (3)]).strs[0];
						(yyval).strs[0]=texture+(yyvsp[(2) - (3)]).strs[0]+command;
						// Put the thing we're sampling into strs[1]
						(yyval).strs[1]=texture;
						// We put the command into strs[2]
						(yyval).strs[2]=command;
					}
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1913 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).token=(yyvsp[(1) - (1)]).token;
						string varName = (yyvsp[(1) - (1)]).strs[0];
						(yyval).strs[0]= varName;
						(yyval).strs[2]= varName;			//Command
					}
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1920 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string varName=(yyvsp[(1) - (4)]).strs[0];
						string index=(yyvsp[(3) - (4)]).strs[0];
						(yyval).strs[1]=varName;
						(yyval).strs[3]=index;
						(yyval).strs[2]=(yyvsp[(1) - (4)]).strs[2];			//Command

						// Is it actually a texture we're indexing?
						GLTextureType glTextureType			=GetTextureType(buildFunction,varName);
						if(glTextureType==0)
						{
							(yyval).strs[0]=varName+(yyvsp[(2) - (4)]).strs[0]+index+(yyvsp[(4) - (4)]).strs[0];
						}
						else
						{
							buildFunction.declarations.insert(varName);
							ostringstream str;
							if(IsRWTexture(glTextureType))
							{
								str<<"imageLoad("<<varName<<",ivec"<<GetTextureDimension(glTextureType)<<"("<<index<<"))";
							}
							else
							{
								str<<"texelFetch("<<varName<<",ivec"<<GetTextureDimension(glTextureType)<<"("<<index<<"),0)";
							}
							(yyval).strs[0]=str.str();
						}
					}
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1949 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string textureName	=(yyvsp[(1) - (6)]).strs[1];
						string x			=(yyvsp[(3) - (6)]).strs[0];
						string y			=(yyvsp[(5) - (6)]).strs[0];
						
						(yyval).strs[0]=GetSizeFunction( textureName, x, y,"");
						buildFunction.declarations.insert(textureName);
					}
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1958 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string textureName	=(yyvsp[(1) - (8)]).strs[1];
						string x			=(yyvsp[(3) - (8)]).strs[0];
						string y			=(yyvsp[(5) - (8)]).strs[0];
						string z			=(yyvsp[(7) - (8)]).strs[0];
						(yyval).strs[0]=GetSizeFunction( textureName, x, y,z);
						buildFunction.declarations.insert(textureName);
					}
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1967 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string textureName		=(yyvsp[(1) - (4)]).strs[1];
						string command			=(yyvsp[(1) - (4)]).strs[2];
						string pos,msaa_index;
						if((yyvsp[(3) - (4)]).vars)
						{
							auto vars=*((yyvsp[(3) - (4)]).vars);
							delete (yyvsp[(3) - (4)]).vars;
							pos				=vars[0].identifier;
							
							if(vars.size()>1)
								msaa_index=vars[1].identifier;
						}
						int glTextureType	=(GetTextureType(buildFunction,textureName));
						int return_size=2;
						switch(glTextureType)
						{
						case gsampler1D:
						case sampler1DShadow:
						case gsamplerBuffer:
							return_size=1;
							break;
						case gsampler3D:
						case gsamplerCubeArray:
						case samplerCubeArrayShadow:
						case gsampler2DArray:
						case gsampler2DMSArray:
						case sampler2DArrayShadow:
						case gimage3D:
						case gimage2DArray:
						case gimage2DMSArray:
							return_size=3;
							break;
						default:
							break;
						}
						ostringstream full_expr;
						full_expr<<command<<"("<<textureName<<","<<pos<<".xy";
						if(return_size==3)
							full_expr<<"z,"<<pos<<".w";
						else if(!msaa_index.length())
							full_expr<<","<<pos<<".z";
						else
							full_expr<<",int("<<msaa_index<<")";
						full_expr<<")";
						(yyval).strs[0]				=full_expr.str();
						buildFunction.declarations.insert(textureName);
					}
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 2016 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string textureName		=(yyvsp[(1) - (6)]).strs[1];
						string command			=(yyvsp[(1) - (6)]).strs[2];
						string samplerStateName	=(yyvsp[(3) - (6)]).strs[0];
						string otherParams		=(yyvsp[(5) - (6)]).strs[0];
						
						buildFunction.declarations.insert(textureName);
						// The actual texturing command depends on the type of the texture.
						if(IsDeclared(textureName))
						{
							string type=GetDeclaredType(textureName);
							if(type==string("samplerCube"))
							{
							//.	command="texture";
							}
						}
						if(!IsDeclared(samplerStateName))
						{
							errSem(string("Undeclared sampler state ")+samplerStateName);
						}
						// The first argument is the sampler state, the second is the texcoords, the third if present is the lod.
						// So: to translate this to GLSL, an expression like
						//			mainTexture.Sample(samplerState1,texcoords)
						// becomes:
						//			texture(mainTexture&samplerState1,texcoords)
						// and we will map the string "mainTexture&samplerState1" to mainTexture and samplerState1.

						// Later, if we call the function, we will insert it into the shader code.
						// each Texture parameter must be replaced by a list of all the texture-sampler combinations that use it.
						// each SamplerState parameter must be replaced by a list of the texture-sampler combinations that use it.
						// This might lead to some doubling-up.
						TextureSampler *ts=AddTextureSampler(textureName,samplerStateName);
						string full_expr=(command+"(")+(ts->textureSamplerName()+",")+otherParams+")";
						(yyval).strs[0]=full_expr;

						// There are four possibilities for each sampling command:
						// 1. both the texture and the sampler are declared globally.
						// 2. The texture is a function parameter and the sampler is declared globally.
						// 3. The texture is declared globally and the sampler is a function parameter.
						// 4. The texture and the sampler are function parameters.

						// In case 1., we insert the textureSampler declaration at the top, and in C++ apply that sampler to the texture.

						// In case 2., we replace the texture parameter with as many textureSampler parameters as needed.
						//				In the calls to this function we replace the texture parameter with the textureSamplers.
						// In case 3., we replace the sampler parameter with as many textureSampler parameters as needed.
						//				In the calls to this function we replace the sampler parameter with the textureSamplers.
						// In case 4., we replace the texture with as many textureSampler parameters as needed, and remove the sampler parameters.

						// Initially we will only implement cases 1 and 2.
					}
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 2068 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string expr1=(yyvsp[(1) - (4)]).strs[0];
						string expr2=(yyvsp[(2) - (4)]).strs[0];
						string params;
						auto vars			=*((yyvsp[(3) - (4)]).vars);
						delete (yyvsp[(3) - (4)]).vars;
						string expr4		=(yyvsp[(4) - (4)]).strs[0];
						// ok this could be a function call.(((std::basic_string<char,std::char_traits<char>,std::allocator<char> >*)(&(command)))->_Bx._Buf)[0]=='d'
						string command		=(yyvsp[(1) - (4)]).strs[2];
						const std::map<std::string,std::vector<Function*> > &functions=gEffect->GetFunctions();
						auto j=functions.find(command);
						if(j==functions.end())
						{
							params=(yyvsp[(3) - (4)]).strs[0];
						}
						else
						{
							int num=0;
							for(auto u=j->second.begin();u!=j->second.end();u++)
							{
								if((*u)->parameters.size()!=vars.size())
									continue;
								buildFunction.functionsCalled.insert(*u);
								num++;
							}
							for(auto u=j->second.begin();u!=j->second.end();u++)
							{
								if((*u)->parameters.size()!=vars.size())
									continue;
								const Function &f=**u;
								// If it's a known function, there may need to be a mapping from the texture and sampler parameters.
								// We figure out this mapping by looking at the vars list of the function we're calling.
								//	this is the unmodified list, so it corresponds to the call we're making.
								for(vector<glfxstype::variable>::const_iterator i=f.expanded_parameters.begin();i!=f.expanded_parameters.end();i++)
								{
									string declared_name=i->identifier;
									// That's how the parameter was declared. Is it in the original list?
									int index=GetIndexOfParameter(f.parameters,declared_name);
									auto u=f.textureSamplers.find(declared_name);
									if(u!=f.textureSamplers.end())
									{
										TextureSampler *ts		=u->second;
										int t					=GetIndexOfParameter(f.parameters,ts->textureName);
										int s					=GetIndexOfParameter(f.parameters,ts->samplerStateName);
										string textureName		=ts->textureName;
										string samplerStateName	=ts->samplerStateName;
										if(t>=0)
											textureName			=(vars[t]).identifier;
										if(s>=0)
											samplerStateName	=(vars[s]).identifier;
										TextureSampler *ts_new=AddTextureSampler(textureName,samplerStateName);
										if(params.size())
											params+=",";
										params+=ts_new->textureSamplerName();
									
									}
									// If so we pass the corresponding parameter from the call.
									else if(index>=0)
									{
										const glfxstype::variable &p=vars[index];
										if(params.size())
											params+=",";
										params+=p.identifier;
									}
									else // could be a textureSampler...
									{
										if(params.size())
											params+=",";
										params+=declared_name;
									}
								}
								for(auto j=f.textureSamplers.begin();j!=f.textureSamplers.end();j++)
								{
									TextureSampler *ts		=j->second;
									if(ts->global)
										AddTextureSampler(ts->textureName,ts->samplerStateName)->global=true;
								}
								break;
							}
							if(num==0)
							{
								errSem(string("No matching function definition found for ")+command);
								break;
							}
						}
						(yyval).strs[0]=expr1+expr2+params+expr4;
					}
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 2156 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						// ok this could be a function call.
						string command=(yyvsp[(1) - (3)]).strs[2];
						(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
					}
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 2162 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string expr1=(yyvsp[(1) - (3)]).strs[0];
						string expr3=(yyvsp[(3) - (3)]).strs[0];
						(yyval).strs[2]=expr3;			//Command
						(yyval).strs[0]=expr1+(yyvsp[(2) - (3)]).strs[0]+expr3;
					}
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 2169 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
					}
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 2173 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).strs[0]=(yyvsp[(1) - (2)]).strs[0]+(yyvsp[(2) - (2)]).strs[0];
					}
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 2177 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
						(yyval).vars=(yyvsp[(1) - (3)]).vars;
						YYSTYPE::variable var;
						var.identifier	=(yyvsp[(3) - (3)]).strs[0];
						(yyval).vars->push_back(var);
					}
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 2185 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
						(yyval).vars = new vector<YYSTYPE::variable>;
						YYSTYPE::variable var;
						var.identifier	=(yyvsp[(1) - (1)]).strs[0];
						(yyval).vars->push_back(var);
					}
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 2193 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					string assignment_exp = (yyvsp[(1) - (1)]).strs[0];
					(yyval).strs[0]=assignment_exp;
				}
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 2198 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					vector<glfxstype> &children = (yyval).children;
					children = (yyvsp[(2) - (3)]).children;
					string initlist = (yyvsp[(1) - (3)]).strs[0] + (yyvsp[(2) - (3)]).strs[0] + (yyvsp[(3) - (3)]).strs[0];
					(yyval).strs[0] = initlist;
				}
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 2205 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					vector<glfxstype> &children = (yyval).children;
					children = (yyvsp[(2) - (4)]).children;
					string initlist = (yyvsp[(1) - (4)]).strs[0] + (yyvsp[(2) - (4)]).strs[0] + (yyvsp[(3) - (4)]).strs[0] + (yyvsp[(4) - (4)]).strs[0];
					(yyval).strs[0] = initlist;
				}
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 2212 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					vector<glfxstype> &children = (yyval).children;
					children.clear();
					children.push_back((yyvsp[(1) - (1)]));
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 2219 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					vector<glfxstype> &children = (yyval).children;
					children = (yyvsp[(1) - (3)]).children;
					children.push_back((yyvsp[(3) - (3)]));
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 2226 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					string varName=(yyvsp[(1) - (1)]).strs[0];
					(yyval).strs[0]=varName;
					if(gEffect->IsTextureDeclared(varName))
						buildFunction.declarations.insert(varName);
				}
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 2233 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).token=(yyvsp[(1) - (1)]).token;
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 2238 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 2242 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (3)]).strs[0]+(yyvsp[(2) - (3)]).strs[0]+(yyvsp[(3) - (3)]).strs[0];
				}
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 2246 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 2250 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
				}
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 2254 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		ostringstream layoutDef;
		//layoutDef<<"#line "<<$1.lineno<<endl;
		layoutDef<<"layout"<<(yyvsp[(2) - (2)]).strs[0]<<' '<<(yyvsp[(1) - (2)]).strs[0]<<';'<<endl;
		(yyval).strs[0]=layoutDef.str();
	}
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 2262 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
		ostringstream layoutDef;
		//layoutDef<<"#line "<<$1.lineno<<endl;
		layoutDef<<"layout"<<(yyvsp[(2) - (2)]).strs[0]<<' '<<"out float "<<(yyvsp[(1) - (2)]).strs[0]<<';'<<endl;
		(yyval).strs[0]=layoutDef.str();
	}
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 2270 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    { /* read parenthesis */
		(yyval).strs[0]=glfxreadblock('(', ')');
	}
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 2274 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						string id	=(yyvsp[(4) - (5)]).strs[0];
						(yyval).strs[0]	=id;
						(yyval).num		=-1;
						if(id.length()>1)
						{
							string nstr	=id.substr(1,id.length()-1);
							int num		=atoi(nstr.c_str());
							(yyval).num		=num;
						}
					}
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 2286 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).strs[0]	="";
						(yyval).num		=-1;
					}
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 2291 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).strs[0]		=(yyvsp[(2) - (2)]).strs[0];
					}
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 2295 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).strs[0]		=(yyvsp[(2) - (2)]).strs[0];
					}
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 2299 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).strs[0]="";
					}
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 2303 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno		=(yyvsp[(2) - (3)]).lineno;
					}
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 2306 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
					}
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 2310 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno		=(yyvsp[(2) - (2)]).lineno;
					}
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 2315 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno		=(yyvsp[(1) - (1)]).lineno;
					}
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 2319 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
							(yyval).lineno		=(yyvsp[(3) - (4)]).lineno;
							string name		=(yyvsp[(1) - (4)]).strs[0];
							string value	=(yyvsp[(3) - (4)]).strs[0];
							int token		= (yyvsp[(3) - (4)]).token;
							int index		=(yyvsp[(1) - (4)]).num;
							int ival		=(yyvsp[(3) - (4)]).num;
							float fval		=(yyvsp[(3) - (4)]).fnum;
							bool bval		=(ival!=0);
							if(renderState==RASTERIZER_STATE)
							{
								if(is_equal(name,"FillMode"))
									buildRasterizerState.FillMode=(GLenum)toFillModeGLenum(token);
								else if(is_equal(name,"CullMode"))
									buildRasterizerState.CullMode=(GLenum)toCullModeGLenum(token);
								else if(is_equal(name,"FrontCounterClockwise"))
									buildRasterizerState.FrontCounterClockwise=toBool(value);
								else if(is_equal(name,"DepthBias"))
									buildRasterizerState.DepthBias=ival;
								else if(is_equal(name,"DepthBiasClamp"))
									buildRasterizerState.DepthBiasClamp=fval;
								else if(is_equal(name,"SlopeScaledDepthBias"))
									buildRasterizerState.SlopeScaledDepthBias=fval;
								else if(is_equal(name,"DepthClipEnable"))
									buildRasterizerState.DepthClipEnable=toBool(value);
								else if(is_equal(name,"ScissorEnable"))
									buildRasterizerState.ScissorEnable=toBool(value);
								else if(is_equal(name,"MultisampleEnable"))
									buildRasterizerState.MultisampleEnable=toBool(value);
								else if(is_equal(name,"AntialiasedLineEnable"))
									buildRasterizerState.AntialiasedLineEnable=toBool(value);
								else
								{
									ostringstream str;
									str<<"Unknown rasterizer state command: "<<name;
									errSem(str.str().c_str());
								}
							}
							if(renderState==DEPTHSTENCIL_STATE)
							{
								if(is_equal(name,"DepthEnable"))
									buildDepthStencilState.DepthEnable		=toBool(value);
								else if(is_equal(name,"DepthWriteMask"))
								{
									if(is_equal(value,"ALL"))
										buildDepthStencilState.DepthWriteMask	=1;
									else if(is_equal(value,"ZERO"))
										buildDepthStencilState.DepthWriteMask	=0;
									else 
										errSem("Unknown depth write mask");
								}
								else if(is_equal(name,"DepthFunc"))
									buildDepthStencilState.DepthFunc=(GLenum)toDepthFuncGLEnum(token);
								else
								{
									ostringstream str;
									str<<"Unknown DepthStencil state command: "<<name;
									errSem(str.str().c_str());
								}
							}
							if(renderState==BLEND_STATE)
							{
								if(is_equal(name,"SrcBlend"))
									buildBlendState.SrcBlend=(GLenum)toBlendGLEnum(token);
								else if(is_equal(name,"DestBlend"))
									buildBlendState.DestBlend=(GLenum)toBlendGLEnum(token);
								else if(is_equal(name,"BlendOp"))
									buildBlendState.BlendOp=(GLenum)toBlendOpGLEnum(token);
								else if(is_equal(name,"SrcBlendAlpha"))
									buildBlendState.SrcBlendAlpha=(GLenum)toBlendGLEnum(token);
								else if(is_equal(name,"DestBlendAlpha"))
									buildBlendState.DestBlendAlpha=(GLenum)toBlendGLEnum(token);
								else if(is_equal(name,"BlendOpAlpha"))
									buildBlendState.BlendOpAlpha=(GLenum)toBlendOpGLEnum(token);
								else if(is_equal(name,"AlphaToCoverageEnable"))
									buildBlendState.AlphaToCoverageEnable=toBool(value);
								else if(is_equal(name,"BlendEnable"))
									buildBlendState.BlendEnable[index]=toBool(value);
								else if(is_equal(name,"RenderTargetWriteMask"))
									buildBlendState.RenderTargetWriteMask[index]=ival;
								else
								{
									ostringstream str;
									str<<"Unknown blend state command: "<<name;
									errSem(str.str().c_str());
								}
							}
							if(renderState==SAMPLER_STATE)
							{
								if(is_equal(name,"Filter"))
								{
									buildSamplerState.MinFilter			=(GLenum)toMinFilterModeGLEnum(token);
									buildSamplerState.MagFilter			=(GLenum)toMagFilterModeGLEnum(token);
								}
								else if(is_equal(name,"AddressU"))
									buildSamplerState.AddressU			=(GLenum)toAddressModeGLEnum(token);
								else if(is_equal(name,"AddressV"))
									buildSamplerState.AddressV			=(GLenum)toAddressModeGLEnum(token);
								else if(is_equal(name,"AddressW"))
									buildSamplerState.AddressW			=(GLenum)toAddressModeGLEnum(token);
								else if(is_equal(name,"MipLODBias"))
									buildSamplerState.MipLODBias		=toFloat(value);
								else if(is_equal(name,"MaxLod"))
									buildSamplerState.MaxLod			=toFloat(value);
								else if(is_equal(name,"MinLod"))
									buildSamplerState.MinLod			=toFloat(value);
								else if(is_equal(name,"MaxAnisotropy"))
									buildSamplerState.MaxAnisotropy		=toFloat(value);
								else
								{
									ostringstream str;
									str<<"Unknown Sampler state command: "<<name;
									errSem(str.str().c_str());
								}
							}
						}
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 2437 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
							(yyval).lineno	=(yyvsp[(1) - (1)]).lineno;
							(yyval).num		=-1;
						  (yyval).strs[0]	="";
						}
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 2443 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
							(yyval).lineno	=(yyvsp[(1) - (2)]).lineno;
							(yyval).strs[0]	=(yyvsp[(1) - (2)]).strs[0];
							(yyval).num		=(yyvsp[(2) - (2)]).num;
						}
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 2449 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
							 (yyval).num=(yyvsp[(2) - (3)]).num;
							 (yyval).fnum=(yyvsp[(2) - (3)]).fnum;
						}
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 2454 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
							 (yyval).num=0;
						}
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 2458 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).strs[0] = (yyvsp[(1) - (1)]).strs[0];
					}
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 2463 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).num = (yyvsp[(1) - (1)]).num;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 2469 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).fnum = (yyvsp[(1) - (1)]).fnum;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 269:
/* Line 1792 of yacc.c  */
#line 2475 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 270:
/* Line 1792 of yacc.c  */
#line 2480 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 2485 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 2490 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 2495 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 2500 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						// THis could be different types depending on context: cull or fill.
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 275:
/* Line 1792 of yacc.c  */
#line 2506 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 2511 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 2516 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 2521 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 2526 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 2531 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 2536 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 2541 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 2546 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 2551 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 2556 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token =(yyvsp[(1) - (1)]).token;
					}
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 2561 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 2566 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 2571 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 2576 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 2581 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 2586 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 2591 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 2596 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 2601 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 2606 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 296:
/* Line 1792 of yacc.c  */
#line 2611 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 2616 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 2621 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
				(yyval).token = (yyvsp[(1) - (1)]).token;
			}
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 2626 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 2631 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 2636 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 2641 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 2647 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 2652 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 2657 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 2663 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 2668 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 2674 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 2679 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 2684 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
				(yyval).token = (yyvsp[(1) - (1)]).token;
			}
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 2689 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
				(yyval).token = (yyvsp[(1) - (1)]).token;
			}
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 2694 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
				(yyval).token = (yyvsp[(1) - (1)]).token;
			}
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 2699 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
				(yyval).token = (yyvsp[(1) - (1)]).token;
			}
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 2704 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
				(yyval).token = (yyvsp[(1) - (1)]).token;
			}
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 2709 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
				(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
				(yyval).token = (yyvsp[(1) - (1)]).token;
			}
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 2715 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 2720 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 2725 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
						(yyval).lineno = (yyvsp[(1) - (1)]).lineno;
						(yyval).token = (yyvsp[(1) - (1)]).token;
					}
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 2730 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"
    {
			(yyval).strs[0]=(yyvsp[(1) - (1)]).strs[0];
		}
    break;


/* Line 1792 of yacc.c  */
#line 5943 "src/generated/glfxLALRParser.cpp"
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
#line 2733 "C:\\Users\\v-brflem\\Downloads\\glfx-master\\src\\glfx.ypp"


	GLTextureType ToGLTextureType(int t)
	{
		switch(t)
		{
		case TEXTURE2D:
			return gsampler2D;
		case TEXTURE2DMS:
			return gsampler2DMS;
		case TEXTURE3D:
			return gsampler3D;
		case TEXTURECUBE:
			return gsamplerCube;
		case TEXTURE2DARRAY:
			return gsampler2DArray;
		case RW_TEXTURE2D:
			return gimage2D;
		case IMAGE3D:
		case RW_TEXTURE3D:
		case RW_TEXTURE3D_FLOAT4:
			return gimage3D;
		case RW_TEXTURE2DARRAY:
			return gimage2DArray;
		default:
			return unknwownTextureType;
		};
	}
	string getGLInputSemantic(int s)
	{
		switch(s)
		{
		case SV_DISPATCHTHREADID:
			return "gl_GlobalInvocationID";
		case SV_GROUPTHREADID:
			return "gl_LocalInvocationID";
		case SV_GROUPID:
			return "gl_WorkGroupID";
		case SV_VERTEXID:
			return "gl_VertexID";
		default:
		break;
		};
		return "";
	}
	
	bool IsBufferType(int t)
	{
		switch(t)
		{
		case STRUCTUREDBUFFER:
		case RW_STRUCTUREDBUFFER:
			return true;
		default:
			return false;
		};
	}
void glfxWarning(const char* e)
{
    ostringstream errMsg;
	int lex_linenumber=glfxget_lineno();
	int true_linenumber=lex_linenumber+last_linenumber-global_linenumber;
    errMsg<<current_filenumber<<"("<< true_linenumber<<") : glfx warning: \""<<glfxget_text()<<"\" "<<e<<std::endl;
	gEffect->Log()<<errMsg.str();
}

void glfxerror(const char* e)
{
    errSyn(e);
}

void errSyn(const char* e)
{
    ostringstream errMsg;
	int lex_linenumber=glfxget_lineno();
	int true_linenumber=lex_linenumber+last_linenumber-global_linenumber;
	char txt[500];
	strcpy_s(txt,499,glfxget_text());
	txt[499]=0;
    errMsg<<current_filenumber<<"("<< true_linenumber<<") : glfx syntax error: \""<<txt<<"\" "<<e<<" in "<<glfxGetStateText();
    throw std::runtime_error(errMsg.str());
}

void errSem(const string& str, int lex_linenumber)
{
    ostringstream errMsg;
    
    if(lex_linenumber==-1)
        lex_linenumber= glfxget_lineno();
	int true_linenumber=lex_linenumber+last_linenumber-global_linenumber;
	char txt[500];
	strcpy_s(txt,499,glfxget_text());
	txt[499]=0;
    errMsg<<current_filenumber<<"("<<true_linenumber<<") : glfx error: "<<str<<" in "<<glfxGetStateText();
    throw std::runtime_error(errMsg.str());
}

void resetGlfxParse()
{
	buildFunction.clear();
	buildStruct		=Struct();
	gEffect->Clear();
	read_shader		=false;
	read_function	=false;
}
