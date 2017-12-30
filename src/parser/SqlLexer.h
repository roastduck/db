
// Generated from /home/rd/src/db/src/parser/SqlLexer.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"


/* lexer postinclude section */
#include "../config.h"
#include "../exception/IdentifierTooLongException.h"
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif




class  SqlLexer : public antlr4::Lexer {
public:
  enum {
    WhiteSpaces = 1, DATABASE = 2, DATABASES = 3, TABLE = 4, TABLES = 5, 
    SHOW = 6, CREATE = 7, DROP = 8, USE = 9, PRIMARY = 10, KEY = 11, NOT = 12, 
    NULL_TOKEN = 13, INSERT = 14, INTO = 15, VALUES = 16, DELETE = 17, FROM = 18, 
    WHERE = 19, UPDATE = 20, SET = 21, SELECT = 22, IS = 23, INT = 24, CHAR = 25, 
    VARCHAR = 26, DESC = 27, REFERENCES = 28, INDEX = 29, AND = 30, DATE = 31, 
    FLOAT = 32, FOREIGN = 33, CHECK = 34, IN = 35, ORDER = 36, BY = 37, 
    Identifier = 38, Int = 39, String = 40, SEMICOLON = 41, LEFT_PARENTHESIS = 42, 
    RIGHT_PARENTHESIS = 43, COMMA = 44, DOT = 45, STAR = 46, LESS_THAN = 47, 
    LESS_EQUAL = 48, GREATER_THAN = 49, GREATER_EQUAL = 50, EQUAL = 51, 
    NOT_EQUAL = 52
  };

  SqlLexer(antlr4::CharStream *input);
  ~SqlLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

  virtual void action(antlr4::RuleContext *context, size_t ruleIndex, size_t actionIndex) override;
private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.
  void IdentifierAction(antlr4::RuleContext *context, size_t actionIndex);

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

