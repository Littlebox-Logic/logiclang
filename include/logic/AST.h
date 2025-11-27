/*
 * File		: include/logic/AST.h
 * Date		: 2025.11.27 (Thus) 13:46
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#ifndef LOGIC_AST_H
#define LOGIC_AST_H

enum Type {PTR, VOID, INT8, INT16, INT64, INT128, WCHAR, FLOAT, LFLOAT, STR, LIST, ARRAY, SET, HASH};

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

typedef struct _AST_Node
{
	func		function;
	var			variable;
	struct _AST_Node *parent;
	struct _AST_Node *next[];
}	_AST, *AST;

#endif
