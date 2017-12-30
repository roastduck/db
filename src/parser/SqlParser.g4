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
          { createTable($Identifier.text, $fieldList.cols, $fieldList.priIdx, $fieldList.fors, $fieldList.chk); }
        | DROP TABLE Identifier ';'
          { dropTable($Identifier.text); }
        | DESC Identifier ';'
          { desc($Identifier.text); }
        | INSERT INTO Identifier VALUES valueLists ';'
          { insert($Identifier.text, $valueLists.result); }
        | DELETE FROM Identifier WHERE whereClauses[$Identifier.text] ';'
          { remove($Identifier.text, $whereClauses.icm, $whereClauses.ocm); }
        | UPDATE Identifier SET setClauses WHERE whereClauses[$Identifier.text] ';'
          { update($Identifier.text, $setClauses.result, $whereClauses.icm, $whereClauses.ocm); }
        | SELECT '*' FROM Identifier WHERE whereClauses[$Identifier.text] groupClause orderClause ';'
          {
            select(
                None(), {$Identifier.text}, $whereClauses.icm, $whereClauses.ocm,
                $orderClause.result, $groupClause.result
            );
          }
        | SELECT '*' FROM tableList WHERE whereClauses[] groupClause orderClause ';'
          {
            select(
                None(), $tableList.result, $whereClauses.icm, $whereClauses.ocm,
                $orderClause.result, $groupClause.result
            );
          }
        | SELECT selAgg FROM Identifier WHERE whereClauses[$Identifier.text] groupClause orderClause ';'
          {
            select(
                $selAgg.result, {$Identifier.text}, $whereClauses.icm, $whereClauses.ocm,
                $orderClause.result, $groupClause.result, $selAgg.agg
            );
          }
        | SELECT selAgg FROM tableList WHERE whereClauses[] groupClause orderClause ';'
          {
            select(
                $selAgg.result, $tableList.result, $whereClauses.icm, $whereClauses.ocm,
                $orderClause.result, $groupClause.result, $selAgg.agg
            );
          }
        | CREATE INDEX Identifier '(' columnList ')' ';' // Modification: supporting multiple keys
          { createIndex($Identifier.text, $columnList.result); }
        | DROP INDEX Identifier '(' columnList ')' ';'
          { dropIndex($Identifier.text, $columnList.result); }
        ;

fieldList returns [Cols cols, PriIdx priIdx, Fors fors, Chk chk]
        : /* empty */ // Modification: we allow empty set
        | field[&$cols, &$priIdx, &$fors, &$chk] (',' field[&$cols, &$priIdx, &$fors, &$chk])*
        ;

field[Cols *cols, PriIdx *priIdx, Fors *fors, Chk *chk]
        : Identifier type
          { cols->push_back(std::make_pair($Identifier.text, (Column){$type.typeID, $type.length, false})); }
        | Identifier type NOT NULL_TOKEN
          { cols->push_back(std::make_pair($Identifier.text, (Column){$type.typeID, $type.length, true})); }
        | PRIMARY KEY '(' columnList ')' // Modification: supporting multiple keys
          {
            if (priIdx->isOk()) throw MultiplePrimaryException();
            *priIdx = $columnList.result;
          }
        | FOREIGN KEY '(' referrer=columnList ')' REFERENCES Identifier '(' referee=columnList ')'
          { fors->push_back((TableMgr::ForeignKey){$Identifier.text, std::move($referrer.result), std::move($referee.result)}); }
        | CHECK '(' Identifier IN valueList ')'
          { (*chk)[$Identifier.text] = $valueList.result; }
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

tableList returns [std::vector<std::string> result] // We match length >= 2 here, and discuss length = 1 above
        : Identifier {$result.push_back($Identifier.text);} (',' Identifier {$result.push_back($Identifier.text);})+
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

whereClauses[std::string defaultTb = ""] returns [ICM icm, OCM ocm]
        : whereClause[defaultTb, &$icm, &$ocm] (AND whereClause[defaultTb, &$icm, &$ocm])*
        ;

whereClause[std::string defaultTb, ICM *icm, OCM *ocm]
        : col[defaultTb] op value
          {
            if ($value.result.isOk())
                (*$icm)[$col.tb][$col.c].push_back((Table::ConLiteral){$op.dir, $value.result.ok()});
            else // Trick when the condition is always false
               (*$ocm)[std::make_pair($col.tb, $col.tb)].push_back((Table::OuterCon){Table::NE, $col.c, $col.c});
          }
        | col1=col[defaultTb] op col2=col[defaultTb]
          { (*$ocm)[std::make_pair($col1.tb, $col2.tb)].push_back((Table::OuterCon){$op.dir, $col1.c, $col2.c}); }
        | col[defaultTb] IS NULL_TOKEN
          { (*$icm)[$col.tb][$col.c].push_back((Table::ConLiteral){Table::IS_NULL, ""}); }
        | col[defaultTb] IS NOT NULL_TOKEN
          { (*$icm)[$col.tb][$col.c].push_back((Table::ConLiteral){Table::IS_NOT_NULL, ""}); }
        ;

col[std::string defaultTb] returns [std::string tb, std::string c]
        : (Identifier {$tb = $Identifier.text;} '.')? Identifier {$c = $Identifier.text;}
          { if ($tb.empty()) $tb = $defaultTb; }
        ;

colAgg[std::string defaultTb] returns [std::string tb, std::string c, Aggregate::AggType agg]
        : col[defaultTb]
          { $tb = $col.tb, $c = $col.c, $agg = Aggregate::NONE; }
        | SUM '(' col[defaultTb] ')'
          { $tb = $col.tb, $c = $col.c, $agg = Aggregate::SUM; }
        | AVG '(' col[defaultTb] ')'
          { $tb = $col.tb, $c = $col.c, $agg = Aggregate::AVG; }
        | MIN '(' col[defaultTb] ')'
          { $tb = $col.tb, $c = $col.c, $agg = Aggregate::MIN; }
        | MAX '(' col[defaultTb] ')'
          { $tb = $col.tb, $c = $col.c, $agg = Aggregate::MAX; }
        ;

op returns [Table::ConDir dir]
        : '<'
          { $dir = Table::LT; }
        | '<='
          { $dir = Table::LE; }
        | '>'
          { $dir = Table::GT; }
        | '>='
          { $dir = Table::GE; }
        | '='
          { $dir = Table::EQ; }
        | '<>'
          { $dir = Table::NE; }
        ;

setClauses returns [Table::ColL result] // We treate SET a=1, a=2 as legal operation
        : setClause {$result[$setClause.k] = $setClause.v;} (',' setClause {$result[$setClause.k] = $setClause.v;})*
        ;

setClause returns [std::string k, Optional<std::string> v]
        : Identifier '=' value
          { $k = $Identifier.text, $v = $value.result; }
        ;

selector returns [Tgt result]
        : col[""] {$result[$col.tb].push_back($col.c);} (',' col[""] {$result[$col.tb].push_back($col.c);})*
        ;

selAgg returns [Tgt result, Agg agg]
        : colAgg[""]
          {
            $result[$colAgg.tb].push_back($colAgg.c);
            if ($colAgg.agg != Aggregate::NONE)
                $agg[$colAgg.tb][$colAgg.c] = $colAgg.agg;
          }
          (',' colAgg[""]
          {
            $result[$colAgg.tb].push_back($colAgg.c);
            if ($colAgg.agg != Aggregate::NONE)
                $agg[$colAgg.tb][$colAgg.c] = $colAgg.agg;
          }
          )*
        ;

orderClause returns [Tgt result]
        : /* empty */
          { $result = {}; }
        | ORDER BY selector
          { $result = $selector.result; }
        ;

groupClause returns [Tgt result]
        : /* empty */
          { $result = {}; }
        | GROUP BY selector
          { $result = $selector.result; }
        ;

