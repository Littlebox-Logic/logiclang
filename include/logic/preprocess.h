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

typedef enum
{
	KEYWORD, INTEGER, NEGAINT, IDENTFR,
	STRINGV, CHARCTR, PROPRTY, OPERATR,
	REALNUM, NEGREAL, EOFLINE, EOFFILE,
	LFTBRAC, RGTBRAC, BLKBEGN, BLKTERM
}	Wordtype;

typedef struct _Header_Struct
{
	char path[MAX_PATH_LENGTH];
	struct _Header *next;
}	_Header, *Header;

typedef struct _Word_Struct
{
	void *value;
	struct _Word_Struct *next;
	Wordtype type,
}	_Word, *Word;

FILE *header_inc(const char *);
FILE *preprocess(const char *);

#endif
