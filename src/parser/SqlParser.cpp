
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
    setState(37);
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
      setState(34);
      stmt();
      setState(39);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(40);
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

tree::TerminalNode* SqlParser::StmtContext::WHERE() {
  return getToken(SqlParser::WHERE, 0);
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

SqlParser::TableListContext* SqlParser::StmtContext::tableList() {
  return getRuleContext<SqlParser::TableListContext>(0);
}

SqlParser::SelectorContext* SqlParser::StmtContext::selector() {
  return getRuleContext<SqlParser::SelectorContext>(0);
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
    setState(159);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(42);
      match(SqlParser::SHOW);
      setState(43);
      match(SqlParser::DATABASES);
      setState(44);
      match(SqlParser::SEMICOLON);
       showDbs(); 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(46);
      match(SqlParser::CREATE);
      setState(47);
      match(SqlParser::DATABASE);
      setState(48);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(49);
      match(SqlParser::SEMICOLON);
       createDb((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(51);
      match(SqlParser::DROP);
      setState(52);
      match(SqlParser::DATABASE);
      setState(53);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(54);
      match(SqlParser::SEMICOLON);
       dropDb((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(56);
      match(SqlParser::USE);
      setState(57);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(58);
      match(SqlParser::SEMICOLON);
       use((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(60);
      match(SqlParser::SHOW);
      setState(61);
      match(SqlParser::TABLES);
      setState(62);
      match(SqlParser::SEMICOLON);
       showTables(); 
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(64);
      match(SqlParser::CREATE);
      setState(65);
      match(SqlParser::TABLE);
      setState(66);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(67);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(68);
      dynamic_cast<StmtContext *>(_localctx)->fieldListContext = fieldList();
      setState(69);
      match(SqlParser::RIGHT_PARENTHESIS);
      setState(70);
      match(SqlParser::SEMICOLON);
       createTable((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->fieldListContext->cols, dynamic_cast<StmtContext *>(_localctx)->fieldListContext->priIdx, dynamic_cast<StmtContext *>(_localctx)->fieldListContext->fors); 
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(73);
      match(SqlParser::DROP);
      setState(74);
      match(SqlParser::TABLE);
      setState(75);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
       dropTable((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(77);
      match(SqlParser::DESC);
      setState(78);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
       desc((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")); 
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(80);
      match(SqlParser::INSERT);
      setState(81);
      match(SqlParser::INTO);
      setState(82);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(83);
      match(SqlParser::VALUES);
      setState(84);
      dynamic_cast<StmtContext *>(_localctx)->valueListsContext = valueLists();
      setState(85);
      match(SqlParser::SEMICOLON);
       insert((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->valueListsContext->result); 
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(88);
      match(SqlParser::DELETE);
      setState(89);
      match(SqlParser::FROM);
      setState(90);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(91);
      match(SqlParser::WHERE);
      setState(92);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""));
      setState(93);
      match(SqlParser::SEMICOLON);
       remove((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm); 
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(96);
      match(SqlParser::UPDATE);
      setState(97);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(98);
      match(SqlParser::SET);
      setState(99);
      dynamic_cast<StmtContext *>(_localctx)->setClausesContext = setClauses();
      setState(100);
      match(SqlParser::WHERE);
      setState(101);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""));
      setState(102);
      match(SqlParser::SEMICOLON);
       update((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->setClausesContext->result, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm); 
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(105);
      match(SqlParser::SELECT);
      setState(106);
      match(SqlParser::STAR);
      setState(107);
      match(SqlParser::FROM);
      setState(108);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(109);
      match(SqlParser::WHERE);
      setState(110);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""));
      setState(111);
      match(SqlParser::SEMICOLON);
       select(None(), {(dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")}, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm); 
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(114);
      match(SqlParser::SELECT);
      setState(115);
      match(SqlParser::STAR);
      setState(116);
      match(SqlParser::FROM);
      setState(117);
      dynamic_cast<StmtContext *>(_localctx)->tableListContext = tableList();
      setState(118);
      match(SqlParser::WHERE);
      setState(119);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses();
      setState(120);
      match(SqlParser::SEMICOLON);
       select(None(), dynamic_cast<StmtContext *>(_localctx)->tableListContext->result, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm); 
      break;
    }

    case 14: {
      enterOuterAlt(_localctx, 14);
      setState(123);
      match(SqlParser::SELECT);
      setState(124);
      dynamic_cast<StmtContext *>(_localctx)->selectorContext = selector();
      setState(125);
      match(SqlParser::FROM);
      setState(126);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(127);
      match(SqlParser::WHERE);
      setState(128);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""));
      setState(129);
      match(SqlParser::SEMICOLON);
       select(dynamic_cast<StmtContext *>(_localctx)->selectorContext->result, {(dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : "")}, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm); 
      break;
    }

    case 15: {
      enterOuterAlt(_localctx, 15);
      setState(132);
      match(SqlParser::SELECT);
      setState(133);
      dynamic_cast<StmtContext *>(_localctx)->selectorContext = selector();
      setState(134);
      match(SqlParser::FROM);
      setState(135);
      dynamic_cast<StmtContext *>(_localctx)->tableListContext = tableList();
      setState(136);
      match(SqlParser::WHERE);
      setState(137);
      dynamic_cast<StmtContext *>(_localctx)->whereClausesContext = whereClauses();
      setState(138);
      match(SqlParser::SEMICOLON);
       select(dynamic_cast<StmtContext *>(_localctx)->selectorContext->result, dynamic_cast<StmtContext *>(_localctx)->tableListContext->result, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->icm, dynamic_cast<StmtContext *>(_localctx)->whereClausesContext->ocm); 
      break;
    }

    case 16: {
      enterOuterAlt(_localctx, 16);
      setState(141);
      match(SqlParser::CREATE);
      setState(142);
      match(SqlParser::INDEX);
      setState(143);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(144);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(145);
      dynamic_cast<StmtContext *>(_localctx)->columnListContext = columnList();
      setState(146);
      match(SqlParser::RIGHT_PARENTHESIS);
      setState(147);
      match(SqlParser::SEMICOLON);
       createIndex((dynamic_cast<StmtContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<StmtContext *>(_localctx)->identifierToken->getText() : ""), dynamic_cast<StmtContext *>(_localctx)->columnListContext->result); 
      break;
    }

    case 17: {
      enterOuterAlt(_localctx, 17);
      setState(150);
      match(SqlParser::DROP);
      setState(151);
      match(SqlParser::INDEX);
      setState(152);
      dynamic_cast<StmtContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(153);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(154);
      dynamic_cast<StmtContext *>(_localctx)->columnListContext = columnList();
      setState(155);
      match(SqlParser::RIGHT_PARENTHESIS);
      setState(156);
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
    setState(170);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::RIGHT_PARENTHESIS: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case SqlParser::PRIMARY:
      case SqlParser::FOREIGN:
      case SqlParser::Identifier: {
        enterOuterAlt(_localctx, 2);
        setState(162);
        field(&_localctx->cols, &_localctx->priIdx, &_localctx->fors);
        setState(167);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SqlParser::COMMA) {
          setState(163);
          match(SqlParser::COMMA);
          setState(164);
          field(&_localctx->cols, &_localctx->priIdx, &_localctx->fors);
          setState(169);
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

SqlParser::FieldContext::FieldContext(ParserRuleContext *parent, size_t invokingState, Cols * cols, PriIdx * priIdx, Fors * fors)
  : ParserRuleContext(parent, invokingState) {
  this->cols = cols;
  this->priIdx = priIdx;
  this->fors = fors;
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


size_t SqlParser::FieldContext::getRuleIndex() const {
  return SqlParser::RuleField;
}


SqlParser::FieldContext* SqlParser::field(Cols * cols,PriIdx * priIdx,Fors * fors) {
  FieldContext *_localctx = _tracker.createInstance<FieldContext>(_ctx, getState(), cols, priIdx, fors);
  enterRule(_localctx, 6, SqlParser::RuleField);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(201);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(172);
      dynamic_cast<FieldContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(173);
      dynamic_cast<FieldContext *>(_localctx)->typeContext = type();
       cols->push_back(std::make_pair((dynamic_cast<FieldContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<FieldContext *>(_localctx)->identifierToken->getText() : ""), (Column){dynamic_cast<FieldContext *>(_localctx)->typeContext->typeID, dynamic_cast<FieldContext *>(_localctx)->typeContext->length, false})); 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(176);
      dynamic_cast<FieldContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(177);
      dynamic_cast<FieldContext *>(_localctx)->typeContext = type();
      setState(178);
      match(SqlParser::NOT);
      setState(179);
      match(SqlParser::NULL_TOKEN);
       cols->push_back(std::make_pair((dynamic_cast<FieldContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<FieldContext *>(_localctx)->identifierToken->getText() : ""), (Column){dynamic_cast<FieldContext *>(_localctx)->typeContext->typeID, dynamic_cast<FieldContext *>(_localctx)->typeContext->length, true})); 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(182);
      match(SqlParser::PRIMARY);
      setState(183);
      match(SqlParser::KEY);
      setState(184);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(185);
      dynamic_cast<FieldContext *>(_localctx)->columnListContext = columnList();
      setState(186);
      match(SqlParser::RIGHT_PARENTHESIS);

                  if (priIdx->isOk()) throw MultiplePrimaryException();
                  *priIdx = dynamic_cast<FieldContext *>(_localctx)->columnListContext->result;
                
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(189);
      match(SqlParser::FOREIGN);
      setState(190);
      match(SqlParser::KEY);
      setState(191);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(192);
      dynamic_cast<FieldContext *>(_localctx)->referrer = columnList();
      setState(193);
      match(SqlParser::RIGHT_PARENTHESIS);
      setState(194);
      match(SqlParser::REFERENCES);
      setState(195);
      dynamic_cast<FieldContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      setState(196);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(197);
      dynamic_cast<FieldContext *>(_localctx)->referee = columnList();
      setState(198);
      match(SqlParser::RIGHT_PARENTHESIS);
       fors->push_back((TableMgr::ForeignKey){(dynamic_cast<FieldContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<FieldContext *>(_localctx)->identifierToken->getText() : ""), std::move(dynamic_cast<FieldContext *>(_localctx)->referrer->result), std::move(dynamic_cast<FieldContext *>(_localctx)->referee->result)}); 
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

tree::TerminalNode* SqlParser::TypeContext::INT() {
  return getToken(SqlParser::INT, 0);
}

tree::TerminalNode* SqlParser::TypeContext::Int() {
  return getToken(SqlParser::Int, 0);
}

tree::TerminalNode* SqlParser::TypeContext::CHAR() {
  return getToken(SqlParser::CHAR, 0);
}

tree::TerminalNode* SqlParser::TypeContext::VARCHAR() {
  return getToken(SqlParser::VARCHAR, 0);
}

tree::TerminalNode* SqlParser::TypeContext::DATE() {
  return getToken(SqlParser::DATE, 0);
}

tree::TerminalNode* SqlParser::TypeContext::FLOAT() {
  return getToken(SqlParser::FLOAT, 0);
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
    setState(224);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(203);
      match(SqlParser::INT);
       dynamic_cast<TypeContext *>(_localctx)->typeID =  Type::INT; 
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(205);
      match(SqlParser::INT);
      setState(206);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(207);
      dynamic_cast<TypeContext *>(_localctx)->intToken = match(SqlParser::Int);
      setState(208);
      match(SqlParser::RIGHT_PARENTHESIS);
       dynamic_cast<TypeContext *>(_localctx)->typeID =  Type::INT, _localctx->length = std::stoi((dynamic_cast<TypeContext *>(_localctx)->intToken != nullptr ? dynamic_cast<TypeContext *>(_localctx)->intToken->getText() : "")); 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(210);
      match(SqlParser::CHAR);
      setState(211);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(212);
      dynamic_cast<TypeContext *>(_localctx)->intToken = match(SqlParser::Int);
      setState(213);
      match(SqlParser::RIGHT_PARENTHESIS);
       dynamic_cast<TypeContext *>(_localctx)->typeID =  Type::CHAR, _localctx->length = std::stoi((dynamic_cast<TypeContext *>(_localctx)->intToken != nullptr ? dynamic_cast<TypeContext *>(_localctx)->intToken->getText() : "")); 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(215);
      match(SqlParser::VARCHAR);
      setState(216);
      match(SqlParser::LEFT_PARENTHESIS);
      setState(217);
      dynamic_cast<TypeContext *>(_localctx)->intToken = match(SqlParser::Int);
      setState(218);
      match(SqlParser::RIGHT_PARENTHESIS);
       dynamic_cast<TypeContext *>(_localctx)->typeID =  Type::VARCHAR, _localctx->length = std::stoi((dynamic_cast<TypeContext *>(_localctx)->intToken != nullptr ? dynamic_cast<TypeContext *>(_localctx)->intToken->getText() : "")); 
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(220);
      match(SqlParser::DATE);
       dynamic_cast<TypeContext *>(_localctx)->typeID =  Type::DATE; 
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(222);
      match(SqlParser::FLOAT);
       dynamic_cast<TypeContext *>(_localctx)->typeID =  Type::FLOAT; 
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
    setState(226);
    dynamic_cast<ColumnListContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
    _localctx->result.push_back((dynamic_cast<ColumnListContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<ColumnListContext *>(_localctx)->identifierToken->getText() : ""));
    setState(233);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(228);
      match(SqlParser::COMMA);
      setState(229);
      dynamic_cast<ColumnListContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      _localctx->result.push_back((dynamic_cast<ColumnListContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<ColumnListContext *>(_localctx)->identifierToken->getText() : ""));
      setState(235);
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
    setState(236);
    dynamic_cast<TableListContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
    _localctx->result.push_back((dynamic_cast<TableListContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<TableListContext *>(_localctx)->identifierToken->getText() : ""));
    setState(241); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(238);
      match(SqlParser::COMMA);
      setState(239);
      dynamic_cast<TableListContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      _localctx->result.push_back((dynamic_cast<TableListContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<TableListContext *>(_localctx)->identifierToken->getText() : ""));
      setState(243); 
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
    setState(245);
    dynamic_cast<ValueListsContext *>(_localctx)->valueListContext = valueList();
    append(_localctx->result, dynamic_cast<ValueListsContext *>(_localctx)->valueListContext->result);
    setState(253);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(247);
      match(SqlParser::COMMA);
      setState(248);
      dynamic_cast<ValueListsContext *>(_localctx)->valueListContext = valueList();
      append(_localctx->result, dynamic_cast<ValueListsContext *>(_localctx)->valueListContext->result);
      setState(255);
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
    setState(271);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::SEMICOLON:
      case SqlParser::COMMA: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case SqlParser::LEFT_PARENTHESIS: {
        enterOuterAlt(_localctx, 2);
        setState(257);
        match(SqlParser::LEFT_PARENTHESIS);
        setState(258);
        dynamic_cast<ValueListContext *>(_localctx)->valueContext = value();
        append(_localctx->result, dynamic_cast<ValueListContext *>(_localctx)->valueContext->result);
        setState(266);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SqlParser::COMMA) {
          setState(260);
          match(SqlParser::COMMA);
          setState(261);
          dynamic_cast<ValueListContext *>(_localctx)->valueContext = value();
          append(_localctx->result, dynamic_cast<ValueListContext *>(_localctx)->valueContext->result);
          setState(268);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(269);
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

tree::TerminalNode* SqlParser::ValueContext::Int() {
  return getToken(SqlParser::Int, 0);
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

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(279);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::Int: {
        enterOuterAlt(_localctx, 1);
        setState(273);
        dynamic_cast<ValueContext *>(_localctx)->intToken = match(SqlParser::Int);
         dynamic_cast<ValueContext *>(_localctx)->result =  (dynamic_cast<ValueContext *>(_localctx)->intToken != nullptr ? dynamic_cast<ValueContext *>(_localctx)->intToken->getText() : ""); 
        break;
      }

      case SqlParser::String: {
        enterOuterAlt(_localctx, 2);
        setState(275);
        dynamic_cast<ValueContext *>(_localctx)->stringToken = match(SqlParser::String);
         dynamic_cast<ValueContext *>(_localctx)->result =  (dynamic_cast<ValueContext *>(_localctx)->stringToken != nullptr ? dynamic_cast<ValueContext *>(_localctx)->stringToken->getText() : "").substr(1, (dynamic_cast<ValueContext *>(_localctx)->stringToken != nullptr ? dynamic_cast<ValueContext *>(_localctx)->stringToken->getText() : "").length() - 2); 
        break;
      }

      case SqlParser::NULL_TOKEN: {
        enterOuterAlt(_localctx, 3);
        setState(277);
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
    enterOuterAlt(_localctx, 1);
    setState(281);
    whereClause(defaultTb, &_localctx->icm, &_localctx->ocm);
    setState(286);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::AND) {
      setState(282);
      match(SqlParser::AND);
      setState(283);
      whereClause(defaultTb, &_localctx->icm, &_localctx->ocm);
      setState(288);
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
    setState(310);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(289);
      dynamic_cast<WhereClauseContext *>(_localctx)->colContext = col(defaultTb);
      setState(290);
      dynamic_cast<WhereClauseContext *>(_localctx)->opContext = op();
      setState(291);
      dynamic_cast<WhereClauseContext *>(_localctx)->valueContext = value();

                  if (dynamic_cast<WhereClauseContext *>(_localctx)->valueContext->result.isOk())
                      (*_localctx->icm)[dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb][dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c].push_back((Table::ConLiteral){dynamic_cast<WhereClauseContext *>(_localctx)->opContext->dir, dynamic_cast<WhereClauseContext *>(_localctx)->valueContext->result.ok()});
                  else // Trick when the condition is always false
                     (*_localctx->ocm)[std::make_pair(dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb, dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb)].push_back((Table::OuterCon){Table::NE, dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c, dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c});
                
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(294);
      dynamic_cast<WhereClauseContext *>(_localctx)->col1 = col(defaultTb);
      setState(295);
      dynamic_cast<WhereClauseContext *>(_localctx)->opContext = op();
      setState(296);
      dynamic_cast<WhereClauseContext *>(_localctx)->col2 = col(defaultTb);
       (*_localctx->ocm)[std::make_pair(dynamic_cast<WhereClauseContext *>(_localctx)->col1->tb, dynamic_cast<WhereClauseContext *>(_localctx)->col2->tb)].push_back((Table::OuterCon){dynamic_cast<WhereClauseContext *>(_localctx)->opContext->dir, dynamic_cast<WhereClauseContext *>(_localctx)->col1->c, dynamic_cast<WhereClauseContext *>(_localctx)->col2->c}); 
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(299);
      dynamic_cast<WhereClauseContext *>(_localctx)->colContext = col(defaultTb);
      setState(300);
      match(SqlParser::IS);
      setState(301);
      match(SqlParser::NULL_TOKEN);
       (*_localctx->icm)[dynamic_cast<WhereClauseContext *>(_localctx)->colContext->tb][dynamic_cast<WhereClauseContext *>(_localctx)->colContext->c].push_back((Table::ConLiteral){Table::IS_NULL, ""}); 
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(304);
      dynamic_cast<WhereClauseContext *>(_localctx)->colContext = col(defaultTb);
      setState(305);
      match(SqlParser::IS);
      setState(306);
      match(SqlParser::NOT);
      setState(307);
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
    setState(315);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      setState(312);
      dynamic_cast<ColContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
      dynamic_cast<ColContext *>(_localctx)->tb =  (dynamic_cast<ColContext *>(_localctx)->identifierToken != nullptr ? dynamic_cast<ColContext *>(_localctx)->identifierToken->getText() : "");
      setState(314);
      match(SqlParser::DOT);
      break;
    }

    }
    setState(317);
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

//----------------- OpContext ------------------------------------------------------------------

SqlParser::OpContext::OpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SqlParser::OpContext::getRuleIndex() const {
  return SqlParser::RuleOp;
}


SqlParser::OpContext* SqlParser::op() {
  OpContext *_localctx = _tracker.createInstance<OpContext>(_ctx, getState());
  enterRule(_localctx, 26, SqlParser::RuleOp);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(333);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SqlParser::LESS_THAN: {
        enterOuterAlt(_localctx, 1);
        setState(321);
        match(SqlParser::LESS_THAN);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::LT; 
        break;
      }

      case SqlParser::LESS_EQUAL: {
        enterOuterAlt(_localctx, 2);
        setState(323);
        match(SqlParser::LESS_EQUAL);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::LE; 
        break;
      }

      case SqlParser::GREATER_THAN: {
        enterOuterAlt(_localctx, 3);
        setState(325);
        match(SqlParser::GREATER_THAN);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::GT; 
        break;
      }

      case SqlParser::GREATER_EQUAL: {
        enterOuterAlt(_localctx, 4);
        setState(327);
        match(SqlParser::GREATER_EQUAL);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::GE; 
        break;
      }

      case SqlParser::EQUAL: {
        enterOuterAlt(_localctx, 5);
        setState(329);
        match(SqlParser::EQUAL);
         dynamic_cast<OpContext *>(_localctx)->dir =  Table::EQ; 
        break;
      }

      case SqlParser::NOT_EQUAL: {
        enterOuterAlt(_localctx, 6);
        setState(331);
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
  enterRule(_localctx, 28, SqlParser::RuleSetClauses);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(335);
    dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext = setClause();
    _localctx->result[dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext->k] = dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext->v;
    setState(343);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(337);
      match(SqlParser::COMMA);
      setState(338);
      dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext = setClause();
      _localctx->result[dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext->k] = dynamic_cast<SetClausesContext *>(_localctx)->setClauseContext->v;
      setState(345);
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
  enterRule(_localctx, 30, SqlParser::RuleSetClause);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(346);
    dynamic_cast<SetClauseContext *>(_localctx)->identifierToken = match(SqlParser::Identifier);
    setState(347);
    match(SqlParser::EQUAL);
    setState(348);
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
  enterRule(_localctx, 32, SqlParser::RuleSelector);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(351);
    dynamic_cast<SelectorContext *>(_localctx)->colContext = col("");
    _localctx->result[dynamic_cast<SelectorContext *>(_localctx)->colContext->tb].push_back(dynamic_cast<SelectorContext *>(_localctx)->colContext->c);
    setState(359);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SqlParser::COMMA) {
      setState(353);
      match(SqlParser::COMMA);
      setState(354);
      dynamic_cast<SelectorContext *>(_localctx)->colContext = col("");
      _localctx->result[dynamic_cast<SelectorContext *>(_localctx)->colContext->tb].push_back(dynamic_cast<SelectorContext *>(_localctx)->colContext->c);
      setState(361);
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

// Static vars and initialization.
std::vector<dfa::DFA> SqlParser::_decisionToDFA;
atn::PredictionContextCache SqlParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SqlParser::_atn;
std::vector<uint16_t> SqlParser::_serializedATN;

std::vector<std::string> SqlParser::_ruleNames = {
  "program", "stmt", "fieldList", "field", "type", "columnList", "tableList", 
  "valueLists", "valueList", "value", "whereClauses", "whereClause", "col", 
  "op", "setClauses", "setClause", "selector"
};

std::vector<std::string> SqlParser::_literalNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "';'", "'('", "')'", "','", "'.'", "'*'", "'<'", "'<='", "'>'", "'>='", 
  "'='", "'<>'"
};

std::vector<std::string> SqlParser::_symbolicNames = {
  "", "WhiteSpaces", "DATABASE", "DATABASES", "TABLE", "TABLES", "SHOW", 
  "CREATE", "DROP", "USE", "PRIMARY", "KEY", "NOT", "NULL_TOKEN", "INSERT", 
  "INTO", "VALUES", "DELETE", "FROM", "WHERE", "UPDATE", "SET", "SELECT", 
  "IS", "INT", "CHAR", "VARCHAR", "DESC", "REFERENCES", "INDEX", "AND", 
  "DATE", "FLOAT", "FOREIGN", "Identifier", "Int", "String", "SEMICOLON", 
  "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "COMMA", "DOT", "STAR", "LESS_THAN", 
  "LESS_EQUAL", "GREATER_THAN", "GREATER_EQUAL", "EQUAL", "NOT_EQUAL"
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
    0x3, 0x32, 0x16d, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x3, 0x2, 0x7, 0x2, 0x26, 0xa, 0x2, 0xc, 0x2, 0xe, 
    0x2, 0x29, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
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
    0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0xa2, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x7, 0x4, 0xa8, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0xab, 0xb, 
    0x4, 0x5, 0x4, 0xad, 0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0xcc, 0xa, 0x5, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 
    0x6, 0xe3, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x7, 0x7, 0xea, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0xed, 0xb, 0x7, 0x3, 0x8, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x6, 0x8, 0xf4, 0xa, 0x8, 0xd, 
    0x8, 0xe, 0x8, 0xf5, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x7, 0x9, 0xfe, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x101, 0xb, 
    0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xa, 0x7, 0xa, 0x10b, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 0x10e, 
    0xb, 0xa, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0x112, 0xa, 0xa, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x11a, 0xa, 0xb, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0x11f, 0xa, 0xc, 0xc, 0xc, 0xe, 
    0xc, 0x122, 0xb, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x139, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x5, 0xe, 0x13e, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x150, 0xa, 
    0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x7, 0x10, 0x158, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0x15b, 0xb, 0x10, 
    0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 
    0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0x168, 
    0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 0x16b, 0xb, 0x12, 0x3, 0x12, 0x2, 0x2, 
    0x13, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 
    0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x2, 0x2, 0x2, 0x189, 0x2, 0x27, 0x3, 
    0x2, 0x2, 0x2, 0x4, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x6, 0xac, 0x3, 0x2, 0x2, 
    0x2, 0x8, 0xcb, 0x3, 0x2, 0x2, 0x2, 0xa, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xc, 
    0xe4, 0x3, 0x2, 0x2, 0x2, 0xe, 0xee, 0x3, 0x2, 0x2, 0x2, 0x10, 0xf7, 
    0x3, 0x2, 0x2, 0x2, 0x12, 0x111, 0x3, 0x2, 0x2, 0x2, 0x14, 0x119, 0x3, 
    0x2, 0x2, 0x2, 0x16, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x18, 0x138, 0x3, 0x2, 
    0x2, 0x2, 0x1a, 0x13d, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x14f, 0x3, 0x2, 0x2, 
    0x2, 0x1e, 0x151, 0x3, 0x2, 0x2, 0x2, 0x20, 0x15c, 0x3, 0x2, 0x2, 0x2, 
    0x22, 0x161, 0x3, 0x2, 0x2, 0x2, 0x24, 0x26, 0x5, 0x4, 0x3, 0x2, 0x25, 
    0x24, 0x3, 0x2, 0x2, 0x2, 0x26, 0x29, 0x3, 0x2, 0x2, 0x2, 0x27, 0x25, 
    0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x3, 0x2, 0x2, 0x2, 0x28, 0x2a, 0x3, 
    0x2, 0x2, 0x2, 0x29, 0x27, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2b, 0x7, 0x2, 
    0x2, 0x3, 0x2b, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x2d, 0x7, 0x8, 0x2, 
    0x2, 0x2d, 0x2e, 0x7, 0x5, 0x2, 0x2, 0x2e, 0x2f, 0x7, 0x27, 0x2, 0x2, 
    0x2f, 0xa2, 0x8, 0x3, 0x1, 0x2, 0x30, 0x31, 0x7, 0x9, 0x2, 0x2, 0x31, 
    0x32, 0x7, 0x4, 0x2, 0x2, 0x32, 0x33, 0x7, 0x24, 0x2, 0x2, 0x33, 0x34, 
    0x7, 0x27, 0x2, 0x2, 0x34, 0xa2, 0x8, 0x3, 0x1, 0x2, 0x35, 0x36, 0x7, 
    0xa, 0x2, 0x2, 0x36, 0x37, 0x7, 0x4, 0x2, 0x2, 0x37, 0x38, 0x7, 0x24, 
    0x2, 0x2, 0x38, 0x39, 0x7, 0x27, 0x2, 0x2, 0x39, 0xa2, 0x8, 0x3, 0x1, 
    0x2, 0x3a, 0x3b, 0x7, 0xb, 0x2, 0x2, 0x3b, 0x3c, 0x7, 0x24, 0x2, 0x2, 
    0x3c, 0x3d, 0x7, 0x27, 0x2, 0x2, 0x3d, 0xa2, 0x8, 0x3, 0x1, 0x2, 0x3e, 
    0x3f, 0x7, 0x8, 0x2, 0x2, 0x3f, 0x40, 0x7, 0x7, 0x2, 0x2, 0x40, 0x41, 
    0x7, 0x27, 0x2, 0x2, 0x41, 0xa2, 0x8, 0x3, 0x1, 0x2, 0x42, 0x43, 0x7, 
    0x9, 0x2, 0x2, 0x43, 0x44, 0x7, 0x6, 0x2, 0x2, 0x44, 0x45, 0x7, 0x24, 
    0x2, 0x2, 0x45, 0x46, 0x7, 0x28, 0x2, 0x2, 0x46, 0x47, 0x5, 0x6, 0x4, 
    0x2, 0x47, 0x48, 0x7, 0x29, 0x2, 0x2, 0x48, 0x49, 0x7, 0x27, 0x2, 0x2, 
    0x49, 0x4a, 0x8, 0x3, 0x1, 0x2, 0x4a, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x4b, 
    0x4c, 0x7, 0xa, 0x2, 0x2, 0x4c, 0x4d, 0x7, 0x6, 0x2, 0x2, 0x4d, 0x4e, 
    0x7, 0x24, 0x2, 0x2, 0x4e, 0xa2, 0x8, 0x3, 0x1, 0x2, 0x4f, 0x50, 0x7, 
    0x1d, 0x2, 0x2, 0x50, 0x51, 0x7, 0x24, 0x2, 0x2, 0x51, 0xa2, 0x8, 0x3, 
    0x1, 0x2, 0x52, 0x53, 0x7, 0x10, 0x2, 0x2, 0x53, 0x54, 0x7, 0x11, 0x2, 
    0x2, 0x54, 0x55, 0x7, 0x24, 0x2, 0x2, 0x55, 0x56, 0x7, 0x12, 0x2, 0x2, 
    0x56, 0x57, 0x5, 0x10, 0x9, 0x2, 0x57, 0x58, 0x7, 0x27, 0x2, 0x2, 0x58, 
    0x59, 0x8, 0x3, 0x1, 0x2, 0x59, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 
    0x7, 0x13, 0x2, 0x2, 0x5b, 0x5c, 0x7, 0x14, 0x2, 0x2, 0x5c, 0x5d, 0x7, 
    0x24, 0x2, 0x2, 0x5d, 0x5e, 0x7, 0x15, 0x2, 0x2, 0x5e, 0x5f, 0x5, 0x16, 
    0xc, 0x2, 0x5f, 0x60, 0x7, 0x27, 0x2, 0x2, 0x60, 0x61, 0x8, 0x3, 0x1, 
    0x2, 0x61, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x62, 0x63, 0x7, 0x16, 0x2, 0x2, 
    0x63, 0x64, 0x7, 0x24, 0x2, 0x2, 0x64, 0x65, 0x7, 0x17, 0x2, 0x2, 0x65, 
    0x66, 0x5, 0x1e, 0x10, 0x2, 0x66, 0x67, 0x7, 0x15, 0x2, 0x2, 0x67, 0x68, 
    0x5, 0x16, 0xc, 0x2, 0x68, 0x69, 0x7, 0x27, 0x2, 0x2, 0x69, 0x6a, 0x8, 
    0x3, 0x1, 0x2, 0x6a, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x6c, 0x7, 0x18, 
    0x2, 0x2, 0x6c, 0x6d, 0x7, 0x2c, 0x2, 0x2, 0x6d, 0x6e, 0x7, 0x14, 0x2, 
    0x2, 0x6e, 0x6f, 0x7, 0x24, 0x2, 0x2, 0x6f, 0x70, 0x7, 0x15, 0x2, 0x2, 
    0x70, 0x71, 0x5, 0x16, 0xc, 0x2, 0x71, 0x72, 0x7, 0x27, 0x2, 0x2, 0x72, 
    0x73, 0x8, 0x3, 0x1, 0x2, 0x73, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 
    0x7, 0x18, 0x2, 0x2, 0x75, 0x76, 0x7, 0x2c, 0x2, 0x2, 0x76, 0x77, 0x7, 
    0x14, 0x2, 0x2, 0x77, 0x78, 0x5, 0xe, 0x8, 0x2, 0x78, 0x79, 0x7, 0x15, 
    0x2, 0x2, 0x79, 0x7a, 0x5, 0x16, 0xc, 0x2, 0x7a, 0x7b, 0x7, 0x27, 0x2, 
    0x2, 0x7b, 0x7c, 0x8, 0x3, 0x1, 0x2, 0x7c, 0xa2, 0x3, 0x2, 0x2, 0x2, 
    0x7d, 0x7e, 0x7, 0x18, 0x2, 0x2, 0x7e, 0x7f, 0x5, 0x22, 0x12, 0x2, 0x7f, 
    0x80, 0x7, 0x14, 0x2, 0x2, 0x80, 0x81, 0x7, 0x24, 0x2, 0x2, 0x81, 0x82, 
    0x7, 0x15, 0x2, 0x2, 0x82, 0x83, 0x5, 0x16, 0xc, 0x2, 0x83, 0x84, 0x7, 
    0x27, 0x2, 0x2, 0x84, 0x85, 0x8, 0x3, 0x1, 0x2, 0x85, 0xa2, 0x3, 0x2, 
    0x2, 0x2, 0x86, 0x87, 0x7, 0x18, 0x2, 0x2, 0x87, 0x88, 0x5, 0x22, 0x12, 
    0x2, 0x88, 0x89, 0x7, 0x14, 0x2, 0x2, 0x89, 0x8a, 0x5, 0xe, 0x8, 0x2, 
    0x8a, 0x8b, 0x7, 0x15, 0x2, 0x2, 0x8b, 0x8c, 0x5, 0x16, 0xc, 0x2, 0x8c, 
    0x8d, 0x7, 0x27, 0x2, 0x2, 0x8d, 0x8e, 0x8, 0x3, 0x1, 0x2, 0x8e, 0xa2, 
    0x3, 0x2, 0x2, 0x2, 0x8f, 0x90, 0x7, 0x9, 0x2, 0x2, 0x90, 0x91, 0x7, 
    0x1f, 0x2, 0x2, 0x91, 0x92, 0x7, 0x24, 0x2, 0x2, 0x92, 0x93, 0x7, 0x28, 
    0x2, 0x2, 0x93, 0x94, 0x5, 0xc, 0x7, 0x2, 0x94, 0x95, 0x7, 0x29, 0x2, 
    0x2, 0x95, 0x96, 0x7, 0x27, 0x2, 0x2, 0x96, 0x97, 0x8, 0x3, 0x1, 0x2, 
    0x97, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x98, 0x99, 0x7, 0xa, 0x2, 0x2, 0x99, 
    0x9a, 0x7, 0x1f, 0x2, 0x2, 0x9a, 0x9b, 0x7, 0x24, 0x2, 0x2, 0x9b, 0x9c, 
    0x7, 0x28, 0x2, 0x2, 0x9c, 0x9d, 0x5, 0xc, 0x7, 0x2, 0x9d, 0x9e, 0x7, 
    0x29, 0x2, 0x2, 0x9e, 0x9f, 0x7, 0x27, 0x2, 0x2, 0x9f, 0xa0, 0x8, 0x3, 
    0x1, 0x2, 0xa0, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x2c, 0x3, 0x2, 0x2, 
    0x2, 0xa1, 0x30, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x35, 0x3, 0x2, 0x2, 0x2, 
    0xa1, 0x3a, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x3e, 0x3, 0x2, 0x2, 0x2, 0xa1, 
    0x42, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x4b, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x4f, 
    0x3, 0x2, 0x2, 0x2, 0xa1, 0x52, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x5a, 0x3, 
    0x2, 0x2, 0x2, 0xa1, 0x62, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x6b, 0x3, 0x2, 
    0x2, 0x2, 0xa1, 0x74, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x7d, 0x3, 0x2, 0x2, 
    0x2, 0xa1, 0x86, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x8f, 0x3, 0x2, 0x2, 0x2, 
    0xa1, 0x98, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x5, 0x3, 0x2, 0x2, 0x2, 0xa3, 
    0xad, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa9, 0x5, 0x8, 0x5, 0x2, 0xa5, 0xa6, 
    0x7, 0x2a, 0x2, 0x2, 0xa6, 0xa8, 0x5, 0x8, 0x5, 0x2, 0xa7, 0xa5, 0x3, 
    0x2, 0x2, 0x2, 0xa8, 0xab, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xa7, 0x3, 0x2, 
    0x2, 0x2, 0xa9, 0xaa, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xad, 0x3, 0x2, 0x2, 
    0x2, 0xab, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xac, 0xa3, 0x3, 0x2, 0x2, 0x2, 
    0xac, 0xa4, 0x3, 0x2, 0x2, 0x2, 0xad, 0x7, 0x3, 0x2, 0x2, 0x2, 0xae, 
    0xaf, 0x7, 0x24, 0x2, 0x2, 0xaf, 0xb0, 0x5, 0xa, 0x6, 0x2, 0xb0, 0xb1, 
    0x8, 0x5, 0x1, 0x2, 0xb1, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xb2, 0xb3, 0x7, 
    0x24, 0x2, 0x2, 0xb3, 0xb4, 0x5, 0xa, 0x6, 0x2, 0xb4, 0xb5, 0x7, 0xe, 
    0x2, 0x2, 0xb5, 0xb6, 0x7, 0xf, 0x2, 0x2, 0xb6, 0xb7, 0x8, 0x5, 0x1, 
    0x2, 0xb7, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb9, 0x7, 0xc, 0x2, 0x2, 
    0xb9, 0xba, 0x7, 0xd, 0x2, 0x2, 0xba, 0xbb, 0x7, 0x28, 0x2, 0x2, 0xbb, 
    0xbc, 0x5, 0xc, 0x7, 0x2, 0xbc, 0xbd, 0x7, 0x29, 0x2, 0x2, 0xbd, 0xbe, 
    0x8, 0x5, 0x1, 0x2, 0xbe, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xc0, 0x7, 
    0x23, 0x2, 0x2, 0xc0, 0xc1, 0x7, 0xd, 0x2, 0x2, 0xc1, 0xc2, 0x7, 0x28, 
    0x2, 0x2, 0xc2, 0xc3, 0x5, 0xc, 0x7, 0x2, 0xc3, 0xc4, 0x7, 0x29, 0x2, 
    0x2, 0xc4, 0xc5, 0x7, 0x1e, 0x2, 0x2, 0xc5, 0xc6, 0x7, 0x24, 0x2, 0x2, 
    0xc6, 0xc7, 0x7, 0x28, 0x2, 0x2, 0xc7, 0xc8, 0x5, 0xc, 0x7, 0x2, 0xc8, 
    0xc9, 0x7, 0x29, 0x2, 0x2, 0xc9, 0xca, 0x8, 0x5, 0x1, 0x2, 0xca, 0xcc, 
    0x3, 0x2, 0x2, 0x2, 0xcb, 0xae, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xb2, 0x3, 
    0x2, 0x2, 0x2, 0xcb, 0xb8, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xbf, 0x3, 0x2, 
    0x2, 0x2, 0xcc, 0x9, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xce, 0x7, 0x1a, 0x2, 
    0x2, 0xce, 0xe3, 0x8, 0x6, 0x1, 0x2, 0xcf, 0xd0, 0x7, 0x1a, 0x2, 0x2, 
    0xd0, 0xd1, 0x7, 0x28, 0x2, 0x2, 0xd1, 0xd2, 0x7, 0x25, 0x2, 0x2, 0xd2, 
    0xd3, 0x7, 0x29, 0x2, 0x2, 0xd3, 0xe3, 0x8, 0x6, 0x1, 0x2, 0xd4, 0xd5, 
    0x7, 0x1b, 0x2, 0x2, 0xd5, 0xd6, 0x7, 0x28, 0x2, 0x2, 0xd6, 0xd7, 0x7, 
    0x25, 0x2, 0x2, 0xd7, 0xd8, 0x7, 0x29, 0x2, 0x2, 0xd8, 0xe3, 0x8, 0x6, 
    0x1, 0x2, 0xd9, 0xda, 0x7, 0x1c, 0x2, 0x2, 0xda, 0xdb, 0x7, 0x28, 0x2, 
    0x2, 0xdb, 0xdc, 0x7, 0x25, 0x2, 0x2, 0xdc, 0xdd, 0x7, 0x29, 0x2, 0x2, 
    0xdd, 0xe3, 0x8, 0x6, 0x1, 0x2, 0xde, 0xdf, 0x7, 0x21, 0x2, 0x2, 0xdf, 
    0xe3, 0x8, 0x6, 0x1, 0x2, 0xe0, 0xe1, 0x7, 0x22, 0x2, 0x2, 0xe1, 0xe3, 
    0x8, 0x6, 0x1, 0x2, 0xe2, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xcf, 0x3, 
    0x2, 0x2, 0x2, 0xe2, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xd9, 0x3, 0x2, 
    0x2, 0x2, 0xe2, 0xde, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe0, 0x3, 0x2, 0x2, 
    0x2, 0xe3, 0xb, 0x3, 0x2, 0x2, 0x2, 0xe4, 0xe5, 0x7, 0x24, 0x2, 0x2, 
    0xe5, 0xeb, 0x8, 0x7, 0x1, 0x2, 0xe6, 0xe7, 0x7, 0x2a, 0x2, 0x2, 0xe7, 
    0xe8, 0x7, 0x24, 0x2, 0x2, 0xe8, 0xea, 0x8, 0x7, 0x1, 0x2, 0xe9, 0xe6, 
    0x3, 0x2, 0x2, 0x2, 0xea, 0xed, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xe9, 0x3, 
    0x2, 0x2, 0x2, 0xeb, 0xec, 0x3, 0x2, 0x2, 0x2, 0xec, 0xd, 0x3, 0x2, 
    0x2, 0x2, 0xed, 0xeb, 0x3, 0x2, 0x2, 0x2, 0xee, 0xef, 0x7, 0x24, 0x2, 
    0x2, 0xef, 0xf3, 0x8, 0x8, 0x1, 0x2, 0xf0, 0xf1, 0x7, 0x2a, 0x2, 0x2, 
    0xf1, 0xf2, 0x7, 0x24, 0x2, 0x2, 0xf2, 0xf4, 0x8, 0x8, 0x1, 0x2, 0xf3, 
    0xf0, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xf5, 0x3, 0x2, 0x2, 0x2, 0xf5, 0xf3, 
    0x3, 0x2, 0x2, 0x2, 0xf5, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xf6, 0xf, 0x3, 
    0x2, 0x2, 0x2, 0xf7, 0xf8, 0x5, 0x12, 0xa, 0x2, 0xf8, 0xff, 0x8, 0x9, 
    0x1, 0x2, 0xf9, 0xfa, 0x7, 0x2a, 0x2, 0x2, 0xfa, 0xfb, 0x5, 0x12, 0xa, 
    0x2, 0xfb, 0xfc, 0x8, 0x9, 0x1, 0x2, 0xfc, 0xfe, 0x3, 0x2, 0x2, 0x2, 
    0xfd, 0xf9, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x101, 0x3, 0x2, 0x2, 0x2, 0xff, 
    0xfd, 0x3, 0x2, 0x2, 0x2, 0xff, 0x100, 0x3, 0x2, 0x2, 0x2, 0x100, 0x11, 
    0x3, 0x2, 0x2, 0x2, 0x101, 0xff, 0x3, 0x2, 0x2, 0x2, 0x102, 0x112, 0x3, 
    0x2, 0x2, 0x2, 0x103, 0x104, 0x7, 0x28, 0x2, 0x2, 0x104, 0x105, 0x5, 
    0x14, 0xb, 0x2, 0x105, 0x10c, 0x8, 0xa, 0x1, 0x2, 0x106, 0x107, 0x7, 
    0x2a, 0x2, 0x2, 0x107, 0x108, 0x5, 0x14, 0xb, 0x2, 0x108, 0x109, 0x8, 
    0xa, 0x1, 0x2, 0x109, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x106, 0x3, 
    0x2, 0x2, 0x2, 0x10b, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x10c, 0x10a, 0x3, 
    0x2, 0x2, 0x2, 0x10c, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x10f, 0x3, 
    0x2, 0x2, 0x2, 0x10e, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10f, 0x110, 0x7, 
    0x29, 0x2, 0x2, 0x110, 0x112, 0x3, 0x2, 0x2, 0x2, 0x111, 0x102, 0x3, 
    0x2, 0x2, 0x2, 0x111, 0x103, 0x3, 0x2, 0x2, 0x2, 0x112, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0x113, 0x114, 0x7, 0x25, 0x2, 0x2, 0x114, 0x11a, 0x8, 0xb, 
    0x1, 0x2, 0x115, 0x116, 0x7, 0x26, 0x2, 0x2, 0x116, 0x11a, 0x8, 0xb, 
    0x1, 0x2, 0x117, 0x118, 0x7, 0xf, 0x2, 0x2, 0x118, 0x11a, 0x8, 0xb, 
    0x1, 0x2, 0x119, 0x113, 0x3, 0x2, 0x2, 0x2, 0x119, 0x115, 0x3, 0x2, 
    0x2, 0x2, 0x119, 0x117, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x15, 0x3, 0x2, 0x2, 
    0x2, 0x11b, 0x120, 0x5, 0x18, 0xd, 0x2, 0x11c, 0x11d, 0x7, 0x20, 0x2, 
    0x2, 0x11d, 0x11f, 0x5, 0x18, 0xd, 0x2, 0x11e, 0x11c, 0x3, 0x2, 0x2, 
    0x2, 0x11f, 0x122, 0x3, 0x2, 0x2, 0x2, 0x120, 0x11e, 0x3, 0x2, 0x2, 
    0x2, 0x120, 0x121, 0x3, 0x2, 0x2, 0x2, 0x121, 0x17, 0x3, 0x2, 0x2, 0x2, 
    0x122, 0x120, 0x3, 0x2, 0x2, 0x2, 0x123, 0x124, 0x5, 0x1a, 0xe, 0x2, 
    0x124, 0x125, 0x5, 0x1c, 0xf, 0x2, 0x125, 0x126, 0x5, 0x14, 0xb, 0x2, 
    0x126, 0x127, 0x8, 0xd, 0x1, 0x2, 0x127, 0x139, 0x3, 0x2, 0x2, 0x2, 
    0x128, 0x129, 0x5, 0x1a, 0xe, 0x2, 0x129, 0x12a, 0x5, 0x1c, 0xf, 0x2, 
    0x12a, 0x12b, 0x5, 0x1a, 0xe, 0x2, 0x12b, 0x12c, 0x8, 0xd, 0x1, 0x2, 
    0x12c, 0x139, 0x3, 0x2, 0x2, 0x2, 0x12d, 0x12e, 0x5, 0x1a, 0xe, 0x2, 
    0x12e, 0x12f, 0x7, 0x19, 0x2, 0x2, 0x12f, 0x130, 0x7, 0xf, 0x2, 0x2, 
    0x130, 0x131, 0x8, 0xd, 0x1, 0x2, 0x131, 0x139, 0x3, 0x2, 0x2, 0x2, 
    0x132, 0x133, 0x5, 0x1a, 0xe, 0x2, 0x133, 0x134, 0x7, 0x19, 0x2, 0x2, 
    0x134, 0x135, 0x7, 0xe, 0x2, 0x2, 0x135, 0x136, 0x7, 0xf, 0x2, 0x2, 
    0x136, 0x137, 0x8, 0xd, 0x1, 0x2, 0x137, 0x139, 0x3, 0x2, 0x2, 0x2, 
    0x138, 0x123, 0x3, 0x2, 0x2, 0x2, 0x138, 0x128, 0x3, 0x2, 0x2, 0x2, 
    0x138, 0x12d, 0x3, 0x2, 0x2, 0x2, 0x138, 0x132, 0x3, 0x2, 0x2, 0x2, 
    0x139, 0x19, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13b, 0x7, 0x24, 0x2, 0x2, 
    0x13b, 0x13c, 0x8, 0xe, 0x1, 0x2, 0x13c, 0x13e, 0x7, 0x2b, 0x2, 0x2, 
    0x13d, 0x13a, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x13e, 0x3, 0x2, 0x2, 0x2, 
    0x13e, 0x13f, 0x3, 0x2, 0x2, 0x2, 0x13f, 0x140, 0x7, 0x24, 0x2, 0x2, 
    0x140, 0x141, 0x8, 0xe, 0x1, 0x2, 0x141, 0x142, 0x8, 0xe, 0x1, 0x2, 
    0x142, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x143, 0x144, 0x7, 0x2d, 0x2, 0x2, 
    0x144, 0x150, 0x8, 0xf, 0x1, 0x2, 0x145, 0x146, 0x7, 0x2e, 0x2, 0x2, 
    0x146, 0x150, 0x8, 0xf, 0x1, 0x2, 0x147, 0x148, 0x7, 0x2f, 0x2, 0x2, 
    0x148, 0x150, 0x8, 0xf, 0x1, 0x2, 0x149, 0x14a, 0x7, 0x30, 0x2, 0x2, 
    0x14a, 0x150, 0x8, 0xf, 0x1, 0x2, 0x14b, 0x14c, 0x7, 0x31, 0x2, 0x2, 
    0x14c, 0x150, 0x8, 0xf, 0x1, 0x2, 0x14d, 0x14e, 0x7, 0x32, 0x2, 0x2, 
    0x14e, 0x150, 0x8, 0xf, 0x1, 0x2, 0x14f, 0x143, 0x3, 0x2, 0x2, 0x2, 
    0x14f, 0x145, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x147, 0x3, 0x2, 0x2, 0x2, 
    0x14f, 0x149, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x14b, 0x3, 0x2, 0x2, 0x2, 
    0x14f, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x150, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x151, 
    0x152, 0x5, 0x20, 0x11, 0x2, 0x152, 0x159, 0x8, 0x10, 0x1, 0x2, 0x153, 
    0x154, 0x7, 0x2a, 0x2, 0x2, 0x154, 0x155, 0x5, 0x20, 0x11, 0x2, 0x155, 
    0x156, 0x8, 0x10, 0x1, 0x2, 0x156, 0x158, 0x3, 0x2, 0x2, 0x2, 0x157, 
    0x153, 0x3, 0x2, 0x2, 0x2, 0x158, 0x15b, 0x3, 0x2, 0x2, 0x2, 0x159, 
    0x157, 0x3, 0x2, 0x2, 0x2, 0x159, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x15a, 
    0x1f, 0x3, 0x2, 0x2, 0x2, 0x15b, 0x159, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x15d, 
    0x7, 0x24, 0x2, 0x2, 0x15d, 0x15e, 0x7, 0x31, 0x2, 0x2, 0x15e, 0x15f, 
    0x5, 0x14, 0xb, 0x2, 0x15f, 0x160, 0x8, 0x11, 0x1, 0x2, 0x160, 0x21, 
    0x3, 0x2, 0x2, 0x2, 0x161, 0x162, 0x5, 0x1a, 0xe, 0x2, 0x162, 0x169, 
    0x8, 0x12, 0x1, 0x2, 0x163, 0x164, 0x7, 0x2a, 0x2, 0x2, 0x164, 0x165, 
    0x5, 0x1a, 0xe, 0x2, 0x165, 0x166, 0x8, 0x12, 0x1, 0x2, 0x166, 0x168, 
    0x3, 0x2, 0x2, 0x2, 0x167, 0x163, 0x3, 0x2, 0x2, 0x2, 0x168, 0x16b, 
    0x3, 0x2, 0x2, 0x2, 0x169, 0x167, 0x3, 0x2, 0x2, 0x2, 0x169, 0x16a, 
    0x3, 0x2, 0x2, 0x2, 0x16a, 0x23, 0x3, 0x2, 0x2, 0x2, 0x16b, 0x169, 0x3, 
    0x2, 0x2, 0x2, 0x14, 0x27, 0xa1, 0xa9, 0xac, 0xcb, 0xe2, 0xeb, 0xf5, 
    0xff, 0x10c, 0x111, 0x119, 0x120, 0x138, 0x13d, 0x14f, 0x159, 0x169, 
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
