// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "scheme.tab.c" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "scheme.tab.h"

// User implementation prologue.

#line 51 "scheme.tab.c" // lalr1.cc:412
// Unqualified %code blocks.
#line 25 "scheme.y" // lalr1.cc:413

    #include <iostream>
    #include <cstdlib>
    #include <fstream>

    #include "SchemeDriver.h"

    static int yylex(Scheme::SchemeParser::semantic_type *yylval,
                     Scheme::SchemeScanner &scanner,
                     Scheme::SchemeDriver &driver);

#line 65 "scheme.tab.c" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif



// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 6 "scheme.y" // lalr1.cc:479
namespace Scheme {
#line 132 "scheme.tab.c" // lalr1.cc:479

  /// Build a parser object.
  SchemeParser::SchemeParser (SchemeScanner &scanner_yyarg, SchemeDriver &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  SchemeParser::~SchemeParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  SchemeParser::syntax_error::syntax_error (const std::string& m)
    : std::runtime_error (m)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  SchemeParser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  SchemeParser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
  {
    value = other.value;
  }


  template <typename Base>
  inline
  SchemeParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v)
    : Base (t)
    , value (v)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  SchemeParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  inline
  SchemeParser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  SchemeParser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  SchemeParser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  SchemeParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
  }

  // by_type.
  inline
  SchemeParser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  SchemeParser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  SchemeParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  SchemeParser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  SchemeParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  SchemeParser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  SchemeParser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  SchemeParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  SchemeParser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  SchemeParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  SchemeParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  SchemeParser::symbol_number_type
  SchemeParser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  SchemeParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  SchemeParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  SchemeParser::stack_symbol_type&
  SchemeParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }


  template <typename Base>
  inline
  void
  SchemeParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.type_get ())
    {
            case 24: // ICONST

#line 75 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.ival)); }
#line 342 "scheme.tab.c" // lalr1.cc:614
        break;

      case 25: // BCONST

#line 75 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.bval)); }
#line 349 "scheme.tab.c" // lalr1.cc:614
        break;

      case 26: // SCONST

#line 75 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.sval)); }
#line 356 "scheme.tab.c" // lalr1.cc:614
        break;

      case 27: // ID

#line 75 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.sval)); }
#line 363 "scheme.tab.c" // lalr1.cc:614
        break;

      case 30: // list_of_defines

#line 76 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.dnodes)); }
#line 370 "scheme.tab.c" // lalr1.cc:614
        break;

      case 31: // define

#line 74 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.dnode)); }
#line 377 "scheme.tab.c" // lalr1.cc:614
        break;

      case 32: // list_of_identifiers

#line 76 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.inodes)); }
#line 384 "scheme.tab.c" // lalr1.cc:614
        break;

      case 33: // expr

#line 74 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.enode)); }
#line 391 "scheme.tab.c" // lalr1.cc:614
        break;

      case 34: // if_expr

#line 77 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.enode)); }
#line 398 "scheme.tab.c" // lalr1.cc:614
        break;

      case 35: // let_expr

#line 77 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.enode)); }
#line 405 "scheme.tab.c" // lalr1.cc:614
        break;

      case 36: // list_of_exprs

#line 76 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.enodes)); }
#line 412 "scheme.tab.c" // lalr1.cc:614
        break;

      case 37: // primapp_expr

#line 77 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.enode)); }
#line 419 "scheme.tab.c" // lalr1.cc:614
        break;

      case 38: // funapp_expr

#line 77 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.enode)); }
#line 426 "scheme.tab.c" // lalr1.cc:614
        break;

      case 39: // prim_app

#line 77 "scheme.y" // lalr1.cc:614
        { delete((yysym.value.enode)); }
#line 433 "scheme.tab.c" // lalr1.cc:614
        break;


      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  SchemeParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  SchemeParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  SchemeParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  SchemeParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
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
#endif // YYDEBUG

  inline SchemeParser::state_type
  SchemeParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

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
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, scanner, driver));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 83 "scheme.y" // lalr1.cc:859
    { (yylhs.value.pnode) = new Scheme::AST::ProgramNode((yystack_[1].value.dnodes), (yystack_[0].value.enode)); driver.set_prog((yylhs.value.pnode)); }
#line 663 "scheme.tab.c" // lalr1.cc:859
    break;

  case 3:
#line 86 "scheme.y" // lalr1.cc:859
    { (yylhs.value.dnodes) = new std::list<Scheme::AST::DefineNode *>(); }
#line 669 "scheme.tab.c" // lalr1.cc:859
    break;

  case 4:
#line 87 "scheme.y" // lalr1.cc:859
    { (yylhs.value.dnodes) = (yystack_[1].value.dnodes); (yylhs.value.dnodes)->push_back((yystack_[0].value.dnode)); }
#line 675 "scheme.tab.c" // lalr1.cc:859
    break;

  case 5:
#line 90 "scheme.y" // lalr1.cc:859
    { (yylhs.value.dnode) = new Scheme::AST::DefineNode(new Scheme::AST::IdExprNode((yystack_[4].value.sval)), (yystack_[3].value.inodes), (yystack_[1].value.enode)); }
#line 681 "scheme.tab.c" // lalr1.cc:859
    break;

  case 6:
#line 93 "scheme.y" // lalr1.cc:859
    { (yylhs.value.inodes) = new std::list<Scheme::AST::IdExprNode *>(); }
#line 687 "scheme.tab.c" // lalr1.cc:859
    break;

  case 7:
#line 94 "scheme.y" // lalr1.cc:859
    { (yylhs.value.inodes) = (yystack_[1].value.inodes); (yylhs.value.inodes)->push_back(new Scheme::AST::IdExprNode((yystack_[0].value.sval))); }
#line 693 "scheme.tab.c" // lalr1.cc:859
    break;

  case 8:
#line 97 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::IdExprNode((yystack_[0].value.sval)); }
#line 699 "scheme.tab.c" // lalr1.cc:859
    break;

  case 9:
#line 98 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::IntConstExprNode((yystack_[0].value.ival)); }
#line 705 "scheme.tab.c" // lalr1.cc:859
    break;

  case 10:
#line 99 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::StrConstExprNode((yystack_[0].value.sval)); }
#line 711 "scheme.tab.c" // lalr1.cc:859
    break;

  case 11:
#line 100 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BoolConstExprNode((yystack_[0].value.bval)); }
#line 717 "scheme.tab.c" // lalr1.cc:859
    break;

  case 12:
#line 101 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::NilConstExprNode(); }
#line 723 "scheme.tab.c" // lalr1.cc:859
    break;

  case 13:
#line 102 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = (yystack_[0].value.enode); }
#line 729 "scheme.tab.c" // lalr1.cc:859
    break;

  case 14:
#line 103 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = (yystack_[0].value.enode); }
#line 735 "scheme.tab.c" // lalr1.cc:859
    break;

  case 15:
#line 104 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = (yystack_[0].value.enode); }
#line 741 "scheme.tab.c" // lalr1.cc:859
    break;

  case 16:
#line 105 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = (yystack_[0].value.enode); }
#line 747 "scheme.tab.c" // lalr1.cc:859
    break;

  case 17:
#line 108 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::IfExprNode((yystack_[3].value.enode), (yystack_[2].value.enode), (yystack_[1].value.enode)); }
#line 753 "scheme.tab.c" // lalr1.cc:859
    break;

  case 18:
#line 111 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::LetExprNode(new Scheme::AST::IdExprNode((yystack_[5].value.sval)), (yystack_[4].value.enode), (yystack_[1].value.enode)); }
#line 759 "scheme.tab.c" // lalr1.cc:859
    break;

  case 19:
#line 114 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enodes) = new std::list<Scheme::AST::ExprNode *>(); }
#line 765 "scheme.tab.c" // lalr1.cc:859
    break;

  case 20:
#line 115 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enodes) = (yystack_[1].value.enodes); (yylhs.value.enodes)->push_back((yystack_[0].value.enode)); }
#line 771 "scheme.tab.c" // lalr1.cc:859
    break;

  case 21:
#line 118 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = (yystack_[1].value.enode); }
#line 777 "scheme.tab.c" // lalr1.cc:859
    break;

  case 22:
#line 121 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::FuncExprNode(new Scheme::AST::IdExprNode((yystack_[2].value.sval)), (yystack_[1].value.enodes)); }
#line 783 "scheme.tab.c" // lalr1.cc:859
    break;

  case 23:
#line 124 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::UnaryPrimExprNode("pair?", (yystack_[0].value.enode)); }
#line 789 "scheme.tab.c" // lalr1.cc:859
    break;

  case 24:
#line 125 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::UnaryPrimExprNode("null?", (yystack_[0].value.enode)); }
#line 795 "scheme.tab.c" // lalr1.cc:859
    break;

  case 25:
#line 126 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("eq?", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 801 "scheme.tab.c" // lalr1.cc:859
    break;

  case 26:
#line 127 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::UnaryPrimExprNode("car", (yystack_[0].value.enode)); }
#line 807 "scheme.tab.c" // lalr1.cc:859
    break;

  case 27:
#line 128 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::UnaryPrimExprNode("cdr", (yystack_[0].value.enode)); }
#line 813 "scheme.tab.c" // lalr1.cc:859
    break;

  case 28:
#line 129 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("cons", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 819 "scheme.tab.c" // lalr1.cc:859
    break;

  case 29:
#line 130 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("+", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 825 "scheme.tab.c" // lalr1.cc:859
    break;

  case 30:
#line 131 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("-", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 831 "scheme.tab.c" // lalr1.cc:859
    break;

  case 31:
#line 132 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("*", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 837 "scheme.tab.c" // lalr1.cc:859
    break;

  case 32:
#line 133 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("/", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 843 "scheme.tab.c" // lalr1.cc:859
    break;

  case 33:
#line 134 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("mod", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 849 "scheme.tab.c" // lalr1.cc:859
    break;

  case 34:
#line 135 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("<", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 855 "scheme.tab.c" // lalr1.cc:859
    break;

  case 35:
#line 136 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode(">", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 861 "scheme.tab.c" // lalr1.cc:859
    break;

  case 36:
#line 137 "scheme.y" // lalr1.cc:859
    { (yylhs.value.enode) = new Scheme::AST::BinaryPrimExprNode("=", (yystack_[1].value.enode), (yystack_[0].value.enode)); }
#line 867 "scheme.tab.c" // lalr1.cc:859
    break;


#line 871 "scheme.tab.c" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
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
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
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

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
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
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  SchemeParser::error (const syntax_error& yyexc)
  {
    error (yyexc.what());
  }

  // Generate an error message.
  std::string
  SchemeParser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char SchemeParser::yypact_ninf_ = -17;

  const signed char SchemeParser::yytable_ninf_ = -1;

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

  const signed char
  SchemeParser::yypgoto_[] =
  {
     -17,   -17,   -17,   -17,   -17,   -16,   -17,   -17,   -17,   -17,
     -17,   -17
  };

  const signed char
  SchemeParser::yydefgoto_[] =
  {
      -1,     1,     2,    10,    70,    11,    12,    13,    53,    14,
      15,    34
  };

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

  const unsigned char
  SchemeParser::yyr1_[] =
  {
       0,    28,    29,    30,    30,    31,    32,    32,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    34,    35,    36,
      36,    37,    38,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39
  };

  const unsigned char
  SchemeParser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     8,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,    10,     0,
       2,     3,     4,     2,     2,     3,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const SchemeParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "COMMA", "LP", "RP", "ADD",
  "SUB", "MUL", "DIV", "MOD", "PAIRQ", "NULLQ", "EQQ", "CONS", "CAR",
  "CDR", "LT", "GT", "EQ", "DEFINE", "LET", "IF", "NIL", "ICONST",
  "BCONST", "SCONST", "ID", "$accept", "program", "list_of_defines",
  "define", "list_of_identifiers", "expr", "if_expr", "let_expr",
  "list_of_exprs", "primapp_expr", "funapp_expr", "prim_app", YY_NULLPTR
  };


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
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  SchemeParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
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
    const unsigned int user_token_number_max_ = 282;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 6 "scheme.y" // lalr1.cc:1167
} // Scheme
#line 1250 "scheme.tab.c" // lalr1.cc:1167
#line 140 "scheme.y" // lalr1.cc:1168


void Scheme::SchemeParser::error(const std::string &err) {
    driver.getErrorStream() << "ERROR near Line " << scanner.get_yylineno() << " [" << err << "]: " << scanner.get_yytext() << std::endl;
}

#include "SchemeScanner.hpp"
static int yylex(Scheme::SchemeParser::semantic_type *yylval, Scheme::SchemeScanner &scanner, Scheme::SchemeDriver &driver) {
    return scanner.yylex(yylval);
}
