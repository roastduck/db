
// Generated from /home/rd/src/db/src/parser/SqlParser.g4 by ANTLR 4.7



#include "SqlParser.h"


/* parser postinclude section */
#include "BaseParser.h"

#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


using namespace antlrcpp;
using namespace antlr4;

SqlParser::SqlParser(TokenStream *input) : BaseParser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

SqlParser::~SqlParser() {
  delete _interpreter;
}

std::string SqlParser::getGrammarFileName() const {
  return "SqlParser.g4";
}

const std::vector<std::string>& SqlParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& SqlParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

SqlParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SqlParser::ProgramContext::EOF() {
  return getToken(SqlParser::EOF, 0);
}

std::vector<SqlParser::StmtContext *> SqlParser::ProgramContext::stmt() {
  return getRuleContexts<SqlParser::StmtContext>();
}

SqlParser::StmtContext* SqlParser::ProgramContext::stmt(size_t i) {
  return getRuleContext<SqlParser::StmtContext>(i);
}


size_t SqlParser::ProgramContext::getRuleIndex() const {
  return SqlParser::RuleProgram;
}


SqlParser::ProgramContext* SqlParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, SqlParser::RuleProgram);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(45);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SqlParser::SHOW)
      | (1ULL << SqlParser::CREATE)
      | (1ULL << SqlParser::DROP)
      | (1ULL << SqlParser::USE)
      | (1ULL << SqlParser::INSERT)
      | (1ULL << SqlParser::DELETE)
      | (1ULL << SqlParser::UPDATE)
      | (1ULL << SqlParser::SELECT)
      | (1ULL << SqlParser::DESC))) != 0)) {
      setState(42);
      stmt();
      setState(47);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(48);
    match(SqlParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

SqlParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SqlParser::StmtContext::SHOW() {
  return getToken(SqlParser::SHOW, 0);
}

tree::TerminalNode* SqlParser::StmtContext::DATABASES() {
  return getToken(SqlParser::DATABASES, 0);
}

tree::TerminalNode* SqlParser::StmtContext::CREATE() {
  return getToken(SqlParser::CREATE, 0);
}

tree::TerminalNode* SqlParser::StmtContext::DATABASE() {
  return getToken(SqlParser::DATABASE, 0);
}

tree::TerminalNode* SqlParser::StmtContext::Identifier() {
  return getToken(SqlParser::Identifier, 0);
}

tree::TerminalNode* SqlParser::StmtContext::DROP() {
  return getToken(SqlParser::DROP, 0);
}

tree::TerminalNode* SqlParser::StmtContext::USE() {
  return getToken(SqlParser::USE, 0);
}

tree::TerminalNode* SqlParser::StmtContext::TABLES() {
  return getToken(SqlParser::TABLES, 0);
}

tree::TerminalNode* SqlParser::StmtContext::TABLE() {
  return getToken(SqlParser::TABLE, 0);
}

SqlParser::FieldListContext* SqlParser::StmtContext::fieldList() {
  return getRuleContext<SqlParser::FieldListContext>(0);
}

tree::TerminalNode* SqlParser::StmtContext::DESC() {
  return getToken(SqlParser::DESC, 0);
}

tree::TerminalNode* SqlParser::StmtContext::INSERT() {
  return getToken(SqlParser::INSERT, 0);
}

tree::TerminalNode* SqlParser::StmtContext::INTO() {
  return getToken(SqlParser::INTO, 0);
}

tree::TerminalNode* SqlParser::StmtContext::VALUES() {
  return getToken(SqlParser::VALUES, 0);
}

SqlParser::ValueListsContext* SqlParser::StmtContext::valueLists() {
  return getRuleContext<SqlParser::ValueListsContext>(0);
}

tree::TerminalNode* SqlParser::StmtContext::DELETE() {
  return getToken(SqlParser::DELETE, 0);
}

tree::TerminalNode* SqlParser::StmtContext::FROM() {
  return getToken(SqlParser::FROM, 0);
}

SqlParser::WhereClausesContext* SqlParser::StmtContext::whereClauses() {
  return getRuleContext<SqlParser::WhereClausesContext>(0);
}

tree::TerminalNode* SqlParser::StmtContext::UPDATE() {
  return getToken(SqlParser::UPDATE, 0);
}

tree::TerminalNode* SqlParser::StmtContext::SET() {
  return getToken(SqlParser::SET, 0);
}

SqlParser::SetClausesContext* SqlParser::StmtContext::setClauses() {
  return getRuleContext<SqlParser::SetClausesContext>(0);
}

tree::TerminalNode* SqlParser::StmtContext::SELECT() {
  return getToken(SqlParser::SELECT, 0);
}

SqlParser::GroupClauseContext* SqlParser::StmtContext::groupClause() {
  return getRuleContext<SqlParser::GroupClauseContext>(0);
}

SqlParser::OrderClauseContext* SqlParser::StmtContext::orderClause() {
  return getRuleContext<SqlParser::OrderClauseContext>(0);
}

SqlParser::TableListContext* SqlParser::StmtContext::tableList() {
  return getRuleContext<SqlParser::TableListContext>(0);
}

SqlParser::SelAggContext* SqlParser::StmtContext::selAgg() {
  return getRuleContext<SqlParser::SelAggContext>(0);
}

tree::TerminalNode* SqlParser::StmtContext::INDEX() {
  return getToken(SqlParser::INDEX, 0);
}

SqlParser::ColumnListContext* SqlParser::StmtContext::columnList() {
  return getRuleContext<SqlParser::ColumnListContext>(0);
}


size_t SqlParser::StmtContext::getRuleIndex() const {
  return SqlParser::RuleStmt;
}


SqlParser::StmtContext* SqlParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 2, SqlParser::RuleStmt);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(171);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(50);
      match(SqlParser::SHOW);
      setState(51);
      match(SqlParser::DATABASES);
      setState(52);
      match(SqlParser::SEMICOLON);
       showDbs(); 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(54);
      match(SqlParser::CREATE);
      setState(55);
      match(SqlParser::DATABASE);
      setState(56);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(57);
      match(SqlParser::SEMICOLON);
       createDb((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(59);
      match(SqlParser::DROP);
      setState(60);
      match(SqlParser::DATABASE);
      setState(61);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(62);
      match(SqlParser::SEMICOLON);
       dropDb((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(64);
      match(SqlParser::USE);
      setState(65);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(66);
      match(SqlParser::SEMICOLON);
       use((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(68);
      match(SqlParser::SHOW);
      setState(69);
      match(SqlParser::TABLES);
      setState(70);
      match(SqlParser::SEMICOLON);
       showTables(); 
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(72);
      match(SqlParser::CREATE);
      setState(73);
      match(SqlParser::TABLE);
      setState(74);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(75);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(76);
      dynamic_cast<StmtContext *>(_localctx)->fieldListContext = fieldList();
      setState(77);
      match(SqlParser::RIGHT_PARENTHESIS);
      setState(78);
      match(SqlParser::SEMICOLON);
       createTable((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->fieldListContext->cols, dynamic_cast<StmtContext *>(_localctx)->fieldListContext->priIdx, dynamic_cast<StmtContext *>(_localctx)->fieldListContext->fors, dynamic_cast<StmtContext *>(_localctx)->fieldListContext->chk); 
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(81);
      match(SqlParser::DROP);
      setState(82);
      match(SqlParser::TABLE);
      setState(83);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(84);
      match(SqlParser::SEMICOLON);
       dropTable((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(86);
      match(SqlParser::DESC);
      setState(87);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(88);
      match(SqlParser::SEMICOLON);
       desc((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(90);
      match(SqlParser::INSERT);
      setState(91);
      match(SqlParser::INTO);
      setState(92);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(93);
      match(SqlParser::VALUES);
      setState(94);
      dynamic_cast<StmtContext *>(_localctx)->valueListsContext = valueLists();
      setState(95);
      match(SqlParser::SEMICOLON);
       insert((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->valueListsContext->result); 
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(98);
      match(SqlParser::DELETE);
      setState(99);
      match(SqlParser::FROM);
      setState(100);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(101);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""));
      setState(102);
      match(SqlParser::SEMICOLON);
       remove((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm); 
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(105);
      match(SqlParser::UPDATE);
      setState(106);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(107);
      match(SqlParser::SET);
      setState(108);
      dynamic_cast<StmtContext *>(_localctx)->setClausesContext = setClauses();
      setState(109);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""));
      setState(110);
      match(SqlParser::SEMICOLON);
       update((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->setClausesContext->result, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm); 
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(113);
      match(SqlParser::SELECT);
      setState(114);
      match(SqlParser::STAR);
      setState(115);
      match(SqlParser::FROM);
      setState(116);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(117);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""));
      setState(118);
      dynamic_cast<StmtContext *>(_localctx)->groupClauseContext = groupClause();
      setState(119);
      dynamic_cast<StmtContext *>(_localctx)->orderClauseContext = orderClause();
      setState(120);
      match(SqlParser::SEMICOLON);

                  select(
                      None(), {(dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")}, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm,
                      dynamic_cast<StmtContext *>(_localctx)->orderClauseContext->result, dynamic_cast<StmtContext *>(_localctx)->groupClauseContext->result
                  );
                
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(123);
      match(SqlParser::SELECT);
      setState(124);
      match(SqlParser::STAR);
      setState(125);
      match(SqlParser::FROM);
      setState(126);
      dynamic_cast<StmtContext *>(_localctx)->tableListContext = tableList();
      setState(127);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses();
      setState(128);
      dynamic_cast<StmtContext *>(_localctx)->groupClauseContext = groupClause();
      setState(129);
      dynamic_cast<StmtContext *>(_localctx)->orderClauseContext = orderClause();
      setState(130);
      match(SqlParser::SEMICOLON);

                  select(
                      None(), dynamic_cast<StmtContext *>(_localctx)->tableListContext->result, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm,
                      dynamic_cast<StmtContext *>(_localctx)->orderClauseContext->result, dynamic_cast<StmtContext *>(_localctx)->groupClauseContext->result
                  );
                
      break;
    }

    case 14: {
      enterOuterAlt(_localctx, 14);
      setState(133);
      match(SqlParser::SELECT);
      setState(134);
      dynamic_cast<StmtContext *>(_localctx)->selAggContext = selAgg();
      setState(135);
      match(SqlParser::FROM);
      setState(136);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(137);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""));
      setState(138);
      dynamic_cast<StmtContext *>(_localctx)->groupClauseContext = groupClause();
      setState(139);
      dynamic_cast<StmtContext *>(_localctx)->orderClauseContext = orderClause();
      setState(140);
      match(SqlParser::SEMICOLON);

                  select(
                      dynamic_cast<StmtContext *>(_localctx)->selAggContext->result, {(dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")}, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm,
                      dynamic_cast<StmtContext *>(_localctx)->orderClauseContext->result, dynamic_cast<StmtContext *>(_localctx)->groupClauseContext->result, dynamic_cast<StmtContext *>(_localctx)->selAggContext->agg
                  );
                
      break;
    }

    case 15: {
      enterOuterAlt(_localctx, 15);
      setState(143);
      match(SqlParser::SELECT);
      setState(144);
      dynamic_cast<StmtContext *>(_localctx)->selAggContext = selAgg();
      setState(145);
      match(SqlParser::FROM);
      setState(146);
      dynamic_cast<StmtContext *>(_localctx)->tableListContext = tableList();
      setState(147);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses();
      setState(148);
      dynamic_cast<StmtContext *>(_localctx)->groupClauseContext = groupClause();
      setState(149);
      dynamic_cast<StmtContext *>(_localctx)->orderClauseContext = orderClause();
      setState(150);
      match(SqlParser::SEMICOLON);

                  select(
                      dynamic_cast<StmtContext *>(_localctx)->selAggContext->result, dynamic_cast<StmtContext *>(_localctx)->tableListContext->result, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm,
                      dynamic_cast<StmtContext *>(_localctx)->orderClauseContext->result, dynamic_cast<StmtContext *>(_localctx)->groupClauseContext->result, dynamic_cast<StmtContext *>(_localctx)->selAggContext->agg
                  );
                
      break;
    }

    case 16: {
      enterOuterAlt(_localctx, 16);
      setState(153);
      match(SqlParser::CREATE);
      setState(154);
      match(SqlParser::INDEX);
      setState(155);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(156);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(157);
      dynamic_cast<StmtContext *>(_localctx)->columnListContext = columnList();
      setState(158);
      match(SqlParser::RIGHT_PARENTHESIS);
      setState(159);
      match(SqlParser::SEMICOLON);
       createIndex((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->columnListContext->result); 
      break;
    }

    case 17: {
      enterOuterAlt(_localctx, 17);
      setState(162);
      match(SqlParser::DROP);
      setState(163);
      match(SqlParser::INDEX);
      setState(164);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(165);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(166);
      dynamic_cast<StmtContext *>(_localctx)->columnListContext = columnList();
      setState(167);
      match(SqlParser::RIGHT_PARENTHESIS);
      setState(168);
      match(SqlParser::SEMICOLON);
       dropIndex((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->columnListContext->result); 
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldListContext ------------------------------------------------------------------

SqlParser::FieldListContext::FieldListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SqlParser::FieldContext *> SqlParser::FieldListContext::field() {
  return getRuleContexts<SqlParser::FieldContext>();
}

SqlParser::FieldContext* SqlParser::FieldListContext::field(size_t i) {
  return getRuleContext<SqlParser::FieldContext>(i);
}


size_t SqlParser::FieldListContext::getRuleIndex() const {
  return SqlParser::RuleFieldList;
}


SqlParser::FieldListContext* SqlParser::fieldList() {
  FieldListContext *_localctx = _tracker.createInstance<FieldListContext>(_ctx, getState());
  enterRule(_localctx, 4, SqlParser::RuleFieldList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(182);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::RIGHT_PARENTHESIS: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case SqlParser::PRIMARY:
      case SqlParser::FOREIGN:
      case SqlParser::CHECK:
      case SqlParser::Identifier: {
        enterOuterAlt(_localctx, 2);
        setState(174);
        field(&_localctx->cols, &_localctx->priIdx, &_localctx->fors, &_localctx->chk);
        setState(179);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SqlParser::COMMA) {
          setState(175);
          match(SqlParser::COMMA);
          setState(176);
          field(&_localctx->cols, &_localctx->priIdx, &_localctx->fors, &_localctx->chk);
          setState(181);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldContext ------------------------------------------------------------------

SqlParser::FieldContext::FieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SqlParser::FieldContext::FieldContext(ParserRuleContext *parent, size_t invokingState, Cols * cols, PriIdx * priIdx, Fors * fors, Chk * chk)
  : ParserRuleContext(parent, invokingState) {
  this->cols = cols;
  this->priIdx = priIdx;
  this->fors = fors;
  this->chk = chk;
}

tree::TerminalNode* SqlParser::FieldContext::Identifier() {
  return getToken(SqlParser::Identifier, 0);
}

SqlParser::TypeContext* SqlParser::FieldContext::type() {
  return getRuleContext<SqlParser::TypeContext>(0);
}

tree::TerminalNode* SqlParser::FieldContext::NOT() {
  return getToken(SqlParser::NOT, 0);
}

tree::TerminalNode* SqlParser::FieldContext::NULL_TOKEN() {
  return getToken(SqlParser::NULL_TOKEN, 0);
}

tree::TerminalNode* SqlParser::FieldContext::PRIMARY() {
  return getToken(SqlParser::PRIMARY, 0);
}

tree::TerminalNode* SqlParser::FieldContext::KEY() {
  return getToken(SqlParser::KEY, 0);
}

std::vector<SqlParser::ColumnListContext *> SqlParser::FieldContext::columnList() {
  return getRuleContexts<SqlParser::ColumnListContext>();
}

SqlParser::ColumnListContext* SqlParser::FieldContext::columnList(size_t i) {
  return getRuleContext<SqlParser::ColumnListContext>(i);
}

tree::TerminalNode* SqlParser::FieldContext::FOREIGN() {
  return getToken(SqlParser::FOREIGN, 0);
}

tree::TerminalNode* SqlParser::FieldContext::REFERENCES() {
  return getToken(SqlParser::REFERENCES, 0);
}

tree::TerminalNode* SqlParser::FieldContext::CHECK() {
  return getToken(SqlParser::CHECK, 0);
}

tree::TerminalNode* SqlParser::FieldContext::IN() {
  return getToken(SqlParser::IN, 0);
}

SqlParser::ValueListContext* SqlParser::FieldContext::valueList() {
  return getRuleContext<SqlParser::ValueListContext>(0);
}


size_t SqlParser::FieldContext::getRuleIndex() const {
  return SqlParser::RuleField;
}


SqlParser::FieldContext* SqlParser::field(Cols * cols,PriIdx * priIdx,Fors * fors,Chk * chk) {
  FieldContext *_localctx = _tracker.createInstance<FieldContext>(_ctx, getState(), cols, priIdx, fors, chk);
  enterRule(_localctx, 6, SqlParser::RuleField);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(221);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(184);
      dynamic_cast<FieldContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(185);
      dynamic_cast<FieldContext *>(_localctx)->typeContext = type();
       cols->push_back(std::make_pair((dynamic_cast<FieldContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<FieldContext *>(_localctx)->identifierToken->getText() : ""), (Column){dynamic_cast<FieldContext *>(_localctx)->typeContext->typeID, dynamic_cast<FieldContext *>(_localctx)->typeContext->length, false})); 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(188);
      dynamic_cast<FieldContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(189);
      dynamic_cast<FieldContext *>(_localctx)->typeContext = type();
      setState(190);
      match(SqlParser::NOT);
      setState(191);
      match(SqlParser::NULL_TOKEN);
       cols->push_back(std::make_pair((dynamic_cast<FieldContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<FieldContext *>(_localctx)->identifierToken->getText() : ""), (Column){dynamic_cast<FieldContext *>(_localctx)->typeContext->typeID, dynamic_cast<FieldContext *>(_localctx)->typeContext->length, true})); 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(194);
      match(SqlParser::PRIMARY);
      setState(195);
      match(SqlParser::KEY);
      setState(196);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(197);
      dynamic_cast<FieldContext *>(_localctx)->columnListContext = columnList();
      setState(198);
      match(SqlParser::RIGHT_PARENTHESIS);

                  if (priIdx->isOk()) throw MultiplePrimaryException();
                  *priIdx = dynamic_cast<FieldContext *>(_localctx)->columnListContext->result;
                
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(201);
      match(SqlParser::FOREIGN);
      setState(202);
      match(SqlParser::KEY);
      setState(203);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(204);
      dynamic_cast<FieldContext *>(_localctx)->referrer = columnList();
      setState(205);
      match(SqlParser::RIGHT_PARENTHESIS);
      setState(206);
      match(SqlParser::REFERENCES);
      setState(207);
      dynamic_cast<FieldContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(208);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(209);
      dynamic_cast<FieldContext *>(_localctx)->referee = columnList();
      setState(210);
      match(SqlParser::RIGHT_PARENTHESIS);
       fors->push_back((TableMgr::ForeignKey){(dynamic_cast<FieldContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<FieldContext *>(_localctx)->identifierToken->getText() : ""), std::move(dynamic_cast<FieldContext *>(_localctx)->referrer->result), std::move(dynamic_cast<FieldContext *>(_localctx)->referee->result)}); 
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(213);
      match(SqlParser::CHECK);
      setState(214);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(215);
      dynamic_cast<FieldContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(216);
      match(SqlParser::IN);
      setState(217);
      dynamic_cast<FieldContext *>(_localctx)->valueListContext = valueList();
      setState(218);
      match(SqlParser::RIGHT_PARENTHESIS);
       (*chk)[(dynamic_cast<FieldContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<FieldContext *>(_localctx)->identifierToken->getText() : "")] = dynamic_cast<FieldContext *>(_localctx)->valueListContext->result; 
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

SqlParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SqlParser::TypeContext::Identifier() {
  return getToken(SqlParser::Identifier, 0);
}

tree::TerminalNode* SqlParser::TypeContext::Unsigned() {
  return getToken(SqlParser::Unsigned, 0);
}


size_t SqlParser::TypeContext::getRuleIndex() const {
  return SqlParser::RuleType;
}


SqlParser::TypeContext* SqlParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 8, SqlParser::RuleType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(230);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(223);
      dynamic_cast<TypeContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(224);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(225);
      dynamic_cast<TypeContext *>(_localctx)->unsignedToken = match(SqlParser::Unsigned);
      setState(226);
      match(SqlParser::RIGHT_PARENTHESIS);
       dynamic_cast<TypeContext *>(_localctx)->typeID =  Type::fromName((dynamic_cast<TypeContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<TypeContext *>(_localctx)->identifierToken->getText() : "")), _localctx->length = std::stoi((dynamic_cast<TypeContext *>(_localctx)->unsignedToken != nullptr ? dynamic_cast<TypeContext *>(_localctx)->unsignedToken->getText() : "")); 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(228);
      dynamic_cast<TypeContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
       dynamic_cast<TypeContext *>(_localctx)->typeID =  Type::fromName((dynamic_cast<TypeContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<TypeContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColumnListContext ------------------------------------------------------------------

SqlParser::ColumnListContext::ColumnListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> SqlParser::ColumnListContext::Identifier() {
  return getTokens(SqlParser::Identifier);
}

tree::TerminalNode* SqlParser::ColumnListContext::Identifier(size_t i) {
  return getToken(SqlParser::Identifier, i);
}


size_t SqlParser::ColumnListContext::getRuleIndex() const {
  return SqlParser::RuleColumnList;
}


SqlParser::ColumnListContext* SqlParser::columnList() {
  ColumnListContext *_localctx = _tracker.createInstance<ColumnListContext>(_ctx, getState());
  enterRule(_localctx, 10, SqlParser::RuleColumnList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(232);
    dynamic_cast<ColumnListContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
    _localctx->result.push_back((dynamic_cast<ColumnListContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<ColumnListContext *>(_localctx)->identifierToken->getText() : ""));
    setState(239);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(234);
      match(SqlParser::COMMA);
      setState(235);
      dynamic_cast<ColumnListContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      _localctx->result.push_back((dynamic_cast<ColumnListContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<ColumnListContext *>(_localctx)->identifierToken->getText() : ""));
      setState(241);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TableListContext ------------------------------------------------------------------

SqlParser::TableListContext::TableListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> SqlParser::TableListContext::Identifier() {
  return getTokens(SqlParser::Identifier);
}

tree::TerminalNode* SqlParser::TableListContext::Identifier(size_t i) {
  return getToken(SqlParser::Identifier, i);
}


size_t SqlParser::TableListContext::getRuleIndex() const {
  return SqlParser::RuleTableList;
}


SqlParser::TableListContext* SqlParser::tableList() {
  TableListContext *_localctx = _tracker.createInstance<TableListContext>(_ctx, getState());
  enterRule(_localctx, 12, SqlParser::RuleTableList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(242);
    dynamic_cast<TableListContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
    _localctx->result.push_back((dynamic_cast<TableListContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<TableListContext *>(_localctx)->identifierToken->getText() : ""));
    setState(247); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(244);
      match(SqlParser::COMMA);
      setState(245);
      dynamic_cast<TableListContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      _localctx->result.push_back((dynamic_cast<TableListContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<TableListContext *>(_localctx)->identifierToken->getText() : ""));
      setState(249); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == SqlParser::COMMA);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueListsContext ------------------------------------------------------------------

SqlParser::ValueListsContext::ValueListsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SqlParser::ValueListContext *> SqlParser::ValueListsContext::valueList() {
  return getRuleContexts<SqlParser::ValueListContext>();
}

SqlParser::ValueListContext* SqlParser::ValueListsContext::valueList(size_t i) {
  return getRuleContext<SqlParser::ValueListContext>(i);
}


size_t SqlParser::ValueListsContext::getRuleIndex() const {
  return SqlParser::RuleValueLists;
}


SqlParser::ValueListsContext* SqlParser::valueLists() {
  ValueListsContext *_localctx = _tracker.createInstance<ValueListsContext>(_ctx, getState());
  enterRule(_localctx, 14, SqlParser::RuleValueLists);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(251);
    dynamic_cast<ValueListsContext *>(_localctx)->valueListContext = valueList();
    append(_localctx->result, dynamic_cast<ValueListsContext *>(_localctx)->valueListContext->result);
    setState(259);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(253);
      match(SqlParser::COMMA);
      setState(254);
      dynamic_cast<ValueListsContext *>(_localctx)->valueListContext = valueList();
      append(_localctx->result, dynamic_cast<ValueListsContext *>(_localctx)->valueListContext->result);
      setState(261);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueListContext ------------------------------------------------------------------

SqlParser::ValueListContext::ValueListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SqlParser::ValueContext *> SqlParser::ValueListContext::value() {
  return getRuleContexts<SqlParser::ValueContext>();
}

SqlParser::ValueContext* SqlParser::ValueListContext::value(size_t i) {
  return getRuleContext<SqlParser::ValueContext>(i);
}


size_t SqlParser::ValueListContext::getRuleIndex() const {
  return SqlParser::RuleValueList;
}


SqlParser::ValueListContext* SqlParser::valueList() {
  ValueListContext *_localctx = _tracker.createInstance<ValueListContext>(_ctx, getState());
  enterRule(_localctx, 16, SqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(277);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::SEMICOLON:
      case SqlParser::RIGHT_PARENTHESIS:
      case SqlParser::COMMA: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case SqlParser::LEFT_PARENTHESIS: {
        enterOuterAlt(_localctx, 2);
        setState(263);
        match(SqlParser::LEFT_PARENTHESIS);
        setState(264);
        dynamic_cast<ValueListContext *>(_localctx)->valueContext = value();
        append(_localctx->result, dynamic_cast<ValueListContext *>(_localctx)->valueContext->result);
        setState(272);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SqlParser::COMMA) {
          setState(266);
          match(SqlParser::COMMA);
          setState(267);
          dynamic_cast<ValueListContext *>(_localctx)->valueContext = value();
          append(_localctx->result, dynamic_cast<ValueListContext *>(_localctx)->valueContext->result);
          setState(274);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(275);
        match(SqlParser::RIGHT_PARENTHESIS);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

SqlParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SqlParser::ValueContext::Unsigned() {
  return getToken(SqlParser::Unsigned, 0);
}

tree::TerminalNode* SqlParser::ValueContext::Number() {
  return getToken(SqlParser::Number, 0);
}

tree::TerminalNode* SqlParser::ValueContext::String() {
  return getToken(SqlParser::String, 0);
}

tree::TerminalNode* SqlParser::ValueContext::NULL_TOKEN() {
  return getToken(SqlParser::NULL_TOKEN, 0);
}


size_t SqlParser::ValueContext::getRuleIndex() const {
  return SqlParser::RuleValue;
}


SqlParser::ValueContext* SqlParser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 18, SqlParser::RuleValue);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(285);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::Unsigned:
      case SqlParser::Number: {
        enterOuterAlt(_localctx, 1);
        setState(279);
        dynamic_cast<ValueContext *>(_localctx)->num = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == SqlParser::Unsigned

        || _la == SqlParser::Number)) {
          dynamic_cast<ValueContext *>(_localctx)->num = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
         dynamic_cast<ValueContext *>(_localctx)->result =  (dynamic_cast<ValueContext *>(_localctx)->num != nullptr ? dynamic_cast<ValueContext *>(_localctx)->num->getText() : ""); 
        break;
      }

      case SqlParser::String: {
        enterOuterAlt(_localctx, 2);
        setState(281);
        dynamic_cast<ValueContext *>(_localctx)->stringToken = match(SqlParser::String);
         dynamic_cast<ValueContext *>(_localctx)->result =  unescape((dynamic_cast<ValueContext *>(_localctx)->stringToken != nullptr ? dynamic_cast<ValueContext *>(_localctx)->stringToken->getText() : "").substr(1, (dynamic_cast<ValueContext *>(_localctx)->stringToken != nullptr ? dynamic_cast<ValueContext *>(_localctx)->stringToken->getText() : "").length() - 2)); 
        break;
      }

      case SqlParser::NULL_TOKEN: {
        enterOuterAlt(_localctx, 3);
        setState(283);
        match(SqlParser::NULL_TOKEN);
         dynamic_cast<ValueContext *>(_localctx)->result =  None(); 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhereClausesContext ------------------------------------------------------------------

SqlParser::WhereClausesContext::WhereClausesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SqlParser::WhereClausesContext::WhereClausesContext(ParserRuleContext *parent, size_t invokingState, std::string defaultTb)
  : ParserRuleContext(parent, invokingState) {
  this->defaultTb = defaultTb;
}

tree::TerminalNode* SqlParser::WhereClausesContext::WHERE() {
  return getToken(SqlParser::WHERE, 0);
}

std::vector<SqlParser::WhereClauseContext *> SqlParser::WhereClausesContext::whereClause() {
  return getRuleContexts<SqlParser::WhereClauseContext>();
}

SqlParser::WhereClauseContext* SqlParser::WhereClausesContext::whereClause(size_t i) {
  return getRuleContext<SqlParser::WhereClauseContext>(i);
}

std::vector<tree::TerminalNode *> SqlParser::WhereClausesContext::AND() {
  return getTokens(SqlParser::AND);
}

tree::TerminalNode* SqlParser::WhereClausesContext::AND(size_t i) {
  return getToken(SqlParser::AND, i);
}


size_t SqlParser::WhereClausesContext::getRuleIndex() const {
  return SqlParser::RuleWhereClauses;
}


SqlParser::WhereClausesContext* SqlParser::whereClauses(std::string defaultTb) {
  WhereClausesContext *_localctx = _tracker.createInstance<WhereClausesContext>(_ctx, getState(), defaultTb);
  enterRule(_localctx, 20, SqlParser::RuleWhereClauses);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(297);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::WHERE: {
        enterOuterAlt(_localctx, 1);
        setState(287);
        match(SqlParser::WHERE);
        setState(288);
        whereClause(defaultTb, &_localctx->icm, &_localctx->ocm);
        setState(293);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SqlParser::AND) {
          setState(289);
          match(SqlParser::AND);
          setState(290);
          whereClause(defaultTb, &_localctx->icm, &_localctx->ocm);
          setState(295);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

      case SqlParser::ORDER:
      case SqlParser::GROUP:
      case SqlParser::SEMICOLON: {
        enterOuterAlt(_localctx, 2);
         dynamic_cast<WhereClausesContext *>(_localctx)->icm =  {}, _localctx->ocm = {}; 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhereClauseContext ------------------------------------------------------------------

SqlParser::WhereClauseContext::WhereClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SqlParser::WhereClauseContext::WhereClauseContext(ParserRuleContext *parent, size_t invokingState, std::string defaultTb, ICM * icm, OCM * ocm)
  : ParserRuleContext(parent, invokingState) {
  this->defaultTb = defaultTb;
  this->icm = icm;
  this->ocm = ocm;
}

std::vector<SqlParser::ColContext *> SqlParser::WhereClauseContext::col() {
  return getRuleContexts<SqlParser::ColContext>();
}

SqlParser::ColContext* SqlParser::WhereClauseContext::col(size_t i) {
  return getRuleContext<SqlParser::ColContext>(i);
}

SqlParser::OpContext* SqlParser::WhereClauseContext::op() {
  return getRuleContext<SqlParser::OpContext>(0);
}

SqlParser::ValueContext* SqlParser::WhereClauseContext::value() {
  return getRuleContext<SqlParser::ValueContext>(0);
}

tree::TerminalNode* SqlParser::WhereClauseContext::IS() {
  return getToken(SqlParser::IS, 0);
}

tree::TerminalNode* SqlParser::WhereClauseContext::NULL_TOKEN() {
  return getToken(SqlParser::NULL_TOKEN, 0);
}

tree::TerminalNode* SqlParser::WhereClauseContext::NOT() {
  return getToken(SqlParser::NOT, 0);
}


size_t SqlParser::WhereClauseContext::getRuleIndex() const {
  return SqlParser::RuleWhereClause;
}


SqlParser::WhereClauseContext* SqlParser::whereClause(std::string defaultTb,ICM * icm,OCM * ocm) {
  WhereClauseContext *_localctx = _tracker.createInstance<WhereClauseContext>(_ctx, getState(), defaultTb, icm, ocm);
  enterRule(_localctx, 22, SqlParser::RuleWhereClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(320);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(299);
      dynamic_cast<WhereClauseContext *>(_localctx)->colContext = col(defaultTb);
      setState(300);
      dynamic_cast<WhereClauseContext *>(_localctx)->opContext = op();
      setState(301);
      dynamic_cast<WhereClauseContext *>(_localctx)->valueContext = value();

                  if (dynamic_cast<WhereClauseContext *>(_localctx)->valueContext->result.isOk())
                      (*_localctx->icm)[dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb][dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c].push_back((Table::ConLiteral){dynamic_cast<WhereClauseContext *>(_localctx)->opContext->dir, dynamic_cast<WhereClauseContext *>(_localctx)->valueContext->result.ok()});
                  else // Trick when the condition is always false
                     (*_localctx->ocm)[std::make_pair(dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb, dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb)].push_back((Table::OuterCon){Table::NE, dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c, dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c});
                
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(304);
      dynamic_cast<WhereClauseContext *>(_localctx)->col1 = col(defaultTb);
      setState(305);
      dynamic_cast<WhereClauseContext *>(_localctx)->opContext = op();
      setState(306);
      dynamic_cast<WhereClauseContext *>(_localctx)->col2 = col(defaultTb);
       (*_localctx->ocm)[std::make_pair(dynamic_cast<WhereClauseContext *>(_localctx)->col1->tb, dynamic_cast<WhereClauseContext *>(_localctx)->col2->tb)].push_back((Table::OuterCon){dynamic_cast<WhereClauseContext *>(_localctx)->opContext->dir, dynamic_cast<WhereClauseContext *>(_localctx)->col1->c, dynamic_cast<WhereClauseContext *>(_localctx)->col2->c}); 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(309);
      dynamic_cast<WhereClauseContext *>(_localctx)->colContext = col(defaultTb);
      setState(310);
      match(SqlParser::IS);
      setState(311);
      match(SqlParser::NULL_TOKEN);
       (*_localctx->icm)[dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb][dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c].push_back((Table::ConLiteral){Table::IS_NULL, ""}); 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(314);
      dynamic_cast<WhereClauseContext *>(_localctx)->colContext = col(defaultTb);
      setState(315);
      match(SqlParser::IS);
      setState(316);
      match(SqlParser::NOT);
      setState(317);
      match(SqlParser::NULL_TOKEN);
       (*_localctx->icm)[dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb][dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c].push_back((Table::ConLiteral){Table::IS_NOT_NULL, ""}); 
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColContext ------------------------------------------------------------------

SqlParser::ColContext::ColContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SqlParser::ColContext::ColContext(ParserRuleContext *parent, size_t invokingState, std::string defaultTb)
  : ParserRuleContext(parent, invokingState) {
  this->defaultTb = defaultTb;
}

std::vector<tree::TerminalNode *> SqlParser::ColContext::Identifier() {
  return getTokens(SqlParser::Identifier);
}

tree::TerminalNode* SqlParser::ColContext::Identifier(size_t i) {
  return getToken(SqlParser::Identifier, i);
}


size_t SqlParser::ColContext::getRuleIndex() const {
  return SqlParser::RuleCol;
}


SqlParser::ColContext* SqlParser::col(std::string defaultTb) {
  ColContext *_localctx = _tracker.createInstance<ColContext>(_ctx, getState(), defaultTb);
  enterRule(_localctx, 24, SqlParser::RuleCol);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(325);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      setState(322);
      dynamic_cast<ColContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      dynamic_cast<ColContext *>(_localctx)->tb =  (dynamic_cast<ColContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<ColContext *>(_localctx)->identifierToken->getText() : "");
      setState(324);
      match(SqlParser::DOT);
      break;
    }

    }
    setState(327);
    dynamic_cast<ColContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
    dynamic_cast<ColContext *>(_localctx)->c =  (dynamic_cast<ColContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<ColContext *>(_localctx)->identifierToken->getText() : "");
     if (_localctx->tb.empty()) dynamic_cast<ColContext *>(_localctx)->tb =  _localctx->defaultTb; 
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColAggContext ------------------------------------------------------------------

SqlParser::ColAggContext::ColAggContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SqlParser::ColAggContext::ColAggContext(ParserRuleContext *parent, size_t invokingState, std::string defaultTb)
  : ParserRuleContext(parent, invokingState) {
  this->defaultTb = defaultTb;
}

SqlParser::ColContext* SqlParser::ColAggContext::col() {
  return getRuleContext<SqlParser::ColContext>(0);
}

tree::TerminalNode* SqlParser::ColAggContext::SUM() {
  return getToken(SqlParser::SUM, 0);
}

tree::TerminalNode* SqlParser::ColAggContext::AVG() {
  return getToken(SqlParser::AVG, 0);
}

tree::TerminalNode* SqlParser::ColAggContext::MIN() {
  return getToken(SqlParser::MIN, 0);
}

tree::TerminalNode* SqlParser::ColAggContext::MAX() {
  return getToken(SqlParser::MAX, 0);
}


size_t SqlParser::ColAggContext::getRuleIndex() const {
  return SqlParser::RuleColAgg;
}


SqlParser::ColAggContext* SqlParser::colAgg(std::string defaultTb) {
  ColAggContext *_localctx = _tracker.createInstance<ColAggContext>(_ctx, getState(), defaultTb);
  enterRule(_localctx, 26, SqlParser::RuleColAgg);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(358);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(331);
        dynamic_cast<ColAggContext *>(_localctx)->colContext = col(defaultTb);
         dynamic_cast<ColAggContext *>(_localctx)->tb =  dynamic_cast<ColAggContext *>(_localctx)->colContext->tb, _localctx->c = dynamic_cast<ColAggContext *>(_localctx)->colContext->c, _localctx->agg = Aggregate::NONE; 
        break;
      }

      case SqlParser::SUM: {
        enterOuterAlt(_localctx, 2);
        setState(334);
        match(SqlParser::SUM);
        setState(335);
        match(SqlParser::LEFT_PARENTHESIS);
        setState(336);
        dynamic_cast<ColAggContext *>(_localctx)->colContext = col(defaultTb);
        setState(337);
        match(SqlParser::RIGHT_PARENTHESIS);
         dynamic_cast<ColAggContext *>(_localctx)->tb =  dynamic_cast<ColAggContext *>(_localctx)->colContext->tb, _localctx->c = dynamic_cast<ColAggContext *>(_localctx)->colContext->c, _localctx->agg = Aggregate::SUM; 
        break;
      }

      case SqlParser::AVG: {
        enterOuterAlt(_localctx, 3);
        setState(340);
        match(SqlParser::AVG);
        setState(341);
        match(SqlParser::LEFT_PARENTHESIS);
        setState(342);
        dynamic_cast<ColAggContext *>(_localctx)->colContext = col(defaultTb);
        setState(343);
        match(SqlParser::RIGHT_PARENTHESIS);
         dynamic_cast<ColAggContext *>(_localctx)->tb =  dynamic_cast<ColAggContext *>(_localctx)->colContext->tb, _localctx->c = dynamic_cast<ColAggContext *>(_localctx)->colContext->c, _localctx->agg = Aggregate::AVG; 
        break;
      }

      case SqlParser::MIN: {
        enterOuterAlt(_localctx, 4);
        setState(346);
        match(SqlParser::MIN);
        setState(347);
        match(SqlParser::LEFT_PARENTHESIS);
        setState(348);
        dynamic_cast<ColAggContext *>(_localctx)->colContext = col(defaultTb);
        setState(349);
        match(SqlParser::RIGHT_PARENTHESIS);
         dynamic_cast<ColAggContext *>(_localctx)->tb =  dynamic_cast<ColAggContext *>(_localctx)->colContext->tb, _localctx->c = dynamic_cast<ColAggContext *>(_localctx)->colContext->c, _localctx->agg = Aggregate::MIN; 
        break;
      }

      case SqlParser::MAX: {
        enterOuterAlt(_localctx, 5);
        setState(352);
        match(SqlParser::MAX);
        setState(353);
        match(SqlParser::LEFT_PARENTHESIS);
        setState(354);
        dynamic_cast<ColAggContext *>(_localctx)->colContext = col(defaultTb);
        setState(355);
        match(SqlParser::RIGHT_PARENTHESIS);
         dynamic_cast<ColAggContext *>(_localctx)->tb =  dynamic_cast<ColAggContext *>(_localctx)->colContext->tb, _localctx->c = dynamic_cast<ColAggContext *>(_localctx)->colContext->c, _localctx->agg = Aggregate::MAX; 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OpContext ------------------------------------------------------------------

SqlParser::OpContext::OpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SqlParser::OpContext::getRuleIndex() const {
  return SqlParser::RuleOp;
}


SqlParser::OpContext* SqlParser::op() {
  OpContext *_localctx = _tracker.createInstance<OpContext>(_ctx, getState());
  enterRule(_localctx, 28, SqlParser::RuleOp);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(372);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::LESS_THAN: {
        enterOuterAlt(_localctx, 1);
        setState(360);
        match(SqlParser::LESS_THAN);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::LT; 
        break;
      }

      case SqlParser::LESS_EQUAL: {
        enterOuterAlt(_localctx, 2);
        setState(362);
        match(SqlParser::LESS_EQUAL);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::LE; 
        break;
      }

      case SqlParser::GREATER_THAN: {
        enterOuterAlt(_localctx, 3);
        setState(364);
        match(SqlParser::GREATER_THAN);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::GT; 
        break;
      }

      case SqlParser::GREATER_EQUAL: {
        enterOuterAlt(_localctx, 4);
        setState(366);
        match(SqlParser::GREATER_EQUAL);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::GE; 
        break;
      }

      case SqlParser::EQUAL: {
        enterOuterAlt(_localctx, 5);
        setState(368);
        match(SqlParser::EQUAL);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::EQ; 
        break;
      }

      case SqlParser::NOT_EQUAL: {
        enterOuterAlt(_localctx, 6);
        setState(370);
        match(SqlParser::NOT_EQUAL);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::NE; 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetClausesContext ------------------------------------------------------------------

SqlParser::SetClausesContext::SetClausesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SqlParser::SetClauseContext *> SqlParser::SetClausesContext::setClause() {
  return getRuleContexts<SqlParser::SetClauseContext>();
}

SqlParser::SetClauseContext* SqlParser::SetClausesContext::setClause(size_t i) {
  return getRuleContext<SqlParser::SetClauseContext>(i);
}


size_t SqlParser::SetClausesContext::getRuleIndex() const {
  return SqlParser::RuleSetClauses;
}


SqlParser::SetClausesContext* SqlParser::setClauses() {
  SetClausesContext *_localctx = _tracker.createInstance<SetClausesContext>(_ctx, getState());
  enterRule(_localctx, 30, SqlParser::RuleSetClauses);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(374);
    dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext = setClause();
    _localctx->result[dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext->k] = dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext->v;
    setState(382);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(376);
      match(SqlParser::COMMA);
      setState(377);
      dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext = setClause();
      _localctx->result[dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext->k] = dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext->v;
      setState(384);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetClauseContext ------------------------------------------------------------------

SqlParser::SetClauseContext::SetClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SqlParser::SetClauseContext::Identifier() {
  return getToken(SqlParser::Identifier, 0);
}

SqlParser::ValueContext* SqlParser::SetClauseContext::value() {
  return getRuleContext<SqlParser::ValueContext>(0);
}


size_t SqlParser::SetClauseContext::getRuleIndex() const {
  return SqlParser::RuleSetClause;
}


SqlParser::SetClauseContext* SqlParser::setClause() {
  SetClauseContext *_localctx = _tracker.createInstance<SetClauseContext>(_ctx, getState());
  enterRule(_localctx, 32, SqlParser::RuleSetClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(385);
    dynamic_cast<SetClauseContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
    setState(386);
    match(SqlParser::EQUAL);
    setState(387);
    dynamic_cast<SetClauseContext *>(_localctx)->valueContext = value();
     dynamic_cast<SetClauseContext *>(_localctx)->k =  (dynamic_cast<SetClauseContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<SetClauseContext *>(_localctx)->identifierToken->getText() : ""), _localctx->v = dynamic_cast<SetClauseContext *>(_localctx)->valueContext->result; 
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectorContext ------------------------------------------------------------------

SqlParser::SelectorContext::SelectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SqlParser::ColContext *> SqlParser::SelectorContext::col() {
  return getRuleContexts<SqlParser::ColContext>();
}

SqlParser::ColContext* SqlParser::SelectorContext::col(size_t i) {
  return getRuleContext<SqlParser::ColContext>(i);
}


size_t SqlParser::SelectorContext::getRuleIndex() const {
  return SqlParser::RuleSelector;
}


SqlParser::SelectorContext* SqlParser::selector() {
  SelectorContext *_localctx = _tracker.createInstance<SelectorContext>(_ctx, getState());
  enterRule(_localctx, 34, SqlParser::RuleSelector);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(390);
    dynamic_cast<SelectorContext *>(_localctx)->colContext = col("");
    _localctx->result[dynamic_cast<SelectorContext *>(_localctx)->colContext->tb].push_back(dynamic_cast<SelectorContext *>(_localctx)->colContext->c);
    setState(398);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(392);
      match(SqlParser::COMMA);
      setState(393);
      dynamic_cast<SelectorContext *>(_localctx)->colContext = col("");
      _localctx->result[dynamic_cast<SelectorContext *>(_localctx)->colContext->tb].push_back(dynamic_cast<SelectorContext *>(_localctx)->colContext->c);
      setState(400);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelAggContext ------------------------------------------------------------------

SqlParser::SelAggContext::SelAggContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SqlParser::ColAggContext *> SqlParser::SelAggContext::colAgg() {
  return getRuleContexts<SqlParser::ColAggContext>();
}

SqlParser::ColAggContext* SqlParser::SelAggContext::colAgg(size_t i) {
  return getRuleContext<SqlParser::ColAggContext>(i);
}


size_t SqlParser::SelAggContext::getRuleIndex() const {
  return SqlParser::RuleSelAgg;
}


SqlParser::SelAggContext* SqlParser::selAgg() {
  SelAggContext *_localctx = _tracker.createInstance<SelAggContext>(_ctx, getState());
  enterRule(_localctx, 36, SqlParser::RuleSelAgg);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(401);
    dynamic_cast<SelAggContext *>(_localctx)->colAggContext = colAgg("");

                _localctx->result[dynamic_cast<SelAggContext *>(_localctx)->colAggContext->tb].push_back(dynamic_cast<SelAggContext *>(_localctx)->colAggContext->c);
                if (dynamic_cast<SelAggContext *>(_localctx)->colAggContext->agg != Aggregate::NONE)
                    _localctx->agg[dynamic_cast<SelAggContext *>(_localctx)->colAggContext->tb][dynamic_cast<SelAggContext *>(_localctx)->colAggContext->c] = dynamic_cast<SelAggContext *>(_localctx)->colAggContext->agg;
              
    setState(409);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(403);
      match(SqlParser::COMMA);
      setState(404);
      dynamic_cast<SelAggContext *>(_localctx)->colAggContext = colAgg("");

                  _localctx->result[dynamic_cast<SelAggContext *>(_localctx)->colAggContext->tb].push_back(dynamic_cast<SelAggContext *>(_localctx)->colAggContext->c);
                  if (dynamic_cast<SelAggContext *>(_localctx)->colAggContext->agg != Aggregate::NONE)
                      _localctx->agg[dynamic_cast<SelAggContext *>(_localctx)->colAggContext->tb][dynamic_cast<SelAggContext *>(_localctx)->colAggContext->c] = dynamic_cast<SelAggContext *>(_localctx)->colAggContext->agg;
                
      setState(411);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderClauseContext ------------------------------------------------------------------

SqlParser::OrderClauseContext::OrderClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SqlParser::OrderClauseContext::ORDER() {
  return getToken(SqlParser::ORDER, 0);
}

tree::TerminalNode* SqlParser::OrderClauseContext::BY() {
  return getToken(SqlParser::BY, 0);
}

SqlParser::SelectorContext* SqlParser::OrderClauseContext::selector() {
  return getRuleContext<SqlParser::SelectorContext>(0);
}


size_t SqlParser::OrderClauseContext::getRuleIndex() const {
  return SqlParser::RuleOrderClause;
}


SqlParser::OrderClauseContext* SqlParser::orderClause() {
  OrderClauseContext *_localctx = _tracker.createInstance<OrderClauseContext>(_ctx, getState());
  enterRule(_localctx, 38, SqlParser::RuleOrderClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(418);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::SEMICOLON: {
        enterOuterAlt(_localctx, 1);
         dynamic_cast<OrderClauseContext *>(_localctx)->result =  {}; 
        break;
      }

      case SqlParser::ORDER: {
        enterOuterAlt(_localctx, 2);
        setState(413);
        match(SqlParser::ORDER);
        setState(414);
        match(SqlParser::BY);
        setState(415);
        dynamic_cast<OrderClauseContext *>(_localctx)->selectorContext = selector();
         dynamic_cast<OrderClauseContext *>(_localctx)->result =  dynamic_cast<OrderClauseContext *>(_localctx)->selectorContext->result; 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupClauseContext ------------------------------------------------------------------

SqlParser::GroupClauseContext::GroupClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SqlParser::GroupClauseContext::GROUP() {
  return getToken(SqlParser::GROUP, 0);
}

tree::TerminalNode* SqlParser::GroupClauseContext::BY() {
  return getToken(SqlParser::BY, 0);
}

SqlParser::SelectorContext* SqlParser::GroupClauseContext::selector() {
  return getRuleContext<SqlParser::SelectorContext>(0);
}


size_t SqlParser::GroupClauseContext::getRuleIndex() const {
  return SqlParser::RuleGroupClause;
}


SqlParser::GroupClauseContext* SqlParser::groupClause() {
  GroupClauseContext *_localctx = _tracker.createInstance<GroupClauseContext>(_ctx, getState());
  enterRule(_localctx, 40, SqlParser::RuleGroupClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(426);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::ORDER:
      case SqlParser::SEMICOLON: {
        enterOuterAlt(_localctx, 1);
         dynamic_cast<GroupClauseContext *>(_localctx)->result =  {}; 
        break;
      }

      case SqlParser::GROUP: {
        enterOuterAlt(_localctx, 2);
        setState(421);
        match(SqlParser::GROUP);
        setState(422);
        match(SqlParser::BY);
        setState(423);
        dynamic_cast<GroupClauseContext *>(_localctx)->selectorContext = selector();
         dynamic_cast<GroupClauseContext *>(_localctx)->result =  dynamic_cast<GroupClauseContext *>(_localctx)->selectorContext->result; 
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> SqlParser::_decisionToDFA;
atn::PredictionContextCache SqlParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SqlParser::_atn;
std::vector<uint16_t> SqlParser::_serializedATN;

std::vector<std::string> SqlParser::_ruleNames = {
  "program", "stmt", "fieldList", "field", "type", "columnList", "tableList", 
  "valueLists", "valueList", "value", "whereClauses", "whereClause", "col", 
  "colAgg", "op", "setClauses", "setClause", "selector", "selAgg", "orderClause", 
  "groupClause"
};

std::vector<std::string> SqlParser::_literalNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "';'", "'('", "')'", "','", "'.'", "'*'", "'<'", 
  "'<='", "'>'", "'>='", "'='", "'<>'"
};

std::vector<std::string> SqlParser::_symbolicNames = {
  "", "WhiteSpaces", "DATABASE", "DATABASES", "TABLE", "TABLES", "SHOW", 
  "CREATE", "DROP", "USE", "PRIMARY", "KEY", "NOT", "NULL_TOKEN", "INSERT", 
  "INTO", "VALUES", "DELETE", "FROM", "WHERE", "UPDATE", "SET", "SELECT", 
  "IS", "DESC", "REFERENCES", "INDEX", "AND", "FOREIGN", "CHECK", "IN", 
  "ORDER", "GROUP", "BY", "SUM", "AVG", "MAX", "MIN", "Identifier", "Unsigned", 
  "Number", "String", "SEMICOLON", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", 
  "COMMA", "DOT", "STAR", "LESS_THAN", "LESS_EQUAL", "GREATER_THAN", "GREATER_EQUAL", 
  "EQUAL", "NOT_EQUAL"
};

dfa::Vocabulary SqlParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> SqlParser::_tokenNames;

SqlParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x37, 0x1af, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x3, 0x2, 0x7, 0x2, 0x2e, 0xa, 0x2, 
    0xc, 0x2, 0xe, 0x2, 0x31, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x5, 0x3, 0xae, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x7, 0x4, 0xb4, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0xb7, 0xb, 0x4, 0x5, 0x4, 
    0xb9, 0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0xe0, 0xa, 0x5, 0x3, 0x6, 
    0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 
    0xe9, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 
    0x7, 0xf0, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0xf3, 0xb, 0x7, 0x3, 0x8, 0x3, 
    0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x6, 0x8, 0xfa, 0xa, 0x8, 0xd, 0x8, 
    0xe, 0x8, 0xfb, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x7, 0x9, 0x104, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x107, 0xb, 0x9, 
    0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xa, 0x7, 0xa, 0x111, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 0x114, 0xb, 
    0xa, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0x118, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x120, 0xa, 0xb, 0x3, 
    0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0x126, 0xa, 0xc, 0xc, 0xc, 
    0xe, 0xc, 0x129, 0xb, 0xc, 0x3, 0xc, 0x5, 0xc, 0x12c, 0xa, 0xc, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x5, 
    0xd, 0x143, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x148, 
    0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x169, 0xa, 0xf, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x177, 
    0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 
    0x11, 0x7, 0x11, 0x17f, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x182, 0xb, 
    0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x18f, 
    0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x192, 0xb, 0x13, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x7, 0x14, 0x19a, 0xa, 0x14, 
    0xc, 0x14, 0xe, 0x14, 0x19d, 0xb, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1a5, 0xa, 0x15, 0x3, 0x16, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x1ad, 
    0xa, 0x16, 0x3, 0x16, 0x2, 0x2, 0x17, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 
    0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 
    0x26, 0x28, 0x2a, 0x2, 0x3, 0x3, 0x2, 0x29, 0x2a, 0x2, 0x1cc, 0x2, 0x2f, 
    0x3, 0x2, 0x2, 0x2, 0x4, 0xad, 0x3, 0x2, 0x2, 0x2, 0x6, 0xb8, 0x3, 0x2, 
    0x2, 0x2, 0x8, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xa, 0xe8, 0x3, 0x2, 0x2, 0x2, 
    0xc, 0xea, 0x3, 0x2, 0x2, 0x2, 0xe, 0xf4, 0x3, 0x2, 0x2, 0x2, 0x10, 
    0xfd, 0x3, 0x2, 0x2, 0x2, 0x12, 0x117, 0x3, 0x2, 0x2, 0x2, 0x14, 0x11f, 
    0x3, 0x2, 0x2, 0x2, 0x16, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x18, 0x142, 0x3, 
    0x2, 0x2, 0x2, 0x1a, 0x147, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x168, 0x3, 0x2, 
    0x2, 0x2, 0x1e, 0x176, 0x3, 0x2, 0x2, 0x2, 0x20, 0x178, 0x3, 0x2, 0x2, 
    0x2, 0x22, 0x183, 0x3, 0x2, 0x2, 0x2, 0x24, 0x188, 0x3, 0x2, 0x2, 0x2, 
    0x26, 0x193, 0x3, 0x2, 0x2, 0x2, 0x28, 0x1a4, 0x3, 0x2, 0x2, 0x2, 0x2a, 
    0x1ac, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x2e, 0x5, 0x4, 0x3, 0x2, 0x2d, 0x2c, 
    0x3, 0x2, 0x2, 0x2, 0x2e, 0x31, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x2d, 0x3, 
    0x2, 0x2, 0x2, 0x2f, 0x30, 0x3, 0x2, 0x2, 0x2, 0x30, 0x32, 0x3, 0x2, 
    0x2, 0x2, 0x31, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33, 0x7, 0x2, 0x2, 
    0x3, 0x33, 0x3, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0x7, 0x8, 0x2, 0x2, 
    0x35, 0x36, 0x7, 0x5, 0x2, 0x2, 0x36, 0x37, 0x7, 0x2c, 0x2, 0x2, 0x37, 
    0xae, 0x8, 0x3, 0x1, 0x2, 0x38, 0x39, 0x7, 0x9, 0x2, 0x2, 0x39, 0x3a, 
    0x7, 0x4, 0x2, 0x2, 0x3a, 0x3b, 0x7, 0x28, 0x2, 0x2, 0x3b, 0x3c, 0x7, 
    0x2c, 0x2, 0x2, 0x3c, 0xae, 0x8, 0x3, 0x1, 0x2, 0x3d, 0x3e, 0x7, 0xa, 
    0x2, 0x2, 0x3e, 0x3f, 0x7, 0x4, 0x2, 0x2, 0x3f, 0x40, 0x7, 0x28, 0x2, 
    0x2, 0x40, 0x41, 0x7, 0x2c, 0x2, 0x2, 0x41, 0xae, 0x8, 0x3, 0x1, 0x2, 
    0x42, 0x43, 0x7, 0xb, 0x2, 0x2, 0x43, 0x44, 0x7, 0x28, 0x2, 0x2, 0x44, 
    0x45, 0x7, 0x2c, 0x2, 0x2, 0x45, 0xae, 0x8, 0x3, 0x1, 0x2, 0x46, 0x47, 
    0x7, 0x8, 0x2, 0x2, 0x47, 0x48, 0x7, 0x7, 0x2, 0x2, 0x48, 0x49, 0x7, 
    0x2c, 0x2, 0x2, 0x49, 0xae, 0x8, 0x3, 0x1, 0x2, 0x4a, 0x4b, 0x7, 0x9, 
    0x2, 0x2, 0x4b, 0x4c, 0x7, 0x6, 0x2, 0x2, 0x4c, 0x4d, 0x7, 0x28, 0x2, 
    0x2, 0x4d, 0x4e, 0x7, 0x2d, 0x2, 0x2, 0x4e, 0x4f, 0x5, 0x6, 0x4, 0x2, 
    0x4f, 0x50, 0x7, 0x2e, 0x2, 0x2, 0x50, 0x51, 0x7, 0x2c, 0x2, 0x2, 0x51, 
    0x52, 0x8, 0x3, 0x1, 0x2, 0x52, 0xae, 0x3, 0x2, 0x2, 0x2, 0x53, 0x54, 
    0x7, 0xa, 0x2, 0x2, 0x54, 0x55, 0x7, 0x6, 0x2, 0x2, 0x55, 0x56, 0x7, 
    0x28, 0x2, 0x2, 0x56, 0x57, 0x7, 0x2c, 0x2, 0x2, 0x57, 0xae, 0x8, 0x3, 
    0x1, 0x2, 0x58, 0x59, 0x7, 0x1a, 0x2, 0x2, 0x59, 0x5a, 0x7, 0x28, 0x2, 
    0x2, 0x5a, 0x5b, 0x7, 0x2c, 0x2, 0x2, 0x5b, 0xae, 0x8, 0x3, 0x1, 0x2, 
    0x5c, 0x5d, 0x7, 0x10, 0x2, 0x2, 0x5d, 0x5e, 0x7, 0x11, 0x2, 0x2, 0x5e, 
    0x5f, 0x7, 0x28, 0x2, 0x2, 0x5f, 0x60, 0x7, 0x12, 0x2, 0x2, 0x60, 0x61, 
    0x5, 0x10, 0x9, 0x2, 0x61, 0x62, 0x7, 0x2c, 0x2, 0x2, 0x62, 0x63, 0x8, 
    0x3, 0x1, 0x2, 0x63, 0xae, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 0x7, 0x13, 
    0x2, 0x2, 0x65, 0x66, 0x7, 0x14, 0x2, 0x2, 0x66, 0x67, 0x7, 0x28, 0x2, 
    0x2, 0x67, 0x68, 0x5, 0x16, 0xc, 0x2, 0x68, 0x69, 0x7, 0x2c, 0x2, 0x2, 
    0x69, 0x6a, 0x8, 0x3, 0x1, 0x2, 0x6a, 0xae, 0x3, 0x2, 0x2, 0x2, 0x6b, 
    0x6c, 0x7, 0x16, 0x2, 0x2, 0x6c, 0x6d, 0x7, 0x28, 0x2, 0x2, 0x6d, 0x6e, 
    0x7, 0x17, 0x2, 0x2, 0x6e, 0x6f, 0x5, 0x20, 0x11, 0x2, 0x6f, 0x70, 0x5, 
    0x16, 0xc, 0x2, 0x70, 0x71, 0x7, 0x2c, 0x2, 0x2, 0x71, 0x72, 0x8, 0x3, 
    0x1, 0x2, 0x72, 0xae, 0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 0x7, 0x18, 0x2, 
    0x2, 0x74, 0x75, 0x7, 0x31, 0x2, 0x2, 0x75, 0x76, 0x7, 0x14, 0x2, 0x2, 
    0x76, 0x77, 0x7, 0x28, 0x2, 0x2, 0x77, 0x78, 0x5, 0x16, 0xc, 0x2, 0x78, 
    0x79, 0x5, 0x2a, 0x16, 0x2, 0x79, 0x7a, 0x5, 0x28, 0x15, 0x2, 0x7a, 
    0x7b, 0x7, 0x2c, 0x2, 0x2, 0x7b, 0x7c, 0x8, 0x3, 0x1, 0x2, 0x7c, 0xae, 
    0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 0x7, 0x18, 0x2, 0x2, 0x7e, 0x7f, 0x7, 
    0x31, 0x2, 0x2, 0x7f, 0x80, 0x7, 0x14, 0x2, 0x2, 0x80, 0x81, 0x5, 0xe, 
    0x8, 0x2, 0x81, 0x82, 0x5, 0x16, 0xc, 0x2, 0x82, 0x83, 0x5, 0x2a, 0x16, 
    0x2, 0x83, 0x84, 0x5, 0x28, 0x15, 0x2, 0x84, 0x85, 0x7, 0x2c, 0x2, 0x2, 
    0x85, 0x86, 0x8, 0x3, 0x1, 0x2, 0x86, 0xae, 0x3, 0x2, 0x2, 0x2, 0x87, 
    0x88, 0x7, 0x18, 0x2, 0x2, 0x88, 0x89, 0x5, 0x26, 0x14, 0x2, 0x89, 0x8a, 
    0x7, 0x14, 0x2, 0x2, 0x8a, 0x8b, 0x7, 0x28, 0x2, 0x2, 0x8b, 0x8c, 0x5, 
    0x16, 0xc, 0x2, 0x8c, 0x8d, 0x5, 0x2a, 0x16, 0x2, 0x8d, 0x8e, 0x5, 0x28, 
    0x15, 0x2, 0x8e, 0x8f, 0x7, 0x2c, 0x2, 0x2, 0x8f, 0x90, 0x8, 0x3, 0x1, 
    0x2, 0x90, 0xae, 0x3, 0x2, 0x2, 0x2, 0x91, 0x92, 0x7, 0x18, 0x2, 0x2, 
    0x92, 0x93, 0x5, 0x26, 0x14, 0x2, 0x93, 0x94, 0x7, 0x14, 0x2, 0x2, 0x94, 
    0x95, 0x5, 0xe, 0x8, 0x2, 0x95, 0x96, 0x5, 0x16, 0xc, 0x2, 0x96, 0x97, 
    0x5, 0x2a, 0x16, 0x2, 0x97, 0x98, 0x5, 0x28, 0x15, 0x2, 0x98, 0x99, 
    0x7, 0x2c, 0x2, 0x2, 0x99, 0x9a, 0x8, 0x3, 0x1, 0x2, 0x9a, 0xae, 0x3, 
    0x2, 0x2, 0x2, 0x9b, 0x9c, 0x7, 0x9, 0x2, 0x2, 0x9c, 0x9d, 0x7, 0x1c, 
    0x2, 0x2, 0x9d, 0x9e, 0x7, 0x28, 0x2, 0x2, 0x9e, 0x9f, 0x7, 0x2d, 0x2, 
    0x2, 0x9f, 0xa0, 0x5, 0xc, 0x7, 0x2, 0xa0, 0xa1, 0x7, 0x2e, 0x2, 0x2, 
    0xa1, 0xa2, 0x7, 0x2c, 0x2, 0x2, 0xa2, 0xa3, 0x8, 0x3, 0x1, 0x2, 0xa3, 
    0xae, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 0x7, 0xa, 0x2, 0x2, 0xa5, 0xa6, 
    0x7, 0x1c, 0x2, 0x2, 0xa6, 0xa7, 0x7, 0x28, 0x2, 0x2, 0xa7, 0xa8, 0x7, 
    0x2d, 0x2, 0x2, 0xa8, 0xa9, 0x5, 0xc, 0x7, 0x2, 0xa9, 0xaa, 0x7, 0x2e, 
    0x2, 0x2, 0xaa, 0xab, 0x7, 0x2c, 0x2, 0x2, 0xab, 0xac, 0x8, 0x3, 0x1, 
    0x2, 0xac, 0xae, 0x3, 0x2, 0x2, 0x2, 0xad, 0x34, 0x3, 0x2, 0x2, 0x2, 
    0xad, 0x38, 0x3, 0x2, 0x2, 0x2, 0xad, 0x3d, 0x3, 0x2, 0x2, 0x2, 0xad, 
    0x42, 0x3, 0x2, 0x2, 0x2, 0xad, 0x46, 0x3, 0x2, 0x2, 0x2, 0xad, 0x4a, 
    0x3, 0x2, 0x2, 0x2, 0xad, 0x53, 0x3, 0x2, 0x2, 0x2, 0xad, 0x58, 0x3, 
    0x2, 0x2, 0x2, 0xad, 0x5c, 0x3, 0x2, 0x2, 0x2, 0xad, 0x64, 0x3, 0x2, 
    0x2, 0x2, 0xad, 0x6b, 0x3, 0x2, 0x2, 0x2, 0xad, 0x73, 0x3, 0x2, 0x2, 
    0x2, 0xad, 0x7d, 0x3, 0x2, 0x2, 0x2, 0xad, 0x87, 0x3, 0x2, 0x2, 0x2, 
    0xad, 0x91, 0x3, 0x2, 0x2, 0x2, 0xad, 0x9b, 0x3, 0x2, 0x2, 0x2, 0xad, 
    0xa4, 0x3, 0x2, 0x2, 0x2, 0xae, 0x5, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xb9, 
    0x3, 0x2, 0x2, 0x2, 0xb0, 0xb5, 0x5, 0x8, 0x5, 0x2, 0xb1, 0xb2, 0x7, 
    0x2f, 0x2, 0x2, 0xb2, 0xb4, 0x5, 0x8, 0x5, 0x2, 0xb3, 0xb1, 0x3, 0x2, 
    0x2, 0x2, 0xb4, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb5, 0xb3, 0x3, 0x2, 0x2, 
    0x2, 0xb5, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb9, 0x3, 0x2, 0x2, 0x2, 
    0xb7, 0xb5, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xb8, 
    0xb0, 0x3, 0x2, 0x2, 0x2, 0xb9, 0x7, 0x3, 0x2, 0x2, 0x2, 0xba, 0xbb, 
    0x7, 0x28, 0x2, 0x2, 0xbb, 0xbc, 0x5, 0xa, 0x6, 0x2, 0xbc, 0xbd, 0x8, 
    0x5, 0x1, 0x2, 0xbd, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xbf, 0x7, 0x28, 
    0x2, 0x2, 0xbf, 0xc0, 0x5, 0xa, 0x6, 0x2, 0xc0, 0xc1, 0x7, 0xe, 0x2, 
    0x2, 0xc1, 0xc2, 0x7, 0xf, 0x2, 0x2, 0xc2, 0xc3, 0x8, 0x5, 0x1, 0x2, 
    0xc3, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xc4, 0xc5, 0x7, 0xc, 0x2, 0x2, 0xc5, 
    0xc6, 0x7, 0xd, 0x2, 0x2, 0xc6, 0xc7, 0x7, 0x2d, 0x2, 0x2, 0xc7, 0xc8, 
    0x5, 0xc, 0x7, 0x2, 0xc8, 0xc9, 0x7, 0x2e, 0x2, 0x2, 0xc9, 0xca, 0x8, 
    0x5, 0x1, 0x2, 0xca, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xcc, 0x7, 0x1e, 
    0x2, 0x2, 0xcc, 0xcd, 0x7, 0xd, 0x2, 0x2, 0xcd, 0xce, 0x7, 0x2d, 0x2, 
    0x2, 0xce, 0xcf, 0x5, 0xc, 0x7, 0x2, 0xcf, 0xd0, 0x7, 0x2e, 0x2, 0x2, 
    0xd0, 0xd1, 0x7, 0x1b, 0x2, 0x2, 0xd1, 0xd2, 0x7, 0x28, 0x2, 0x2, 0xd2, 
    0xd3, 0x7, 0x2d, 0x2, 0x2, 0xd3, 0xd4, 0x5, 0xc, 0x7, 0x2, 0xd4, 0xd5, 
    0x7, 0x2e, 0x2, 0x2, 0xd5, 0xd6, 0x8, 0x5, 0x1, 0x2, 0xd6, 0xe0, 0x3, 
    0x2, 0x2, 0x2, 0xd7, 0xd8, 0x7, 0x1f, 0x2, 0x2, 0xd8, 0xd9, 0x7, 0x2d, 
    0x2, 0x2, 0xd9, 0xda, 0x7, 0x28, 0x2, 0x2, 0xda, 0xdb, 0x7, 0x20, 0x2, 
    0x2, 0xdb, 0xdc, 0x5, 0x12, 0xa, 0x2, 0xdc, 0xdd, 0x7, 0x2e, 0x2, 0x2, 
    0xdd, 0xde, 0x8, 0x5, 0x1, 0x2, 0xde, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xdf, 
    0xba, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xbe, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xc4, 
    0x3, 0x2, 0x2, 0x2, 0xdf, 0xcb, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xd7, 0x3, 
    0x2, 0x2, 0x2, 0xe0, 0x9, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe2, 0x7, 0x28, 
    0x2, 0x2, 0xe2, 0xe3, 0x7, 0x2d, 0x2, 0x2, 0xe3, 0xe4, 0x7, 0x29, 0x2, 
    0x2, 0xe4, 0xe5, 0x7, 0x2e, 0x2, 0x2, 0xe5, 0xe9, 0x8, 0x6, 0x1, 0x2, 
    0xe6, 0xe7, 0x7, 0x28, 0x2, 0x2, 0xe7, 0xe9, 0x8, 0x6, 0x1, 0x2, 0xe8, 
    0xe1, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xb, 
    0x3, 0x2, 0x2, 0x2, 0xea, 0xeb, 0x7, 0x28, 0x2, 0x2, 0xeb, 0xf1, 0x8, 
    0x7, 0x1, 0x2, 0xec, 0xed, 0x7, 0x2f, 0x2, 0x2, 0xed, 0xee, 0x7, 0x28, 
    0x2, 0x2, 0xee, 0xf0, 0x8, 0x7, 0x1, 0x2, 0xef, 0xec, 0x3, 0x2, 0x2, 
    0x2, 0xf0, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xef, 0x3, 0x2, 0x2, 0x2, 
    0xf1, 0xf2, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xd, 0x3, 0x2, 0x2, 0x2, 0xf3, 
    0xf1, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xf5, 0x7, 0x28, 0x2, 0x2, 0xf5, 0xf9, 
    0x8, 0x8, 0x1, 0x2, 0xf6, 0xf7, 0x7, 0x2f, 0x2, 0x2, 0xf7, 0xf8, 0x7, 
    0x28, 0x2, 0x2, 0xf8, 0xfa, 0x8, 0x8, 0x1, 0x2, 0xf9, 0xf6, 0x3, 0x2, 
    0x2, 0x2, 0xfa, 0xfb, 0x3, 0x2, 0x2, 0x2, 0xfb, 0xf9, 0x3, 0x2, 0x2, 
    0x2, 0xfb, 0xfc, 0x3, 0x2, 0x2, 0x2, 0xfc, 0xf, 0x3, 0x2, 0x2, 0x2, 
    0xfd, 0xfe, 0x5, 0x12, 0xa, 0x2, 0xfe, 0x105, 0x8, 0x9, 0x1, 0x2, 0xff, 
    0x100, 0x7, 0x2f, 0x2, 0x2, 0x100, 0x101, 0x5, 0x12, 0xa, 0x2, 0x101, 
    0x102, 0x8, 0x9, 0x1, 0x2, 0x102, 0x104, 0x3, 0x2, 0x2, 0x2, 0x103, 
    0xff, 0x3, 0x2, 0x2, 0x2, 0x104, 0x107, 0x3, 0x2, 0x2, 0x2, 0x105, 0x103, 
    0x3, 0x2, 0x2, 0x2, 0x105, 0x106, 0x3, 0x2, 0x2, 0x2, 0x106, 0x11, 0x3, 
    0x2, 0x2, 0x2, 0x107, 0x105, 0x3, 0x2, 0x2, 0x2, 0x108, 0x118, 0x3, 
    0x2, 0x2, 0x2, 0x109, 0x10a, 0x7, 0x2d, 0x2, 0x2, 0x10a, 0x10b, 0x5, 
    0x14, 0xb, 0x2, 0x10b, 0x112, 0x8, 0xa, 0x1, 0x2, 0x10c, 0x10d, 0x7, 
    0x2f, 0x2, 0x2, 0x10d, 0x10e, 0x5, 0x14, 0xb, 0x2, 0x10e, 0x10f, 0x8, 
    0xa, 0x1, 0x2, 0x10f, 0x111, 0x3, 0x2, 0x2, 0x2, 0x110, 0x10c, 0x3, 
    0x2, 0x2, 0x2, 0x111, 0x114, 0x3, 0x2, 0x2, 0x2, 0x112, 0x110, 0x3, 
    0x2, 0x2, 0x2, 0x112, 0x113, 0x3, 0x2, 0x2, 0x2, 0x113, 0x115, 0x3, 
    0x2, 0x2, 0x2, 0x114, 0x112, 0x3, 0x2, 0x2, 0x2, 0x115, 0x116, 0x7, 
    0x2e, 0x2, 0x2, 0x116, 0x118, 0x3, 0x2, 0x2, 0x2, 0x117, 0x108, 0x3, 
    0x2, 0x2, 0x2, 0x117, 0x109, 0x3, 0x2, 0x2, 0x2, 0x118, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0x119, 0x11a, 0x9, 0x2, 0x2, 0x2, 0x11a, 0x120, 0x8, 0xb, 
    0x1, 0x2, 0x11b, 0x11c, 0x7, 0x2b, 0x2, 0x2, 0x11c, 0x120, 0x8, 0xb, 
    0x1, 0x2, 0x11d, 0x11e, 0x7, 0xf, 0x2, 0x2, 0x11e, 0x120, 0x8, 0xb, 
    0x1, 0x2, 0x11f, 0x119, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x11b, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x120, 0x15, 0x3, 0x2, 0x2, 
    0x2, 0x121, 0x122, 0x7, 0x15, 0x2, 0x2, 0x122, 0x127, 0x5, 0x18, 0xd, 
    0x2, 0x123, 0x124, 0x7, 0x1d, 0x2, 0x2, 0x124, 0x126, 0x5, 0x18, 0xd, 
    0x2, 0x125, 0x123, 0x3, 0x2, 0x2, 0x2, 0x126, 0x129, 0x3, 0x2, 0x2, 
    0x2, 0x127, 0x125, 0x3, 0x2, 0x2, 0x2, 0x127, 0x128, 0x3, 0x2, 0x2, 
    0x2, 0x128, 0x12c, 0x3, 0x2, 0x2, 0x2, 0x129, 0x127, 0x3, 0x2, 0x2, 
    0x2, 0x12a, 0x12c, 0x8, 0xc, 0x1, 0x2, 0x12b, 0x121, 0x3, 0x2, 0x2, 
    0x2, 0x12b, 0x12a, 0x3, 0x2, 0x2, 0x2, 0x12c, 0x17, 0x3, 0x2, 0x2, 0x2, 
    0x12d, 0x12e, 0x5, 0x1a, 0xe, 0x2, 0x12e, 0x12f, 0x5, 0x1e, 0x10, 0x2, 
    0x12f, 0x130, 0x5, 0x14, 0xb, 0x2, 0x130, 0x131, 0x8, 0xd, 0x1, 0x2, 
    0x131, 0x143, 0x3, 0x2, 0x2, 0x2, 0x132, 0x133, 0x5, 0x1a, 0xe, 0x2, 
    0x133, 0x134, 0x5, 0x1e, 0x10, 0x2, 0x134, 0x135, 0x5, 0x1a, 0xe, 0x2, 
    0x135, 0x136, 0x8, 0xd, 0x1, 0x2, 0x136, 0x143, 0x3, 0x2, 0x2, 0x2, 
    0x137, 0x138, 0x5, 0x1a, 0xe, 0x2, 0x138, 0x139, 0x7, 0x19, 0x2, 0x2, 
    0x139, 0x13a, 0x7, 0xf, 0x2, 0x2, 0x13a, 0x13b, 0x8, 0xd, 0x1, 0x2, 
    0x13b, 0x143, 0x3, 0x2, 0x2, 0x2, 0x13c, 0x13d, 0x5, 0x1a, 0xe, 0x2, 
    0x13d, 0x13e, 0x7, 0x19, 0x2, 0x2, 0x13e, 0x13f, 0x7, 0xe, 0x2, 0x2, 
    0x13f, 0x140, 0x7, 0xf, 0x2, 0x2, 0x140, 0x141, 0x8, 0xd, 0x1, 0x2, 
    0x141, 0x143, 0x3, 0x2, 0x2, 0x2, 0x142, 0x12d, 0x3, 0x2, 0x2, 0x2, 
    0x142, 0x132, 0x3, 0x2, 0x2, 0x2, 0x142, 0x137, 0x3, 0x2, 0x2, 0x2, 
    0x142, 0x13c, 0x3, 0x2, 0x2, 0x2, 0x143, 0x19, 0x3, 0x2, 0x2, 0x2, 0x144, 
    0x145, 0x7, 0x28, 0x2, 0x2, 0x145, 0x146, 0x8, 0xe, 0x1, 0x2, 0x146, 
    0x148, 0x7, 0x30, 0x2, 0x2, 0x147, 0x144, 0x3, 0x2, 0x2, 0x2, 0x147, 
    0x148, 0x3, 0x2, 0x2, 0x2, 0x148, 0x149, 0x3, 0x2, 0x2, 0x2, 0x149, 
    0x14a, 0x7, 0x28, 0x2, 0x2, 0x14a, 0x14b, 0x8, 0xe, 0x1, 0x2, 0x14b, 
    0x14c, 0x8, 0xe, 0x1, 0x2, 0x14c, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x14e, 
    0x5, 0x1a, 0xe, 0x2, 0x14e, 0x14f, 0x8, 0xf, 0x1, 0x2, 0x14f, 0x169, 
    0x3, 0x2, 0x2, 0x2, 0x150, 0x151, 0x7, 0x24, 0x2, 0x2, 0x151, 0x152, 
    0x7, 0x2d, 0x2, 0x2, 0x152, 0x153, 0x5, 0x1a, 0xe, 0x2, 0x153, 0x154, 
    0x7, 0x2e, 0x2, 0x2, 0x154, 0x155, 0x8, 0xf, 0x1, 0x2, 0x155, 0x169, 
    0x3, 0x2, 0x2, 0x2, 0x156, 0x157, 0x7, 0x25, 0x2, 0x2, 0x157, 0x158, 
    0x7, 0x2d, 0x2, 0x2, 0x158, 0x159, 0x5, 0x1a, 0xe, 0x2, 0x159, 0x15a, 
    0x7, 0x2e, 0x2, 0x2, 0x15a, 0x15b, 0x8, 0xf, 0x1, 0x2, 0x15b, 0x169, 
    0x3, 0x2, 0x2, 0x2, 0x15c, 0x15d, 0x7, 0x27, 0x2, 0x2, 0x15d, 0x15e, 
    0x7, 0x2d, 0x2, 0x2, 0x15e, 0x15f, 0x5, 0x1a, 0xe, 0x2, 0x15f, 0x160, 
    0x7, 0x2e, 0x2, 0x2, 0x160, 0x161, 0x8, 0xf, 0x1, 0x2, 0x161, 0x169, 
    0x3, 0x2, 0x2, 0x2, 0x162, 0x163, 0x7, 0x26, 0x2, 0x2, 0x163, 0x164, 
    0x7, 0x2d, 0x2, 0x2, 0x164, 0x165, 0x5, 0x1a, 0xe, 0x2, 0x165, 0x166, 
    0x7, 0x2e, 0x2, 0x2, 0x166, 0x167, 0x8, 0xf, 0x1, 0x2, 0x167, 0x169, 
    0x3, 0x2, 0x2, 0x2, 0x168, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x168, 0x150, 
    0x3, 0x2, 0x2, 0x2, 0x168, 0x156, 0x3, 0x2, 0x2, 0x2, 0x168, 0x15c, 
    0x3, 0x2, 0x2, 0x2, 0x168, 0x162, 0x3, 0x2, 0x2, 0x2, 0x169, 0x1d, 0x3, 
    0x2, 0x2, 0x2, 0x16a, 0x16b, 0x7, 0x32, 0x2, 0x2, 0x16b, 0x177, 0x8, 
    0x10, 0x1, 0x2, 0x16c, 0x16d, 0x7, 0x33, 0x2, 0x2, 0x16d, 0x177, 0x8, 
    0x10, 0x1, 0x2, 0x16e, 0x16f, 0x7, 0x34, 0x2, 0x2, 0x16f, 0x177, 0x8, 
    0x10, 0x1, 0x2, 0x170, 0x171, 0x7, 0x35, 0x2, 0x2, 0x171, 0x177, 0x8, 
    0x10, 0x1, 0x2, 0x172, 0x173, 0x7, 0x36, 0x2, 0x2, 0x173, 0x177, 0x8, 
    0x10, 0x1, 0x2, 0x174, 0x175, 0x7, 0x37, 0x2, 0x2, 0x175, 0x177, 0x8, 
    0x10, 0x1, 0x2, 0x176, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x176, 0x16c, 0x3, 
    0x2, 0x2, 0x2, 0x176, 0x16e, 0x3, 0x2, 0x2, 0x2, 0x176, 0x170, 0x3, 
    0x2, 0x2, 0x2, 0x176, 0x172, 0x3, 0x2, 0x2, 0x2, 0x176, 0x174, 0x3, 
    0x2, 0x2, 0x2, 0x177, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x178, 0x179, 0x5, 0x22, 
    0x12, 0x2, 0x179, 0x180, 0x8, 0x11, 0x1, 0x2, 0x17a, 0x17b, 0x7, 0x2f, 
    0x2, 0x2, 0x17b, 0x17c, 0x5, 0x22, 0x12, 0x2, 0x17c, 0x17d, 0x8, 0x11, 
    0x1, 0x2, 0x17d, 0x17f, 0x3, 0x2, 0x2, 0x2, 0x17e, 0x17a, 0x3, 0x2, 
    0x2, 0x2, 0x17f, 0x182, 0x3, 0x2, 0x2, 0x2, 0x180, 0x17e, 0x3, 0x2, 
    0x2, 0x2, 0x180, 0x181, 0x3, 0x2, 0x2, 0x2, 0x181, 0x21, 0x3, 0x2, 0x2, 
    0x2, 0x182, 0x180, 0x3, 0x2, 0x2, 0x2, 0x183, 0x184, 0x7, 0x28, 0x2, 
    0x2, 0x184, 0x185, 0x7, 0x36, 0x2, 0x2, 0x185, 0x186, 0x5, 0x14, 0xb, 
    0x2, 0x186, 0x187, 0x8, 0x12, 0x1, 0x2, 0x187, 0x23, 0x3, 0x2, 0x2, 
    0x2, 0x188, 0x189, 0x5, 0x1a, 0xe, 0x2, 0x189, 0x190, 0x8, 0x13, 0x1, 
    0x2, 0x18a, 0x18b, 0x7, 0x2f, 0x2, 0x2, 0x18b, 0x18c, 0x5, 0x1a, 0xe, 
    0x2, 0x18c, 0x18d, 0x8, 0x13, 0x1, 0x2, 0x18d, 0x18f, 0x3, 0x2, 0x2, 
    0x2, 0x18e, 0x18a, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x192, 0x3, 0x2, 0x2, 
    0x2, 0x190, 0x18e, 0x3, 0x2, 0x2, 0x2, 0x190, 0x191, 0x3, 0x2, 0x2, 
    0x2, 0x191, 0x25, 0x3, 0x2, 0x2, 0x2, 0x192, 0x190, 0x3, 0x2, 0x2, 0x2, 
    0x193, 0x194, 0x5, 0x1c, 0xf, 0x2, 0x194, 0x19b, 0x8, 0x14, 0x1, 0x2, 
    0x195, 0x196, 0x7, 0x2f, 0x2, 0x2, 0x196, 0x197, 0x5, 0x1c, 0xf, 0x2, 
    0x197, 0x198, 0x8, 0x14, 0x1, 0x2, 0x198, 0x19a, 0x3, 0x2, 0x2, 0x2, 
    0x199, 0x195, 0x3, 0x2, 0x2, 0x2, 0x19a, 0x19d, 0x3, 0x2, 0x2, 0x2, 
    0x19b, 0x199, 0x3, 0x2, 0x2, 0x2, 0x19b, 0x19c, 0x3, 0x2, 0x2, 0x2, 
    0x19c, 0x27, 0x3, 0x2, 0x2, 0x2, 0x19d, 0x19b, 0x3, 0x2, 0x2, 0x2, 0x19e, 
    0x1a5, 0x8, 0x15, 0x1, 0x2, 0x19f, 0x1a0, 0x7, 0x21, 0x2, 0x2, 0x1a0, 
    0x1a1, 0x7, 0x23, 0x2, 0x2, 0x1a1, 0x1a2, 0x5, 0x24, 0x13, 0x2, 0x1a2, 
    0x1a3, 0x8, 0x15, 0x1, 0x2, 0x1a3, 0x1a5, 0x3, 0x2, 0x2, 0x2, 0x1a4, 
    0x19e, 0x3, 0x2, 0x2, 0x2, 0x1a4, 0x19f, 0x3, 0x2, 0x2, 0x2, 0x1a5, 
    0x29, 0x3, 0x2, 0x2, 0x2, 0x1a6, 0x1ad, 0x8, 0x16, 0x1, 0x2, 0x1a7, 
    0x1a8, 0x7, 0x22, 0x2, 0x2, 0x1a8, 0x1a9, 0x7, 0x23, 0x2, 0x2, 0x1a9, 
    0x1aa, 0x5, 0x24, 0x13, 0x2, 0x1aa, 0x1ab, 0x8, 0x16, 0x1, 0x2, 0x1ab, 
    0x1ad, 0x3, 0x2, 0x2, 0x2, 0x1ac, 0x1a6, 0x3, 0x2, 0x2, 0x2, 0x1ac, 
    0x1a7, 0x3, 0x2, 0x2, 0x2, 0x1ad, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x19, 0x2f, 
    0xad, 0xb5, 0xb8, 0xdf, 0xe8, 0xf1, 0xfb, 0x105, 0x112, 0x117, 0x11f, 
    0x127, 0x12b, 0x142, 0x147, 0x168, 0x176, 0x180, 0x190, 0x19b, 0x1a4, 
    0x1ac, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

SqlParser::Initializer SqlParser::_init;
