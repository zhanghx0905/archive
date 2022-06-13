// Refer to https://decaf-lang.github.io/minidecaf-tutorial/docs/
grammar MiniDecaf;

prog:
	(func | globalDecl)* EOF;

globalDecl:
	ty IDENT ('=' NUM)? ';'		# globalIntOrPointerDecl
	| ty IDENT ('[' NUM ']')+ ';'	# globalArrayDecl
	;


func:
	ty IDENT '(' (ty IDENT (',' ty IDENT)*)? ')' '{' blockItem* '}'	# definedFunc
	| ty IDENT '(' (ty IDENT (',' ty IDENT)*)? ')' ';'	# declaredFunc
	;

ty: 'int' '*'*;

blockItem: localDel | stmt;

// 变量定义
localDel:
	ty IDENT ('=' expr)? ';'		# localIntOrPointerDecl
	| ty IDENT ('[' NUM ']')+ ';'	# localArrayDecl
	;

stmt: 
	'return' expr ';' # returnStmt
	| expr? ';' # exprStmt
	| 'if' '(' expr ')' stmt ('else' stmt)? # ifStmt
	| '{' blockItem* '}' # blockStmt
	| 'while' '(' expr ')' stmt # whileStmt
	| 'for' '(' (localDel | expr? ';') expr? ';' expr? ')' stmt	# forStmt
	| 'do' stmt 'while' '(' expr ')' ';' # doStmt
	| 'break' ';' # breakStmt
	| 'continue' ';' # continueStmt
	;

expr: unary '=' expr | ternary;

ternary: lor '?' expr ':' ternary | lor;

lor: lor '||' lor | land;

land: land '&&' land | leq;

leq: leq ('==' | '!=') leq | rel;

rel: rel ('<' | '>' | '>='| '<=') rel | add;

add: add ('+' | '-') add | mul;

mul: mul ('*' | '/' | '%') mul | unary;

unary:
	('-' | '~' | '!' | '&' | '*') unary	# opUnary
	| '(' ty ')' unary # castUnary
	| postfix # postfixUnary
	;

// 函数调用/表达式
postfix:
	IDENT '(' (expr (',' expr)*)? ')' # callPostfix
	| postfix '[' expr ']' # subscriptPostfix
	| primary # primaryPostfix
	;

primary:
	NUM # numPrimary
	| IDENT # identPrimary
	| '(' expr ')' # parenthesizedPrimary
	;

/* lexer */
WS: [ \t\r\n\u000C] -> skip;

IDENT: [a-zA-Z_] [a-zA-Z_0-9]*;
NUM: [0-9]+;

// comment
// The specification of minidecaf doesn't allow commenting,
// but we provide the comment feature here for the convenience of debugging.
COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
