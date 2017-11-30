parser grammar SqlParser;

options {
	tokenVocab = SqlLexer;
    superClass = BaseParser;
}

// Follows directly after the standard #includes in h + cpp files.
@parser::postinclude {
/* parser postinclude section */
#include "BaseParser.h"

#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

// Actual grammar start.
program : stmt* EOF
        ;

stmt    : CREATE DATABASE Identifier ';'
          { createDb($Identifier.text); }
        ;

