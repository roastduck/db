lexer grammar SqlLexer;

// Follows directly after the standard #includes in h + cpp files.
@lexer::postinclude {
/* lexer postinclude section */
#include "../config.h"
#include "../exception/IdentifierTooLongException.h"
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

fragment A: [aA];
fragment B: [bB];
fragment C: [cC];
fragment D: [dD];
fragment E: [eE];
fragment F: [fF];
fragment G: [gG];
fragment H: [hH];
fragment I: [iI];
fragment J: [jJ];
fragment K: [kK];
fragment L: [lL];
fragment M: [mM];
fragment N: [nN];
fragment O: [oO];
fragment P: [pP];
fragment Q: [qQ];
fragment R: [rR];
fragment S: [sS];
fragment T: [tT];
fragment U: [uU];
fragment V: [vV];
fragment W: [wW];
fragment X: [xX];
fragment Y: [yY];
fragment Z: [zZ];

WhiteSpaces: [ \t\r\n]+ -> skip;

DATABASE:   D A T A B A S E;
DATABASES:  D A T A B A S E S;
TABLE:      T A B L E;
TABLES:     T A B L E S;
SHOW:       S H O W;
CREATE:     C R E A T E;
DROP:       D R O P;
USE:        U S E;
PRIMARY:    P R I M A R Y;
KEY:        K E Y;
NOT:        N O T;
NULL_TOKEN: N U L L; // We don't use `NULL` because it's a macro in C
INSERT:     I N S E R T;
INTO:       I N T O;
VALUES:     V A L U E S;
DELETE:     D E L E T E;
FROM:       F R O M;
WHERE:      W H E R E;
UPDATE:     U P D A T E;
SET:        S E T;
SELECT:     S E L E C T;
IS:         I S;
DESC:       D E S C;
REFERENCES: R E F E R E N C E S;
INDEX:      I N D E X;
AND:        A N D;
FOREIGN:    F O R E I G N;
CHECK:      C H E C K;
IN:         I N;
ORDER:      O R D E R;
GROUP:      G R O U P;
BY:         B Y;
SUM:        S U M;
AVG:        A V G;
MAX:        M A X;
MIN:        M I N;

Identifier: [A-Za-z][_0-9A-Za-z]*
    { if (getText().length() > MAX_IDENTIFIER_LEN) throw IdentifierTooLongException(getText()); }
;
Unsigned:   [0-9]+;
Number:     '-'? [0-9]+ ('.' [0-9]*)?;
// Modification: decimal (not in requirement, but presented in test cases)
String:     '\'' (~['] | '\\\'' | '\\\\')* '\'';
// Modification: string with escaping (not in requirement, but presented in test cases)

SEMICOLON:          ';';
LEFT_PARENTHESIS:   '(';
RIGHT_PARENTHESIS:  ')';
COMMA:              ',';
DOT:                '.';
STAR:               '*';
LESS_THAN:          '<';
LESS_EQUAL:         '<=';
GREATER_THAN:       '>';
GREATER_EQUAL:      '>=';
EQUAL:              '=';
NOT_EQUAL:          '<>';
