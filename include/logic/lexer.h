/*
 * File		: include/logic/lexer.h
 * Date		: 2025.12.03 (Wed) 14:45
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#ifndef LOGIC_LEXER_H
#define LOGIC_LEXER_H

#include <stdint.h>
#include <stdlib.h>

typedef enum
{
	KW_STATIC	, KW_EXTERN	, KW_INTERN	 , KW_INLINE	, KW_GLOBAL	  , KW_BELONG	, KW_BLOCK	 , KW_RKILL	 , KW_RLOCK		,
	KW_REG		, KW_INIT	, KW_THIS	 , KW_DEPEND	, KW_VOID	  , KW_FLOAT	, KW_LFLOAT	 , KW_HFLOAT , KW_CHAR		,
	KW_WCHAR	, KW_BYTE	, KW_INT8	 , KW_INT16		, KW_INT32	  , KW_INT64	, KW_INT128	 , KW_UINT8	 , KW_UINT16	,
	KW_UINT32	, KW_UINT64	, KW_UINT128 , KW_UNUM		, KW_BOOL	  , KW_ENUM		, KW_DYMLIST , KW_LINKLS , KW_DLINKLS	,
	KW_STRUCT	, KW_UNION	, KW_FILE	 , KW_CHANNEL	, KW_STRING	  , KW_ARRAY	, KW_SETARR	 , KW_CLASS	 , KW_OBJECT	,
	KW_THREAD	, KW_MATRIX	, KW_TENSOR	 , KW_VECTOR	, KW_HEADER	  , KW_CALL		, KW_FUNC	 , KW_RETURN , KW_PROCESS	,
	KW_GOTO		, KW_SIZEOF	, KW_LENOF	 , KW_TYPEOF	, KW_MALLOC	  , KW_DESTROY	, KW_INSTAN	 , KW_ASM	 , KW_CONVERT	,
	KW_RENAME	, KW_PUBLIC	, KW_PRIVATE , KW_PROTECT	, KW_READONLY , KW_STRICT	, KW_ENVCHK	 , KW_CONST	 , KW_VAR		,
	KW_EVAL		, KW_NULL	, KW_EOF	 , KW_RESIZE	, KW_IF		  , KW_ELSE		, KW_ELIF	 , KW_WHILE	 , KW_DOWHILE	,
	KW_FOR		, KW_FORMAP	, KW_BREAK	 , KW_CONTINUE	, KW_SWITCH	  , KW_CASE		, KW_DEFAULT , NOTKEY
}	Keyword;

typedef enum
{
	KEYWORD, INTEGER, NEGAINT, IDENTFR,
	STRINGV, CHARCTR, PROPRTY, OPERATR,
	REALNUM, NEGREAL, EOFLINE, EOFFILE,
	LFTBRAC, RGTBRAC, BLKBEGN, BLKTERM
}	Tokentype;

typedef struct
{
	void	*value;
	size_t	line;
	size_t	col;
	Tokentype type;
}	_Token, *Token;

Keyword keyword_check(const char *, size_t);
Token token_stream(const char *);

#endif
