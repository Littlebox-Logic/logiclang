/*
 * File		: include/logic/preprocess.h
 * Date		: 2025.11.27 (Thus) 17:10
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#ifndef LOGIC_PREPROCESS_H
#define LOGIC_PREPROCESS_H

#define MAX_PATH_LENGTH 256

#include <stdio.h>
#include <stdint.h>

typedef enum
{
	STATIC	, EXTERN	, INTERN	, INLINE	, GLOBAL	, BELONG	, BLOCK		, RKILL		, RLOCK		,
	REG		, INIT		, THIS		, DEPEND	, VOID		, FLOAT		, LFLOAT	, HFLOAT	, CHAR		,
	WCHAR	, BYTE		, INT8		, INT16		, INT32		, INT64		, INT128	, UINT8		, UINT16	,
	UINT32	, UINT64	, UINT128	, UNUM		, BOOL		, ENUM		, DYMLIST	, LINKLS	, DLINKLS	,
	STRUCT	, UNION		, KW_FILE	, CHANNEL	, STRING	, ARRAY		, SETARR	, CLASS		, OBJECT	,
	THREAD	, MATRIX	, TENSOR	, VECTOR	, HEADER	, CALL		, FUNC		, RETURN	, PROCESS	,
	GOTO	, SIZEOF	, LENOF		, TYPEOF	, MALLOC	, DESTROY	, INSTAN	, ASM		, CONVERT	,
	RENAME	, PUBLIC	, PRIVATE	, PROTECT	, READONLY	, STRICT	, ENVCHK	, CONST		, VAR		,
	EVAL	, KW_NULL	, KW_EOF	, RESIZE	, IF		, ELSE		, ELIF		, WHILE		, DOWHILE	,
	FOR		, FORMAP	, BREAK		, CONTINUE	, SWITCH	, CASE		, DEFAULT
}	Keyword;

const char *keywords = {"static", "extern", "intern" , "inline"	 , "global"	 , "belong" , "block"  , "rkill" , "rlock"  ,
						"reg"	, "init"  , "this"   , "depend"	 , "void"	 , "float"  , "lfloat" , "hfloat", "char"   ,
						"wchar"	, "byte"  , "int8"   , "int16"	 , "int32"	 , "int64"  , "int128" , "uint8" , "uint16" ,
						"uint32", "uint64", "uint128", "unum"	 , "bool"	 , "enum"   , "dymlist", "linkls", "dlinkls",
						"struct", "union" , "file"   , "channel" , "string"	 , "array"  , "setarr" , "class" , "object" ,
						"thread", "matrix", "tensor" , "vector"	 , "header"	 , "call"   , "func"   , "return", "process",
						"goto"	, "sizeof", "lenof"  , "typeof"  , "malloc"	 , "destroy", "instan" , "asm"   , "convert",
						"rename", "public", "private", "protect" , "readonly", "strict" , "envchk" , "const" , "var"    ,
						"eval"	, "null"  , "eof"	 , "resize"	 , "if"		 , "else"   , "elif"   , "while" . "dowhile",
						"for"	, "formap", "break"  , "continue", "switch"	 , "case"	, "default"}

const char *operators = {"+" , "-" , "*", "/", "%", "^", "->", "." ,
						 "&&", "||", "~", "&", "|", "!", "=" , "==",
						 "!=", "&" , "?", ":", ">", "<", ">=", "<="}

typedef enum
{
	KEYWORD, INTEGER, NEGAINT, IDENTFR,
	STRINGV, CHARCTR, PROPRTY, OPERATR,
	REALNUM, NEGREAL, EOFLINE, EOFFILE,
	LFTBRAC, RGTBRAC, BLKBEGN, BLKTERM
}	Wordtype;

typedef struct _Header_Node
{
	char path[MAX_PATH_LENGTH];
	struct _Header_Node *next;
}	_Header, *Header;

typedef struct _Word_Node
{
	void *value;
	struct _Word_Node *next;
	size_t line;
	size_t col;
	Wordtype type;
}	_Word, *Word;

typedef struct
{
	Word	head;
	Word	tail;
	size_t	length;
}	_Wordlist, *Wordlist;

FILE *header_inc(const char *);
FILE *preprocess(const char *);

#endif
