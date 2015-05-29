grammar Madeup;

program
  : block EOF
  ;

block
  : line*
  ;

line
  : expression NEWLINE # Statement
  | COMMENT NEWLINE # Comment
  | NEWLINE # Blank
  ;

expression
  : '-' expression # Negation

  | '(' expression ')' # Parenthesized
  | PIPE expression PIPE # AbsoluteValue
  | expression LEFT_SUBSCRIPT expression RIGHT_SUBSCRIPT # ArraySubscript
  | expression DOT ID # ArrayLength
  | expression (BY expression)+ OF expression # ArrayMultiDimensional
  | expression OF expression # ArrayOneDimensional
  | NOT expression # Not

  | expression RAISED_TO expression # Power

  | expression (MULTIPLY|DIVIDE|DECIMALDIVIDE|REMAINDER) expression # Multiply

  | expression (ADD|SUBTRACT) expression # Plus

  | expression (LESSER|LESSER_OR_EQUAL|GREATER|GREATER_OR_EQUAL) expression # Order

  | expression (EQUAL|NOT_EQUAL) expression # Compare

  | expression AND expression # And

  | expression OR expression # Or

  | REPEAT expression NEWLINE block end # Repeat
  | WHILE expression NEWLINE block end # While

  | FOR ID TO expression (BY expression)? NEWLINE block end # ForTo
  | FOR ID THROUGH expression (BY expression)? NEWLINE block end # ForThrough
  | FOR ID IN expression RANGE expression BY expression NEWLINE block end # ForRangeBy
  | FOR ID IN expression (',' byExpression=expression)? RANGE expression NEWLINE block end # ForRange

  | FOR ID IN expression NEWLINE block end # ForInBadRange

  | IF condition=expression THEN thenExpression=expression ELSE elseExpression=expression # IfOneLiner
  | IF condition=expression NEWLINE thenBlock=block (ELSE NEWLINE elseBlock=block)? end # If

  | ID # CallWithoutParameters
  | ID expression (',' expression)*? # CallWithParameters
  | ID (ID IS expression)+ # CallWithNamedParameters

  | name=ID (expression ',')* expression expression (','? expression)* # CallWithMissingComma

  | INTEGER # IntegerLiteral
  | DECIMAL # DecimalLiteral
  | BOOLEAN # BooleanLiteral
  | STRING # StringLiteral

  | TO ID+ (NEWLINE block end | EQUALS expression) # DefineFunction
  | ID (NEWLINE block end | EQUALS expression) # DefineVariable
  | expression LEFT_SUBSCRIPT expression RIGHT_SUBSCRIPT EQUALS (NEWLINE block end|expression) # DefineArrayElement

  | END # StrayEnd
  | TO keyword ID* (NEWLINE block end | EQUALS expression) # KeywordAsFunctionName
  | TO name=ID ID* keyword (ID | keyword)* (NEWLINE block end | EQUALS expression) # KeywordAsParameterName
  | TO name=ID ID* EQUALS (NEWLINE block end) # DefineFunctionExtraEquals
  | name=ID ID+ (NEWLINE block end | EQUALS expression) # DefineFunctionMissingTo
  | keyword EQUALS expression # KeywordAssignment
  | op=RAISED_TO expression # BinaryOnlyOneOperand
  | expression op=RAISED_TO # BinaryOnlyOneOperand
  | op=(MULTIPLY|DIVIDE|DECIMALDIVIDE|REMAINDER) expression # BinaryOnlyOneOperand
  | expression op=(MULTIPLY|DIVIDE|DECIMALDIVIDE|REMAINDER) # BinaryOnlyOneOperand
  | expression op=(ADD|SUBTRACT) # BinaryOnlyOneOperand
  | op=ADD expression # BinaryOnlyOneOperand
  | op=(LESSER|LESSER_OR_EQUAL|GREATER|GREATER_OR_EQUAL) expression # BinaryOnlyOneOperand
  | expression op=(LESSER|LESSER_OR_EQUAL|GREATER|GREATER_OR_EQUAL) # BinaryOnlyOneOperand
  | expression op=(EQUAL|NOT_EQUAL) # BinaryOnlyOneOperand
  | op=(EQUAL|NOT_EQUAL) expression # BinaryOnlyOneOperand
  | expression op=AND # BinaryOnlyOneOperand
  | op=AND expression # BinaryOnlyOneOperand
  | expression op=OR # BinaryOnlyOneOperand
  | op=OR expression # BinaryOnlyOneOperand
  ;

end
  : END # EndWithoutExtra
  | END expression # EndWithExtra
  ;

keyword
  : OF | BY | NOT | AND | OR | THROUGH | FOR | TO | IN | BOOLEAN | REPEAT | WHILE | END | IF | THEN | ELSE;

OF : 'of';
BY : 'by';
COMMA : ',';
DOT : '.';
LEFT_SUBSCRIPT : '[';
RIGHT_SUBSCRIPT : ']';
NOT : 'not';
LESSER : '<';
LESSER_OR_EQUAL : '<=';
GREATER : '>';
GREATER_OR_EQUAL : '>=';
EQUAL : '==';
NOT_EQUAL : '!=';
AND : 'and';
OR : 'or';
THROUGH : 'through';
MULTIPLY : '*';
DIVIDE : '/';
DECIMALDIVIDE : '//';
REMAINDER : '%';
RAISED_TO : '^';
ADD : '+';
SUBTRACT : '- ';
EQUALS : '=';
IS : ':';
PIPE : '|';

RANGE : '..';
INTEGER : '-'? [0-9]+;
DECIMAL : '-'? [0-9]+ '.' [0-9]+;

FOR : 'for';
TO : 'to';
IN : 'in';
BOOLEAN : 'true' | 'false';
STRING : '"' .*? '"';

REPEAT : 'repeat';
WHILE : 'while';
END : 'end';
IF : 'if';
THEN : 'then';
ELSE : 'else';

COMMENT : '--' (~ ('\n'|'\r'))*;
NEWLINE : '\r'? '\n';

ID : [a-zA-Z] [a-zA-Z0-9]*;

WHITESPACE : [ \t] -> skip;
