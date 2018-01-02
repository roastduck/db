
// Generated from /home/rd/src/db/src/parser/SqlParser.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"


/* parser postinclude section */
#include "BaseParser.h"

#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif




class  SqlParser : public BaseParser {
public:
  enum {
    WhiteSpaces = 1, DATABASE = 2, DATABASES = 3, TABLE = 4, TABLES = 5, 
    SHOW = 6, CREATE = 7, DROP = 8, USE = 9, PRIMARY = 10, KEY = 11, NOT = 12, 
    NULL_TOKEN = 13, INSERT = 14, INTO = 15, VALUES = 16, DELETE = 17, FROM = 18, 
    WHERE = 19, UPDATE = 20, SET = 21, SELECT = 22, IS = 23, DESC = 24, 
    REFERENCES = 25, INDEX = 26, AND = 27, FOREIGN = 28, CHECK = 29, IN = 30, 
    ORDER = 31, GROUP = 32, BY = 33, SUM = 34, AVG = 35, MAX = 36, MIN = 37, 
    Identifier = 38, Unsigned = 39, Number = 40, String = 41, SEMICOLON = 42, 
    LEFT_PARENTHESIS = 43, RIGHT_PARENTHESIS = 44, COMMA = 45, DOT = 46, 
    STAR = 47, LESS_THAN = 48, LESS_EQUAL = 49, GREATER_THAN = 50, GREATER_EQUAL = 51, 
    EQUAL = 52, NOT_EQUAL = 53
  };

  enum {
    RuleProgram = 0, RuleStmt = 1, RuleFieldList = 2, RuleField = 3, RuleType = 4, 
    RuleColumnList = 5, RuleTableList = 6, RuleValueLists = 7, RuleValueList = 8, 
    RuleValue = 9, RuleWhereClauses = 10, RuleWhereClause = 11, RuleCol = 12, 
    RuleColAgg = 13, RuleOp = 14, RuleSetClauses = 15, RuleSetClause = 16, 
    RuleSelector = 17, RuleSelAgg = 18, RuleOrderClause = 19, RuleGroupClause = 20
  };

  SqlParser(antlr4::TokenStream *input);
  ~SqlParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class ProgramContext;
  class StmtContext;
  class FieldListContext;
  class FieldContext;
  class TypeContext;
  class ColumnListContext;
  class TableListContext;
  class ValueListsContext;
  class ValueListContext;
  class ValueContext;
  class WhereClausesContext;
  class WhereClauseContext;
  class ColContext;
  class ColAggContext;
  class OpContext;
  class SetClausesContext;
  class SetClauseContext;
  class SelectorContext;
  class SelAggContext;
  class OrderClauseContext;
  class GroupClauseContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);

   
  };

  ProgramContext* program();

  class  StmtContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *identifierToken = nullptr;;
    SqlParser::FieldListContext *fieldListContext = nullptr;;
    SqlParser::ValueListsContext *valueListsContext = nullptr;;
    SqlParser::WhereClausesContext *whereClausesContext = nullptr;;
    SqlParser::SetClausesContext *setClausesContext = nullptr;;
    SqlParser::GroupClauseContext *groupClauseContext = nullptr;;
    SqlParser::OrderClauseContext *orderClauseContext = nullptr;;
    SqlParser::TableListContext *tableListContext = nullptr;;
    SqlParser::SelAggContext *selAggContext = nullptr;;
    SqlParser::ColumnListContext *columnListContext = nullptr;;
    StmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOW();
    antlr4::tree::TerminalNode *DATABASES();
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *DATABASE();
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *USE();
    antlr4::tree::TerminalNode *TABLES();
    antlr4::tree::TerminalNode *TABLE();
    FieldListContext *fieldList();
    antlr4::tree::TerminalNode *DESC();
    antlr4::tree::TerminalNode *INSERT();
    antlr4::tree::TerminalNode *INTO();
    antlr4::tree::TerminalNode *VALUES();
    ValueListsContext *valueLists();
    antlr4::tree::TerminalNode *DELETE();
    antlr4::tree::TerminalNode *FROM();
    antlr4::tree::TerminalNode *WHERE();
    WhereClausesContext *whereClauses();
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *SET();
    SetClausesContext *setClauses();
    antlr4::tree::TerminalNode *SELECT();
    GroupClauseContext *groupClause();
    OrderClauseContext *orderClause();
    TableListContext *tableList();
    SelAggContext *selAgg();
    antlr4::tree::TerminalNode *INDEX();
    ColumnListContext *columnList();

   
  };

  StmtContext* stmt();

  class  FieldListContext : public antlr4::ParserRuleContext {
  public:
    Cols cols;
    PriIdx priIdx;
    Fors fors;
    Chk chk;
    FieldListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FieldContext *> field();
    FieldContext* field(size_t i);

   
  };

  FieldListContext* fieldList();

  class  FieldContext : public antlr4::ParserRuleContext {
  public:
    Cols * cols;
    PriIdx * priIdx;
    Fors * fors;
    Chk * chk;
    antlr4::Token *identifierToken = nullptr;;
    SqlParser::TypeContext *typeContext = nullptr;;
    SqlParser::ColumnListContext *columnListContext = nullptr;;
    SqlParser::ColumnListContext *referrer = nullptr;;
    SqlParser::ColumnListContext *referee = nullptr;;
    SqlParser::ValueListContext *valueListContext = nullptr;;
    FieldContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    FieldContext(antlr4::ParserRuleContext *parent, size_t invokingState, Cols * cols, PriIdx * priIdx, Fors * fors, Chk * chk);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    TypeContext *type();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *NULL_TOKEN();
    antlr4::tree::TerminalNode *PRIMARY();
    antlr4::tree::TerminalNode *KEY();
    std::vector<ColumnListContext *> columnList();
    ColumnListContext* columnList(size_t i);
    antlr4::tree::TerminalNode *FOREIGN();
    antlr4::tree::TerminalNode *REFERENCES();
    antlr4::tree::TerminalNode *CHECK();
    antlr4::tree::TerminalNode *IN();
    ValueListContext *valueList();

   
  };

  FieldContext* field(Cols * cols,PriIdx * priIdx,Fors * fors,Chk * chk);

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    Type::TypeID typeID;
    int length = 0;
    antlr4::Token *identifierToken = nullptr;;
    antlr4::Token *unsignedToken = nullptr;;
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *Unsigned();

   
  };

  TypeContext* type();

  class  ColumnListContext : public antlr4::ParserRuleContext {
  public:
    Table::Index result;
    antlr4::Token *identifierToken = nullptr;;
    ColumnListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);

   
  };

  ColumnListContext* columnList();

  class  TableListContext : public antlr4::ParserRuleContext {
  public:
    std::vector<std::string> result;
    antlr4::Token *identifierToken = nullptr;;
    TableListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);

   
  };

  TableListContext* tableList();

  class  ValueListsContext : public antlr4::ParserRuleContext {
  public:
    VLists result;
    SqlParser::ValueListContext *valueListContext = nullptr;;
    ValueListsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ValueListContext *> valueList();
    ValueListContext* valueList(size_t i);

   
  };

  ValueListsContext* valueLists();

  class  ValueListContext : public antlr4::ParserRuleContext {
  public:
    VList result;
    SqlParser::ValueContext *valueContext = nullptr;;
    ValueListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);

   
  };

  ValueListContext* valueList();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    Optional<std::string> result;
    antlr4::Token *num = nullptr;;
    antlr4::Token *stringToken = nullptr;;
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Unsigned();
    antlr4::tree::TerminalNode *Number();
    antlr4::tree::TerminalNode *String();
    antlr4::tree::TerminalNode *NULL_TOKEN();

   
  };

  ValueContext* value();

  class  WhereClausesContext : public antlr4::ParserRuleContext {
  public:
    std::string defaultTb = "";
    ICM icm;
    OCM ocm;
    WhereClausesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    WhereClausesContext(antlr4::ParserRuleContext *parent, size_t invokingState, std::string defaultTb = "");
    virtual size_t getRuleIndex() const override;
    std::vector<WhereClauseContext *> whereClause();
    WhereClauseContext* whereClause(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AND();
    antlr4::tree::TerminalNode* AND(size_t i);

   
  };

  WhereClausesContext* whereClauses(std::string defaultTb = "");

  class  WhereClauseContext : public antlr4::ParserRuleContext {
  public:
    std::string defaultTb;
    ICM * icm;
    OCM * ocm;
    SqlParser::ColContext *colContext = nullptr;;
    SqlParser::OpContext *opContext = nullptr;;
    SqlParser::ValueContext *valueContext = nullptr;;
    SqlParser::ColContext *col1 = nullptr;;
    SqlParser::ColContext *col2 = nullptr;;
    WhereClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    WhereClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState, std::string defaultTb, ICM * icm, OCM * ocm);
    virtual size_t getRuleIndex() const override;
    std::vector<ColContext *> col();
    ColContext* col(size_t i);
    OpContext *op();
    ValueContext *value();
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *NULL_TOKEN();
    antlr4::tree::TerminalNode *NOT();

   
  };

  WhereClauseContext* whereClause(std::string defaultTb,ICM * icm,OCM * ocm);

  class  ColContext : public antlr4::ParserRuleContext {
  public:
    std::string defaultTb;
    std::string tb;
    std::string c;
    antlr4::Token *identifierToken = nullptr;;
    ColContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    ColContext(antlr4::ParserRuleContext *parent, size_t invokingState, std::string defaultTb);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);

   
  };

  ColContext* col(std::string defaultTb);

  class  ColAggContext : public antlr4::ParserRuleContext {
  public:
    std::string defaultTb;
    std::string tb;
    std::string c;
    Aggregate::AggType agg;
    SqlParser::ColContext *colContext = nullptr;;
    ColAggContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    ColAggContext(antlr4::ParserRuleContext *parent, size_t invokingState, std::string defaultTb);
    virtual size_t getRuleIndex() const override;
    ColContext *col();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MAX();

   
  };

  ColAggContext* colAgg(std::string defaultTb);

  class  OpContext : public antlr4::ParserRuleContext {
  public:
    Table::ConDir dir;
    OpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

   
  };

  OpContext* op();

  class  SetClausesContext : public antlr4::ParserRuleContext {
  public:
    Table::ColL result;
    SqlParser::SetClauseContext *setClauseContext = nullptr;;
    SetClausesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SetClauseContext *> setClause();
    SetClauseContext* setClause(size_t i);

   
  };

  SetClausesContext* setClauses();

  class  SetClauseContext : public antlr4::ParserRuleContext {
  public:
    std::string k;
    Optional<std::string> v;
    antlr4::Token *identifierToken = nullptr;;
    SqlParser::ValueContext *valueContext = nullptr;;
    SetClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    ValueContext *value();

   
  };

  SetClauseContext* setClause();

  class  SelectorContext : public antlr4::ParserRuleContext {
  public:
    Tgt result;
    SqlParser::ColContext *colContext = nullptr;;
    SelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColContext *> col();
    ColContext* col(size_t i);

   
  };

  SelectorContext* selector();

  class  SelAggContext : public antlr4::ParserRuleContext {
  public:
    Tgt result;
    Agg agg;
    SqlParser::ColAggContext *colAggContext = nullptr;;
    SelAggContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColAggContext *> colAgg();
    ColAggContext* colAgg(size_t i);

   
  };

  SelAggContext* selAgg();

  class  OrderClauseContext : public antlr4::ParserRuleContext {
  public:
    Tgt result;
    SqlParser::SelectorContext *selectorContext = nullptr;;
    OrderClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *BY();
    SelectorContext *selector();

   
  };

  OrderClauseContext* orderClause();

  class  GroupClauseContext : public antlr4::ParserRuleContext {
  public:
    Tgt result;
    SqlParser::SelectorContext *selectorContext = nullptr;;
    GroupClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *BY();
    SelectorContext *selector();

   
  };

  GroupClauseContext* groupClause();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

