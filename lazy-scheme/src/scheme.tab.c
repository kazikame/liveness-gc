/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2013 Free Software Foundation, Inc.
   
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


/* First part of user declarations.  */

/* Line 283 of lalr1.cc  */
#line 38 "scheme.tab.c"


#include "scheme.tab.h"

/* User implementation prologue.  */

/* Line 289 of lalr1.cc  */
#line 46 "scheme.tab.c"
/* Unqualified %code blocks.  */
/* Line 290 of lalr1.cc  */
#line 25 "scheme.y"

    #include <iostream>
    #include <cstdlib>
    #include <fstream>

    #include "SchemeDriver.h"

    static int yylex(Scheme::SchemeParser::semantic_type *yylval,
                     Scheme::SchemeScanner &scanner,
                     Scheme::SchemeDriver &driver);


/* Line 290 of lalr1.cc  */
#line 63 "scheme.tab.c"


# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location) YYUSE(Type)
# define YY_REDUCE_PRINT(Rule)        static_cast<void>(0)
# define YY_STACK_PRINT()             static_cast<void>(0)

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

/* Line 357 of lalr1.cc  */
#line 6 "scheme.y"
namespace Scheme {
/* Line 357 of lalr1.cc  */
#line 159 "scheme.tab.c"

  /// Build a parser object.
  SchemeParser::SchemeParser (SchemeScanner &scanner_yyarg, SchemeDriver &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {
  }

  SchemeParser::~SchemeParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  SchemeParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    std::ostream& yyo = debug_stream ();
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    YYUSE (yytype);
  }


  void
  SchemeParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  SchemeParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
    {
      case 24: /* ICONST */
/* Line 452 of lalr1.cc  */
#line 75 "scheme.y"
        { delete(((*yyvaluep).ival)); };
/* Line 452 of lalr1.cc  */
#line 225 "scheme.tab.c"
        break;
      case 25: /* BCONST */
/* Line 452 of lalr1.cc  */
#line 75 "scheme.y"
        { delete(((*yyvaluep).bval)); };
/* Line 452 of lalr1.cc  */
#line 232 "scheme.tab.c"
        break;
      case 26: /* SCONST */
/* Line 452 of lalr1.cc  */
#line 75 "scheme.y"
        { delete(((*yyvaluep).sval)); };
/* Line 452 of lalr1.cc  */
#line 239 "scheme.tab.c"
        break;
      case 27: /* ID */
/* Line 452 of lalr1.cc  */
#line 75 "scheme.y"
        { delete(((*yyvaluep).sval)); };
/* Line 452 of lalr1.cc  */
#line 246 "scheme.tab.c"
        break;
      case 30: /* list_of_defines */
/* Line 452 of lalr1.cc  */
#line 76 "scheme.y"
        { delete(((*yyvaluep).dnodes)); };
/* Line 452 of lalr1.cc  */
#line 253 "scheme.tab.c"
        break;
      case 31: /* define */
/* Line 452 of lalr1.cc  */
#line 74 "scheme.y"
        { delete(((*yyvaluep).dnode)); };
/* Line 452 of lalr1.cc  */
#line 260 "scheme.tab.c"
        break;
      case 32: /* list_of_identifiers */
/* Line 452 of lalr1.cc  */
#line 76 "scheme.y"
        { delete(((*yyvaluep).inodes)); };
/* Line 452 of lalr1.cc  */
#line 267 "scheme.tab.c"
        break;
      case 33: /* expr */
/* Line 452 of lalr1.cc  */
#line 74 "scheme.y"
        { delete(((*yyvaluep).enode)); };
/* Line 452 of lalr1.cc  */
#line 274 "scheme.tab.c"
        break;
      case 34: /* if_expr */
/* Line 452 of lalr1.cc  */
#line 77 "scheme.y"
        { delete(((*yyvaluep).enode)); };
/* Line 452 of lalr1.cc  */
#line 281 "scheme.tab.c"
        break;
      case 35: /* let_expr */
/* Line 452 of lalr1.cc  */
#line 77 "scheme.y"
        { delete(((*yyvaluep).enode)); };
/* Line 452 of lalr1.cc  */
#line 288 "scheme.tab.c"
        break;
      case 36: /* list_of_exprs */
/* Line 452 of lalr1.cc  */
#line 76 "scheme.y"
        { delete(((*yyvaluep).enodes)); };
/* Line 452 of lalr1.cc  */
#line 295 "scheme.tab.c"
        break;
      case 37: /* primapp_expr */
/* Line 452 of lalr1.cc  */
#line 77 "scheme.y"
        { delete(((*yyvaluep).enode)); };
/* Line 452 of lalr1.cc  */
#line 302 "scheme.tab.c"
        break;
      case 38: /* funapp_expr */
/* Line 452 of lalr1.cc  */
#line 77 "scheme.y"
        { delete(((*yyvaluep).enode)); };
/* Line 452 of lalr1.cc  */
#line 309 "scheme.tab.c"
        break;
      case 39: /* prim_app */
/* Line 452 of lalr1.cc  */
#line 77 "scheme.y"
        { delete(((*yyvaluep).enode)); };
/* Line 452 of lalr1.cc  */
#line 316 "scheme.tab.c"
        break;

      default:
        break;
    }
  }

  void
  SchemeParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  SchemeParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  SchemeParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  SchemeParser::debug_level_type
  SchemeParser::debug_level () const
  {
    return yydebug_;
  }

  void
  SchemeParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  SchemeParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  SchemeParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  SchemeParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    // State.
    int yyn;
    int yylen = 0;
    int yystate = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    static semantic_type yyval_default;
    semantic_type yylval = yyval_default;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_.clear ();
    yysemantic_stack_.clear ();
    yylocation_stack_.clear ();
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
        YYCDEBUG << "Reading a token: ";
        yychar = yylex (&yylval, scanner, driver);
      }

    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    // Compute the default @$.
    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
          case 2:
/* Line 664 of lalr1.cc  */
#line 83 "scheme.y"
    { (yyval.pnode) = new Scheme::AST::ProgramNode((yysemantic_stack_[(2) - (1)].dnodes), (yysemantic_stack_[(2) - (2)].enode)); driver.set_prog((yyval.pnode)); }
    break;

  case 3:
/* Line 664 of lalr1.cc  */
#line 86 "scheme.y"
    { (yyval.dnodes) = new std::list<Scheme::AST::DefineNode *>(); }
    break;

  case 4:
/* Line 664 of lalr1.cc  */
#line 87 "scheme.y"
    { (yyval.dnodes) = (yysemantic_stack_[(2) - (1)].dnodes); (yyval.dnodes)->push_back((yysemantic_stack_[(2) - (2)].dnode)); }
    break;

  case 5:
/* Line 664 of lalr1.cc  */
#line 90 "scheme.y"
    { (yyval.dnode) = new Scheme::AST::DefineNode(new Scheme::AST::IdExprNode((yysemantic_stack_[(8) - (4)].sval)), (yysemantic_stack_[(8) - (5)].inodes), (yysemantic_stack_[(8) - (7)].enode)); }
    break;

  case 6:
/* Line 664 of lalr1.cc  */
#line 93 "scheme.y"
    { (yyval.inodes) = new std::list<Scheme::AST::IdExprNode *>(); }
    break;

  case 7:
/* Line 664 of lalr1.cc  */
#line 94 "scheme.y"
    { (yyval.inodes) = (yysemantic_stack_[(2) - (1)].inodes); (yyval.inodes)->push_back(new Scheme::AST::IdExprNode((yysemantic_stack_[(2) - (2)].sval))); }
    break;

  case 8:
/* Line 664 of lalr1.cc  */
#line 97 "scheme.y"
    { (yyval.enode) = new Scheme::AST::IdExprNode((yysemantic_stack_[(1) - (1)].sval)); }
    break;

  case 9:
/* Line 664 of lalr1.cc  */
#line 98 "scheme.y"
    { (yyval.enode) = new Scheme::AST::IntConstExprNode((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 10:
/* Line 664 of lalr1.cc  */
#line 99 "scheme.y"
    { (yyval.enode) = new Scheme::AST::StrConstExprNode((yysemantic_stack_[(1) - (1)].sval)); }
    break;

  case 11:
/* Line 664 of lalr1.cc  */
#line 100 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BoolConstExprNode((yysemantic_stack_[(1) - (1)].bval)); }
    break;

  case 12:
/* Line 664 of lalr1.cc  */
#line 101 "scheme.y"
    { (yyval.enode) = new Scheme::AST::NilConstExprNode(); }
    break;

  case 13:
/* Line 664 of lalr1.cc  */
#line 102 "scheme.y"
    { (yyval.enode) = (yysemantic_stack_[(1) - (1)].enode); }
    break;

  case 14:
/* Line 664 of lalr1.cc  */
#line 103 "scheme.y"
    { (yyval.enode) = (yysemantic_stack_[(1) - (1)].enode); }
    break;

  case 15:
/* Line 664 of lalr1.cc  */
#line 104 "scheme.y"
    { (yyval.enode) = (yysemantic_stack_[(1) - (1)].enode); }
    break;

  case 16:
/* Line 664 of lalr1.cc  */
#line 105 "scheme.y"
    { (yyval.enode) = (yysemantic_stack_[(1) - (1)].enode); }
    break;

  case 17:
/* Line 664 of lalr1.cc  */
#line 108 "scheme.y"
    { (yyval.enode) = new Scheme::AST::IfExprNode((yysemantic_stack_[(6) - (3)].enode), (yysemantic_stack_[(6) - (4)].enode), (yysemantic_stack_[(6) - (5)].enode)); }
    break;

  case 18:
/* Line 664 of lalr1.cc  */
#line 111 "scheme.y"
    { (yyval.enode) = new Scheme::AST::LetExprNode(new Scheme::AST::IdExprNode((yysemantic_stack_[(10) - (5)].sval)), (yysemantic_stack_[(10) - (6)].enode), (yysemantic_stack_[(10) - (9)].enode)); }
    break;

  case 19:
/* Line 664 of lalr1.cc  */
#line 114 "scheme.y"
    { (yyval.enodes) = new std::list<Scheme::AST::ExprNode *>(); }
    break;

  case 20:
/* Line 664 of lalr1.cc  */
#line 115 "scheme.y"
    { (yyval.enodes) = (yysemantic_stack_[(2) - (1)].enodes); (yyval.enodes)->push_back((yysemantic_stack_[(2) - (2)].enode)); }
    break;

  case 21:
/* Line 664 of lalr1.cc  */
#line 118 "scheme.y"
    { (yyval.enode) = (yysemantic_stack_[(3) - (2)].enode); }
    break;

  case 22:
/* Line 664 of lalr1.cc  */
#line 121 "scheme.y"
    { (yyval.enode) = new Scheme::AST::FuncExprNode(new Scheme::AST::IdExprNode((yysemantic_stack_[(4) - (2)].sval)), (yysemantic_stack_[(4) - (3)].enodes)); }
    break;

  case 23:
/* Line 664 of lalr1.cc  */
#line 124 "scheme.y"
    { (yyval.enode) = new Scheme::AST::UnaryPrimExprNode("pair?", (yysemantic_stack_[(2) - (2)].enode)); }
    break;

  case 24:
/* Line 664 of lalr1.cc  */
#line 125 "scheme.y"
    { (yyval.enode) = new Scheme::AST::UnaryPrimExprNode("null?", (yysemantic_stack_[(2) - (2)].enode)); }
    break;

  case 25:
/* Line 664 of lalr1.cc  */
#line 126 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("eq?", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 26:
/* Line 664 of lalr1.cc  */
#line 127 "scheme.y"
    { (yyval.enode) = new Scheme::AST::UnaryPrimExprNode("car", (yysemantic_stack_[(2) - (2)].enode)); }
    break;

  case 27:
/* Line 664 of lalr1.cc  */
#line 128 "scheme.y"
    { (yyval.enode) = new Scheme::AST::UnaryPrimExprNode("cdr", (yysemantic_stack_[(2) - (2)].enode)); }
    break;

  case 28:
/* Line 664 of lalr1.cc  */
#line 129 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("cons", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 29:
/* Line 664 of lalr1.cc  */
#line 130 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("+", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 30:
/* Line 664 of lalr1.cc  */
#line 131 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("-", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 31:
/* Line 664 of lalr1.cc  */
#line 132 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("*", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 32:
/* Line 664 of lalr1.cc  */
#line 133 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("/", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 33:
/* Line 664 of lalr1.cc  */
#line 134 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("mod", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 34:
/* Line 664 of lalr1.cc  */
#line 135 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("<", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 35:
/* Line 664 of lalr1.cc  */
#line 136 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode(">", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;

  case 36:
/* Line 664 of lalr1.cc  */
#line 137 "scheme.y"
    { (yyval.enode) = new Scheme::AST::BinaryPrimExprNode("=", (yysemantic_stack_[(3) - (2)].enode), (yysemantic_stack_[(3) - (3)].enode)); }
    break;


/* Line 664 of lalr1.cc  */
#line 737 "scheme.tab.c"
      default:
        break;
      }

    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */
        if (yychar <= yyeof_)
          {
            /* Return failure if at end of input.  */
            if (yychar == yyeof_)
              YYABORT;
          }
        else
          {
            yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
            yychar = yyempty_;
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
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (!yy_pact_value_is_default_ (yyn))
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	  YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystate_stack_.height ())
      {
        yydestruct_ ("Cleanup: popping",
                     yystos_[yystate_stack_[0]],
                     &yysemantic_stack_[0],
                     &yylocation_stack_[0]);
        yypop_ ();
      }

    return yyresult;
    }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (yychar != yyempty_)
          {
            /* Make sure we have latest lookahead translation.  See
               comments at user semantic actions for why this is
               necessary.  */
            yytoken = yytranslate_ (yychar);
            yydestruct_ (YY_NULL, yytoken, &yylval, &yylloc);
          }

        while (1 < yystate_stack_.height ())
          {
            yydestruct_ (YY_NULL,
                         yystos_[yystate_stack_[0]],
                         &yysemantic_stack_[0],
                         &yylocation_stack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  // Generate an error message.
  std::string
  SchemeParser::yysyntax_error_ (int, int)
  {
    return YY_("syntax error");
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char SchemeParser::yypact_ninf_ = -17;
  const signed char
  SchemeParser::yypact_[] =
  {
       -17,    14,    15,   -17,    67,   -17,   -17,   -17,   -17,   -17,
     -17,   -17,   -17,   -17,   -17,   -17,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      11,    13,    45,   -17,    24,    89,    45,    45,    45,    45,
      45,   -17,   -17,    45,    45,   -17,   -17,    45,    45,    45,
      -9,    30,    45,    21,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,   -17,   -17,   -17,   -17,   -17,     8,    45,   -17,   -17,
      25,    45,    38,    45,   -17,    48,   -17,    49,    51,   -17,
      45,    53,   -17
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  SchemeParser::yydefact_[] =
  {
         3,     0,     0,     1,     0,    12,     9,    11,    10,     8,
       4,     2,    13,    14,    15,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    23,    24,     0,     0,    26,    27,     0,     0,     0,
       0,     0,     0,     0,    21,    29,    30,    31,    32,    33,
      25,    28,    34,    35,    36,     6,     0,     0,    22,    20,
       0,     0,     0,     0,     7,     0,    17,     0,     0,     5,
       0,     0,    18
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  SchemeParser::yypgoto_[] =
  {
       -17,   -17,   -17,   -17,   -17,   -16,   -17,   -17,   -17,   -17,
     -17,   -17
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  SchemeParser::yydefgoto_[] =
  {
        -1,     1,     2,    10,    70,    11,    12,    13,    53,    14,
      15,    34
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char SchemeParser::yytable_ninf_ = -1;
  const unsigned char
  SchemeParser::yytable_[] =
  {
        36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     3,    50,    52,    51,    65,     4,
      55,    56,    57,    58,    59,    35,    68,    60,    61,    54,
      73,    62,    63,    64,    66,    71,    67,    69,     5,     6,
       7,     8,     9,    76,     5,     6,     7,     8,     9,    35,
       0,    72,    74,    78,    79,    75,    80,    77,    82,     0,
       0,     0,     0,     0,    81,     0,     0,     0,     5,     6,
       7,     8,     9,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
       0,     0,     0,     0,    33,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     0,
      31,    32,     0,     0,     0,     0,    33
  };

  /* YYCHECK.  */
  const signed char
  SchemeParser::yycheck_[] =
  {
        16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,     0,     4,    32,     4,    27,     4,
      36,    37,    38,    39,    40,     4,     5,    43,    44,     5,
       5,    47,    48,    49,     4,    27,    52,    53,    23,    24,
      25,    26,    27,     5,    23,    24,    25,    26,    27,     4,
      -1,    67,    27,     5,     5,    71,     5,    73,     5,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    23,    24,
      25,    26,    27,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    27,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    22,    -1,    -1,    -1,    -1,    27
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  SchemeParser::yystos_[] =
  {
         0,    29,    30,     0,     4,    23,    24,    25,    26,    27,
      31,    33,    34,    35,    37,    38,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    27,    39,     4,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
       4,     4,    33,    36,     5,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    27,     4,    33,     5,    33,
      32,    27,    33,     5,    27,    33,     5,    33,     5,     5,
       5,    33,     5
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  SchemeParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  SchemeParser::yyr1_[] =
  {
         0,    28,    29,    30,    30,    31,    32,    32,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    34,    35,    36,
      36,    37,    38,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  SchemeParser::yyr2_[] =
  {
         0,     2,     2,     0,     2,     8,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,    10,     0,
       2,     3,     4,     2,     2,     3,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3
  };

#if YYDEBUG
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const SchemeParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "COMMA", "LP", "RP", "ADD",
  "SUB", "MUL", "DIV", "MOD", "PAIRQ", "NULLQ", "EQQ", "CONS", "CAR",
  "CDR", "LT", "GT", "EQ", "DEFINE", "LET", "IF", "NIL", "ICONST",
  "BCONST", "SCONST", "ID", "$accept", "program", "list_of_defines",
  "define", "list_of_identifiers", "expr", "if_expr", "let_expr",
  "list_of_exprs", "primapp_expr", "funapp_expr", "prim_app", YY_NULL
  };


  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const SchemeParser::rhs_number_type
  SchemeParser::yyrhs_[] =
  {
        29,     0,    -1,    30,    33,    -1,    -1,    30,    31,    -1,
       4,    20,     4,    27,    32,     5,    33,     5,    -1,    -1,
      32,    27,    -1,    27,    -1,    24,    -1,    26,    -1,    25,
      -1,    23,    -1,    34,    -1,    35,    -1,    37,    -1,    38,
      -1,     4,    22,    33,    33,    33,     5,    -1,     4,    21,
       4,     4,    27,    33,     5,     5,    33,     5,    -1,    -1,
      36,    33,    -1,     4,    39,     5,    -1,     4,    27,    36,
       5,    -1,    11,    33,    -1,    12,    33,    -1,    13,    33,
      33,    -1,    15,    33,    -1,    16,    33,    -1,    14,    33,
      33,    -1,     6,    33,    33,    -1,     7,    33,    33,    -1,
       8,    33,    33,    -1,     9,    33,    33,    -1,    10,    33,
      33,    -1,    17,    33,    33,    -1,    18,    33,    33,    -1,
      19,    33,    33,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  SchemeParser::yyprhs_[] =
  {
         0,     0,     3,     6,     7,    10,    19,    20,    23,    25,
      27,    29,    31,    33,    35,    37,    39,    41,    48,    59,
      60,    63,    67,    72,    75,    78,    82,    85,    88,    92,
      96,   100,   104,   108,   112,   116,   120
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  SchemeParser::yyrline_[] =
  {
         0,    83,    83,    86,    87,    90,    93,    94,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   108,   111,   114,
     115,   118,   121,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137
  };

  // Print the state stack on the debug stream.
  void
  SchemeParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  SchemeParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  SchemeParser::token_number_type
  SchemeParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int SchemeParser::yyeof_ = 0;
  const int SchemeParser::yylast_ = 116;
  const int SchemeParser::yynnts_ = 12;
  const int SchemeParser::yyempty_ = -2;
  const int SchemeParser::yyfinal_ = 3;
  const int SchemeParser::yyterror_ = 1;
  const int SchemeParser::yyerrcode_ = 256;
  const int SchemeParser::yyntokens_ = 28;

  const unsigned int SchemeParser::yyuser_token_number_max_ = 282;
  const SchemeParser::token_number_type SchemeParser::yyundef_token_ = 2;

/* Line 1135 of lalr1.cc  */
#line 6 "scheme.y"
} // Scheme
/* Line 1135 of lalr1.cc  */
#line 1228 "scheme.tab.c"
/* Line 1136 of lalr1.cc  */
#line 140 "scheme.y"

//DO NOT USE bison 3.9 TO BUILD. BISON 3.0 BREAKS scheme.y FILE USE bison 2.7
void Scheme::SchemeParser::error(const Scheme::SchemeParser::location_type &loc, const std::string &err) {
    driver.getErrorStream() << "ERROR near Line " << scanner.get_yylineno() << " [" << err << "]: " << scanner.get_yytext() << std::endl;


}

#include "SchemeScanner.hpp"
static int yylex(Scheme::SchemeParser::semantic_type *yylval, Scheme::SchemeScanner &scanner, Scheme::SchemeDriver &driver) {
    return scanner.yylex(yylval);
}
