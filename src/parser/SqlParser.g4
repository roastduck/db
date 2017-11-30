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

// Actual grammar starts.
program : stmt* EOF
        ;

stmt    : SHOW DATABASES ';'
          { showDbs(); }
        | CREATE DATABASE Identifier ';'
          { createDb($Identifier.text); }
        | DROP DATABASE Identifier ';'
          { dropDb($Identifier.text); }
        | USE Identifier ';'
          { use($Identifier.text); }
        | SHOW TABLES ';'
          { showTables(); }
        ;

