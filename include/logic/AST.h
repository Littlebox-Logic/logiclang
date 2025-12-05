/*
 * File		: include/logic/AST.h
 * Date		: 2025.11.27 (Thus) 13:46
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#ifndef LOGIC_AST_H
#define LOGIC_AST_H

enum Type {TYPE_PTR, TYPE_VOID, TYPE_INT8, TYPE_INT16, TYPE_INT32, TYPE_INT64, TYPE_INT128, TYPE_WCHAR, TYPE_FLOAT, TYPE_LFLOAT, TYPE_STR, TYPE_LIST, TYPE_ARRAY, TYPE_SET, TYPE_HASH};
enum Ctrl_code {IF, ELSE, ELIF, WHILE, FOR, DO, SWITCH, CASE, DEFAULT, BREAK, CONTINUE, RETURN, CALL,\
				FUNC, VAR_DECLARE, VAR_ASSIGN, CONST_DECLARE, CONST_ASSIGN, DECORATOR, LENGTH, TYPEOF,\
				PLUS, MINUS, MULTIPLY, DIVIDE, MODULO, AND, OR, NOT, XOR, LEFT_SHIFT, RIGHT_SHIFT, ASSIGN,\
				PLUS_ASSIGN, MINUS_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN, MODULO_ASSIGN, AND_ASSIGN,\
				OR_ASSIGN, XOR_ASSIGN, LEFT_SHIFT_ASSIGN, RIGHT_SHIFT_ASSIGN, EQUAL, NOT_EQUAL, GREATER_THAN,\
				LESS_THAN, GREATER_EQUAL, LESS_EQUAL, INCREMENT, DECREMENT, ADDRESS_OF, VALUE_OF, CAST, RANGE,\
				INDEX, MEMBER_ACCESS, POINTER_MEMBER_ACCESS, FUNCTION_CALL, EXPRESSION_GROUPING, TERMINAL,\
				NO_OP, END_OF_FILE, UNKNOWN, ENTRY};

typedef struct
{
	void		*value;
	enum Type	type;
	char		name[32];
}	_var, *var;

typedef struct
{
	void		*addr;
	enum Type	rtn_type;
	var			paras;
	char		name[32];
}	_func, *func;

typedef struct _AST_Subs_Node
{
	func		function;
	var			variable;
	struct _AST_Node *parent;
	struct _AST_Node *next[];
}	_AST_Node, *AST_Node;

typedef struct _AST_Root_Node
{
	func		entry;
	var			variable;
	struct _AST_Node *next[];
}	_AST, *AST;

#endif
