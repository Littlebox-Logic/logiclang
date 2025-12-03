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

typedef struct _Header_Node
{
	char path[MAX_PATH_LENGTH];
	struct _Header_Node *next;
}	_Header, *Header;

FILE *header_inc(const char *);
FILE *preprocess(const char *);

#endif
