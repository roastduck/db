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
        | CREATE TABLE Identifier '(' fieldList ')' ';'
          { createTable($Identifier.text, $fieldList.cols, $fieldList.priIdx, $fieldList.fors); }
        | DROP TABLE Identifier
          { dropTable($Identifier.text); }
        | DESC Identifier
          { desc($Identifier.text); }
        | INSERT INTO Identifier VALUES valueLists ';'
          { insert($Identifier.text, $valueLists.result); }
        ;

fieldList returns [Cols cols, PriIdx priIdx, Fors fors]
        : /* empty */ // Modification: we allow empty set
        | field[&$cols, &$priIdx, &$fors] (',' field[&$cols, &$priIdx, &$fors])*
        ;

field[Cols *cols, PriIdx *priIdx, Fors *fors]
        : Identifier type
          { cols->push_back(std::make_pair($Identifier.text, (Column){$type.typeID, $type.length, false})); }
        | Identifier type NOT NULL_TOKEN
          { cols->push_back(std::make_pair($Identifier.text, (Column){$type.typeID, $type.length, true})); }
        | PRIMARY KEY '(' columnList ')'
          {
            if (priIdx->isOk()) throw MultiplePrimaryException();
            *priIdx = $columnList.result;
          }
        | FOREIGN KEY '(' referrer=columnList ')' REFERENCES Identifier '(' referee=columnList ')'
          { fors->push_back((TableMgr::ForeignKey){$Identifier.text, std::move($referrer.result), std::move($referee.result)}); }
        ;

type returns [Type::TypeID typeID, int length = 0]
        : INT // Modification: we allow INT without length
          { $typeID = Type::INT; }
        | INT '(' Int ')'
          { $typeID = Type::INT, $length = std::stoi($Int.text); }
        | CHAR '(' Int ')'
          { $typeID = Type::CHAR, $length = std::stoi($Int.text); }
        | VARCHAR '(' Int ')'
          { $typeID = Type::VARCHAR, $length = std::stoi($Int.text); }
        | DATE
          { $typeID = Type::DATE; }
        | FLOAT
          { $typeID = Type::FLOAT; }
        ;

columnList returns [Table::Index result]
        : Identifier {$result.push_back($Identifier.text);} (',' Identifier {$result.push_back($Identifier.text);})*
        ;

valueLists returns [VLists result]
        : valueList {append($result, $valueList.result);} (',' valueList {append($result, $valueList.result);})*
        ;

valueList returns [VList result]
        : /* empty */ // Modification: we allow empty set
        | '(' value {append($result, $value.result);} (',' value {append($result, $value.result);})* ')'
        ;

value returns [Optional<std::string> result]
        : Int
          { $result = $Int.text; }
        | String // Out string has no escaping
          { $result = $String.text.substr(1, $String.text.length() - 2); }
        | NULL_TOKEN
          { $result = None(); }
        ;

