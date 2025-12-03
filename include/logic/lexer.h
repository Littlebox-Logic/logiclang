/*
 * File		: include/logic/lexer.c
 * Date		: 2025.12.03 (Wed) 14:45
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#ifndef LOGIC_LEXER_H
#define LOGIC_LEXER_H

#include <stdint.h>

typedef uint32_t Keybit32;

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

#endif
