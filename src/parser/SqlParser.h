
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
    WHERE = 19, UPDATE = 20, SET = 21, SELECT = 22, IS = 23, INT = 24, CHAR = 25, 
    VARCHAR = 26, DESC = 27, REFERENCES = 28, INDEX = 29, AND = 30, DATE = 31, 
    FLOAT = 32, FOREIGN = 33, Identifier = 34, Int = 35, String = 36, SEMICOLON = 37, 
    LEFT_PARENTHESIS = 38, RIGHT_PARENTHESIS = 39, COMMA = 40, DOT = 41, 
    STAR = 42, LESS_THAN = 43, LESS_EQUAL = 44, GREATER_THAN = 45, GREATER_EQUAL = 46, 
    EQUAL = 47, NOT_EQUAL = 48
  };

  enum {
    RuleProgram = 0, RuleStmt = 1, RuleFieldList = 2, RuleField = 3, RuleType = 4, 
    RuleColumnList = 5, RuleTableList = 6, RuleValueLists = 7, RuleValueList = 8, 
    RuleValue = 9, RuleWhereClauses = 10, RuleWhereClause = 11, RuleCol = 12, 
    RuleOp = 13, RuleSetClauses = 14, RuleSetClause = 15, RuleSelector = 16
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
  class OpContext;
  class SetClausesContext;
  class SetClauseContext;
  class SelectorContext; 

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
    SqlParser::TableListContext *tableListContext = nullptr;;
    SqlParser::SelectorContext *selectorContext = nullptr;;
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
    TableListContext *tableList();
    SelectorContext *selector();
    antlr4::tree::TerminalNode *INDEX();
    ColumnListContext *columnList();

   
  };

  StmtContext* stmt();

  class  FieldListContext : public antlr4::ParserRuleContext {
  public:
    Cols cols;
    PriIdx priIdx;
    Fors fors;
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
    antlr4::Token *identifierToken = nullptr;;
    SqlParser::TypeContext *typeContext = nullptr;;
    SqlParser::ColumnListContext *columnListContext = nullptr;;
    SqlParser::ColumnListContext *referrer = nullptr;;
    SqlParser::ColumnListContext *referee = nullptr;;
    FieldContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    FieldContext(antlr4::ParserRuleContext *parent, size_t invokingState, Cols * cols, PriIdx * priIdx, Fors * fors);
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

   
  };

  FieldContext* field(Cols * cols,PriIdx * priIdx,Fors * fors);

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    Type::TypeID typeID;
    int length = 0;
    antlr4::Token *intToken = nullptr;;
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *Int();
    antlr4::tree::TerminalNode *CHAR();
    antlr4::tree::TerminalNode *VARCHAR();
    antlr4::tree::TerminalNode *DATE();
    antlr4::tree::TerminalNode *FLOAT();

   
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
    antlr4::Token *intToken = nullptr;;
    antlr4::Token *stringToken = nullptr;;
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Int();
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

