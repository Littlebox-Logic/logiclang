/*
 * File		: src/lexer.c
 * Date		: 2025.12.03 (Wed) 14:13
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/lexer.h>
#include <logic/preprocess.h>
#include <ctype.h>

Keyword keyword_check(const char *word, size_t wordlen)
{
	uint16_t wordbyte16;
	uint32_t wordbyte32;
	uint64_t wordbyte64;

	switch (wordlen)
	{
		case 2:
			if (word[0] == 'i' && word[1] == 'f')	return IF;
			return NOTKEY;
		case 3:
			wordbyte16 = (uint16_t)(unsigned char)word[1] << 8 | (uint16_t)(unsigned char)word[2];
			switch (word[0])
			{
				case 'a':
					if (wordbyte16 == 0x736d)	return ASM;
					return NOTKEY;
				case 'e':
					if (wordbyte16 == 0x6f66)	return KW_EOF;
					return NOTKEY;
				case 'f':
					if (wordbyte16 == 0x6f72)	return FOR;
					return NOTKEY;
				case 'r':
					if (wordbyte16 == 0x6567)	return REG;
					return NOTKEY;
				case 'v':
					if (wordbyte16 == 0x6172)	return VAR;
					return NOTKEY;
				default:
					return NOTKEY;
			}
		case 4:
			wordbyte32 = (uint32_t)(unsigned char)word[1] << 16 |
						 (uint32_t)(unsigned char)word[2] << 8  |
						 (uint32_t)(unsigned char)word[3];
			switch (word[0])
			{
				case 'b':
					if (wordbyte32 == 0x6f6f6c)	return BOOL;
					if (wordbyte32 == 0x797465)	return BYTE;
					return NOTKEY;
				case 'c':
					if (wordbyte32 == 0x616c6c)	return CALL;
					if (wordbyte32 == 0x617365)	return CASE;
					if (wordbyte32 == 0x686172)	return CHAR;
					return NOTKEY;
				case 'e':
					if (wordbyte32 == 0x6c6966)	return ELIF;
					if (wordbyte32 == 0x6c7365)	return ELSE;
					if (wordbyte32 == 0x6e756d)	return ENUM;
					if (wordbyte32 == 0x76616c)	return EVAL;
					return NOTKEY;
				case 'f':
					if (wordbyte32 == 0x696c65)	return KW_FILE;
					if (wordbyte32 == 0x756e63)	return FUNC;
					return NOTKEY;
				case 'g':
					if (wordbyte32 == 0x6f746f)	return GOTO;
					return NOTKEY;
				case 'i':
					if (wordbyte32 == 0x6e6974)	return INIT;
					if (wordbyte32 == 0x6e7438)	return INT8;
					return NOTKEY;
				case 'n':
					if (wordbyte32 == 0x756c6c)	return KW_NULL;
					return NOTKEY;
				case 't':
					if (wordbyte32 == 0x686973)	return THIS;
					return NOTKEY;
				case 'u':
					if (wordbyte32 == 0x6e756d)	return UNUM;
					return NOTKEY;
				case 'v':
					if (wordbyte32 == 0x6f6964)	return VOID;
					return NOTKEY;
				default:
					return NOTKEY;
			}
		case 5:
			wordbyte32 = (uint32_t)(unsigned char)word[1] << 24 |
						 (uint32_t)(unsigned char)word[2] << 16 |
						 (uint32_t)(unsigned char)word[3] << 8  |
						 (uint32_t)(unsigned char)word[4];
			switch (word[0])
			{
				case 'a':
					if (wordbyte32 == 0x72726179)	return ARRAY;
					return NOTKEY;
				case 'b':
					if (wordbyte32 == 0x6c6f636b)	return BLOCK;
					if (wordbyte32 == 0x7265616b)	return BREAK;
					return NOTKEY;
				case 'c':
					if (wordbyte32 == 0x6c617373)	return CLASS;
					if (wordbyte32 == 0x6f6e7374)	return CONST;
					return NOTKEY;
				case 'f':
					if (wordbyte32 == 0x6c6f6174)	return FLOAT;
					return NOTKEY;
				case 'i':
					if (wordbyte32 == 0x6e743136)	return INT16;
					if (wordbyte32 == 0x6e743332)	return INT32;
					if (wordbyte32 == 0x6e743634)	return INT64;
					return NOTKEY;
				case 'l':
					if (wordbyte32 == 0x656e6f66)	return LENOF;
					return NOTKEY;
				case 'r':
					if (wordbyte32 == 0x6c6f636b)	return RLOCK;
					if (wordbyte32 == 0x6b696c6c)	return RKILL;
					return NOTKEY;
				case 'u':
					if (wordbyte32 == 0x696e7438)	return UINT8;
					if (wordbyte32 == 0x6e696f6e)	return UNION;
					return NOTKEY;
				case 'w':
					if (wordbyte32 == 0x63686172)	return WCHAR;
					if (wordbyte32 == 0x68696c65)	return WHILE;
					return NOTKEY;
				default:
					return NOTKEY;
			}
		case 6:
			wordbyte64 = (uint64_t)(unsigned char)word[1] << 32 |
						 (uint64_t)(unsigned char)word[2] << 24 |
						 (uint64_t)(unsigned char)word[3] << 16 |
						 (uint64_t)(unsigned char)word[4] <<  8 |
						 (uint64_t)(unsigned char)word[5];
			switch (word[0])
			{
				case 'b':
					if (wordbyte64 == 0x656c6f6e67)	return BELONG;
					return NOTKEY;
				case 'd':
					if (wordbyte64 == 0x6570656e64)	return DEPEND;
					return NOTKEY;
				case 'e':
					if (wordbyte64 == 0x6e7663686b)	return ENVCHK;
					if (wordbyte64 == 0x787465726e)	return EXTERN;
					return NOTKEY;
				case 'f':
					if (wordbyte64 == 0x6f726d6170)	return FORMAP;
					return NOTKEY;
				case 'g':
					if (wordbyte64 == 0x6c6f62616c)	return GLOBAL;
					return NOTKEY;
				case 'h':
					if (wordbyte64 == 0x6561646572)	return HEADER;
					if (wordbyte64 == 0x666c6f6174)	return HFLOAT;
					return NOTKEY;
				case 'i':
					if (wordbyte64 == 0x6e6c696e65)	return INLINE;
					if (wordbyte64 == 0x6e7374616e)	return INSTAN;
					if (wordbyte64 == 0x6e7465726e)	return INTERN;
					if (wordbyte64 == 0x6e74313238)	return INT128;
					return NOTKEY;
				case 'l':
					if (wordbyte64 == 0x696e6b6c73)	return LINKLS;
					if (wordbyte64 == 0x666c6f6174)	return LFLOAT;
					return NOTKEY;
				case 'm':
					if (wordbyte64 == 0x616c6c6f63)	return MALLOC;
					if (wordbyte64 == 0x6174726978)	return MATRIX;
					return NOTKEY;
				case 'o':
					if (wordbyte64 == 0x626a656374)	return OBJECT;
					return NOTKEY;
				case 'p':
					if (wordbyte64 == 0x75626c6963)	return PUBLIC;
					return NOTKEY;
				case 'r':
					if (wordbyte64 == 0x656e616d65)	return RENAME;
					if (wordbyte64 == 0x6573697a65)	return RESIZE;
					if (wordbyte64 == 0x657475726e)	return RETURN;
					return NOTKEY;
				case 's':
					if (wordbyte64 == 0x6574617272)	return SETARR;
					if (wordbyte64 == 0x697a656f66)	return SIZEOF;
					if (wordbyte64 == 0x7461746963)	return STATIC;
					if (wordbyte64 == 0x7472696374)	return STRICT;
					if (wordbyte64 == 0x7472696e67)	return STRING;
					if (wordbyte64 == 0x7472756374)	return STRUCT;
					if (wordbyte64 == 0x7769746368)	return SWITCH;
					return NOTKEY;
				case 't':
					if (wordbyte64 == 0x656e736f72)	return TENSOR;
					if (wordbyte64 == 0x6872656164)	return THREAD;
					if (wordbyte64 == 0x7970656f66)	return TYPEOF;
					return NOTKEY;
				case 'u':
					if (wordbyte64 == 0x696e743136)	return UINT16;
					if (wordbyte64 == 0x696e743332)	return UINT32;
					if (wordbyte64 == 0x696e743634)	return UINT64;
					return NOTKEY;
				case 'v':
					if (wordbyte64 == 0x6563746f72)	return VECTOR;
					return NOTKEY;
				default:
					return NOTKEY;
			}
		case 7:
			wordbyte64 = (uint64_t)(unsigned char)word[1] << 40 |
						 (uint64_t)(unsigned char)word[2] << 32 |
						 (uint64_t)(unsigned char)word[3] << 24 |
						 (uint64_t)(unsigned char)word[4] << 16 |
						 (uint64_t)(unsigned char)word[5] <<  8 |
						 (uint64_t)(unsigned char)word[6];
			switch (word[0])
			{
				case 'c':
					if (wordbyte64 == 0x68616e6e656c)	return CHANNEL;
					if (wordbyte64 == 0x6f6e76657274)	return CONVERT;
				case 'd':
					if (wordbyte64 == 0x656661756c74)	return DEFAULT;
					if (wordbyte64 == 0x657374726f79)	return DESTROY;
					if (wordbyte64 == 0x6c696e6b6c73)	return DLINKLS;
					if (wordbyte64 == 0x6f7768696c65)	return DOWHILE;
					if (wordbyte64 == 0x796d6c697374)	return DYMLIST;
				case 'p':
					if (wordbyte64 == 0x726976617465)	return PRIVATE;
					if (wordbyte64 == 0x726f63657373)	return PROCESS;
					if (wordbyte64 == 0x726f74656374)	return PROTECT;
				case 'u':
					if (wordbyte64 == 0x657374726f79)	return UINT128;
				default:
					return NOTKEY;
			}
		case 8:
			wordbyte64 = ((uint64_t)(unsigned char)word[0] << 56 |
						  (uint64_t)(unsigned char)word[1] << 48 |
						  (uint64_t)(unsigned char)word[2] << 40 |
						  (uint64_t)(unsigned char)word[3] << 32 |
						  (uint64_t)(unsigned char)word[4] << 24 |
						  (uint64_t)(unsigned char)word[5] << 16 |
						  (uint64_t)(unsigned char)word[6] << 8  |
						  (uint64_t)(unsigned char)word[7])
			if (wordbyte64 == 0x636f6e74696e7565)	return CONTINUE;
			if (wordbyte64 == 0x726561646f6e6c79)	return READONLY;
			return NOTKEY;
		default:	return NOTKEY;
	}
}

Token token_stream(const char *src_code)
{
	static size_t line	 = 1;
	static size_t colend = 0;
	static size_t srclen = strlen(src_code);
	static size_t seek	 = 0;

	Keyword keyword_type;
	Token token;

	if (!(token = (Token)malloc(sizeof(_Token))))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Failed to allocate memory for new token.");
		return NULL;
	}

	while (seek <= srclen)
	{
		if (seek >= srclen)
		{
			token -> value	= NULL;
			token -> line	= line;
			token -> col	= seek - colend;
			token -> type	= EOFFILE;
			return token;
		}

		if (src_code[seek ++] == '\n')
		{
			line ++;
			colend = seek;
			continue;
		}

		if (isspace((unsigned char)src_code[seek - 1]))	continue;

		/* Comments. */
		if (src_code[seek - 1] == '/' && src_code[seek] == '/'
		 || src_code[seek - 1] == '#' && src_code[seek] == '!')
		{
			while (src_code[seek] != '\n')
				if (++ seek >= srclen)	goto EOFLABEL;

			line ++;
			colend = seek;
		}
		if (src_code[seek - 1] == '/' && src_code[seek] == '*')
		{
			seek ++;
			while (!(src_code[seek - 1] == '*' && src_code[seek] == '/'))
			{
				if (++ seek >= srclen)
				{
					fprintf(stderr, "logicc: \033[;93mWARN\033[0m: Unexpected EOF before \"*/\".");
					goto EOFLABEL;
				}
			}
		}
		
		/* Keywords & identifiers parsing. */
		if (src_code[seek - 1] >= 'a' && src_code[seek - 1] <= 'z' || src_code[seek - 1] >= 'A' && src_code <= 'Z')
		{
			const char *word;
			size_t wordlen = 1;
			while (seek ++ < srclen	&&	src_code[seek - 1] >= 'a' && src_code[seek] <= 'z'
									||	src_code[seek - 1] >= 'A' && src_code[seek] <= 'Z'
									||	src_code[seek - 1] >= '0' && src_code[seek] <= '9'
									||	src_code[seek - 1] == '_')	wordlen ++;
			if ((keyword_type = keyword_check(src_code + seek, wordlen)) != NOTKEY)
			{
				token -> value	= (void *)keyword_type;
				token -> line	= line;
				token -> col	= seek - colend;
				token -> type	= KEYWORD;
				return token;
			}
			else
			{
				token -> value	= (char *)malloc(wordlen + 1);
				strncpy(token -> value, src_code + seek, wordlen)
				token -> value[wordlen] = '\0';
				token -> line	= line;
				token -> col	= seek - colend;
				token -> type	= IDENTFR;
				return token;
			}
		}
	}

EOFLABEL:
	token -> value	= NULL;
	token -> line	= line;
	token -> col	= seek - colend;
	token -> type	= EOFFILE;
	return token;
}
