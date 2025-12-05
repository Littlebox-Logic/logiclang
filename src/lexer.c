/*
 * File		: src/lexer.c
 * Date		: 2025.12.03 (Wed) 14:13
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/lexer.h>
#include <logic/preprocess.h>
#include <logic/log.h>
#include <string.h>
#include <ctype.h>

const char *operators[] = {"+" , "-" , "*", "/", "%", "^", "->", "." ,
						   "&&", "||", "!", "&", "|", "~", "=" , "==",
						   "!=", "@" , "?", ":", ">", "<", ">=", "<="};

/*
 * Function		: keyword_check
 * Description	: Word -> keyword or normal identifier.
 * 				  Matching strings by big-endian hexadecimal integers.
 */
Keyword keyword_check(const char *word, size_t wordlen)
{
	uint16_t wordbyte16;
	uint32_t wordbyte32;
	uint64_t wordbyte64;

	switch (wordlen)
	{
		case 2:
			if (word[0] == 'i' && word[1] == 'f')	return KW_IF;
			return NOTKEY;
		case 3:
			wordbyte16 = (uint16_t)(unsigned char)word[1] << 8 | (uint16_t)(unsigned char)word[2];
			switch (word[0])
			{
				case 'a':
					if (wordbyte16 == 0x736d)	return KW_ASM;
					return NOTKEY;
				case 'e':
					if (wordbyte16 == 0x6f66)	return KW_EOF;
					return NOTKEY;
				case 'f':
					if (wordbyte16 == 0x6f72)	return KW_FOR;
					return NOTKEY;
				case 'r':
					if (wordbyte16 == 0x6567)	return KW_REG;
					return NOTKEY;
				case 'v':
					if (wordbyte16 == 0x6172)	return KW_VAR;
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
					if (wordbyte32 == 0x6f6f6c)	return KW_BOOL;
					if (wordbyte32 == 0x797465)	return KW_BYTE;
					return NOTKEY;
				case 'c':
					if (wordbyte32 == 0x616c6c)	return KW_CALL;
					if (wordbyte32 == 0x617365)	return KW_CASE;
					if (wordbyte32 == 0x686172)	return KW_CHAR;
					return NOTKEY;
				case 'e':
					if (wordbyte32 == 0x6c6966)	return KW_ELIF;
					if (wordbyte32 == 0x6c7365)	return KW_ELSE;
					if (wordbyte32 == 0x6e756d)	return KW_ENUM;
					if (wordbyte32 == 0x76616c)	return KW_EVAL;
					return NOTKEY;
				case 'f':
					if (wordbyte32 == 0x696c65)	return KW_FILE;
					if (wordbyte32 == 0x756e63)	return KW_FUNC;
					return NOTKEY;
				case 'g':
					if (wordbyte32 == 0x6f746f)	return KW_GOTO;
					return NOTKEY;
				case 'i':
					if (wordbyte32 == 0x6e6974)	return KW_INIT;
					if (wordbyte32 == 0x6e7438)	return KW_INT8;
					return NOTKEY;
				case 'n':
					if (wordbyte32 == 0x756c6c)	return KW_NULL;
					return NOTKEY;
				case 't':
					if (wordbyte32 == 0x686973)	return KW_THIS;
					return NOTKEY;
				case 'u':
					if (wordbyte32 == 0x6e756d)	return KW_UNUM;
					return NOTKEY;
				case 'v':
					if (wordbyte32 == 0x6f6964)	return KW_VOID;
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
					if (wordbyte32 == 0x72726179)	return KW_ARRAY;
					return NOTKEY;
				case 'b':
					if (wordbyte32 == 0x6c6f636b)	return KW_BLOCK;
					if (wordbyte32 == 0x7265616b)	return KW_BREAK;
					return NOTKEY;
				case 'c':
					if (wordbyte32 == 0x6c617373)	return KW_CLASS;
					if (wordbyte32 == 0x6f6e7374)	return KW_CONST;
					return NOTKEY;
				case 'f':
					if (wordbyte32 == 0x6c6f6174)	return KW_FLOAT;
					return NOTKEY;
				case 'i':
					if (wordbyte32 == 0x6e743136)	return KW_INT16;
					if (wordbyte32 == 0x6e743332)	return KW_INT32;
					if (wordbyte32 == 0x6e743634)	return KW_INT64;
					return NOTKEY;
				case 'l':
					if (wordbyte32 == 0x656e6f66)	return KW_LENOF;
					return NOTKEY;
				case 'r':
					if (wordbyte32 == 0x6c6f636b)	return KW_RLOCK;
					if (wordbyte32 == 0x6b696c6c)	return KW_RKILL;
					return NOTKEY;
				case 'u':
					if (wordbyte32 == 0x696e7438)	return KW_UINT8;
					if (wordbyte32 == 0x6e696f6e)	return KW_UNION;
					return NOTKEY;
				case 'w':
					if (wordbyte32 == 0x63686172)	return KW_WCHAR;
					if (wordbyte32 == 0x68696c65)	return KW_WHILE;
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
					if (wordbyte64 == 0x656c6f6e67)	return KW_BELONG;
					return NOTKEY;
				case 'd':
					if (wordbyte64 == 0x6570656e64)	return KW_DEPEND;
					return NOTKEY;
				case 'e':
					if (wordbyte64 == 0x6e7663686b)	return KW_ENVCHK;
					if (wordbyte64 == 0x787465726e)	return KW_EXTERN;
					return NOTKEY;
				case 'f':
					if (wordbyte64 == 0x6f726d6170)	return KW_FORMAP;
					return NOTKEY;
				case 'g':
					if (wordbyte64 == 0x6c6f62616c)	return KW_GLOBAL;
					return NOTKEY;
				case 'h':
					if (wordbyte64 == 0x6561646572)	return KW_HEADER;
					if (wordbyte64 == 0x666c6f6174)	return KW_HFLOAT;
					return NOTKEY;
				case 'i':
					if (wordbyte64 == 0x6e6c696e65)	return KW_INLINE;
					if (wordbyte64 == 0x6e7374616e)	return KW_INSTAN;
					if (wordbyte64 == 0x6e7465726e)	return KW_INTERN;
					if (wordbyte64 == 0x6e74313238)	return KW_INT128;
					return NOTKEY;
				case 'l':
					if (wordbyte64 == 0x696e6b6c73)	return KW_LINKLS;
					if (wordbyte64 == 0x666c6f6174)	return KW_LFLOAT;
					return NOTKEY;
				case 'm':
					if (wordbyte64 == 0x616c6c6f63)	return KW_MALLOC;
					if (wordbyte64 == 0x6174726978)	return KW_MATRIX;
					return NOTKEY;
				case 'o':
					if (wordbyte64 == 0x626a656374)	return KW_OBJECT;
					return NOTKEY;
				case 'p':
					if (wordbyte64 == 0x75626c6963)	return KW_PUBLIC;
					return NOTKEY;
				case 'r':
					if (wordbyte64 == 0x656e616d65)	return KW_RENAME;
					if (wordbyte64 == 0x6573697a65)	return KW_RESIZE;
					if (wordbyte64 == 0x657475726e)	return KW_RETURN;
					return NOTKEY;
				case 's':
					if (wordbyte64 == 0x6574617272)	return KW_SETARR;
					if (wordbyte64 == 0x697a656f66)	return KW_SIZEOF;
					if (wordbyte64 == 0x7461746963)	return KW_STATIC;
					if (wordbyte64 == 0x7472696374)	return KW_STRICT;
					if (wordbyte64 == 0x7472696e67)	return KW_STRING;
					if (wordbyte64 == 0x7472756374)	return KW_STRUCT;
					if (wordbyte64 == 0x7769746368)	return KW_SWITCH;
					return NOTKEY;
				case 't':
					if (wordbyte64 == 0x656e736f72)	return KW_TENSOR;
					if (wordbyte64 == 0x6872656164)	return KW_THREAD;
					if (wordbyte64 == 0x7970656f66)	return KW_TYPEOF;
					return NOTKEY;
				case 'u':
					if (wordbyte64 == 0x696e743136)	return KW_UINT16;
					if (wordbyte64 == 0x696e743332)	return KW_UINT32;
					if (wordbyte64 == 0x696e743634)	return KW_UINT64;
					return NOTKEY;
				case 'v':
					if (wordbyte64 == 0x6563746f72)	return KW_VECTOR;
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
					if (wordbyte64 == 0x68616e6e656c)	return KW_CHANNEL;
					if (wordbyte64 == 0x6f6e76657274)	return KW_CONVERT;
				case 'd':
					if (wordbyte64 == 0x656661756c74)	return KW_DEFAULT;
					if (wordbyte64 == 0x657374726f79)	return KW_DESTROY;
					if (wordbyte64 == 0x6c696e6b6c73)	return KW_DLINKLS;
					if (wordbyte64 == 0x6f7768696c65)	return KW_DOWHILE;
					if (wordbyte64 == 0x796d6c697374)	return KW_DYMLIST;
				case 'p':
					if (wordbyte64 == 0x726976617465)	return KW_PRIVATE;
					if (wordbyte64 == 0x726f63657373)	return KW_PROCESS;
					if (wordbyte64 == 0x726f74656374)	return KW_PROTECT;
				case 'u':
					if (wordbyte64 == 0x657374726f79)	return KW_UINT128;
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
						  (uint64_t)(unsigned char)word[7]);
			if (wordbyte64 == 0x636f6e74696e7565)	return KW_CONTINUE;
			if (wordbyte64 == 0x726561646f6e6c79)	return KW_READONLY;
			return NOTKEY;
		default:	return NOTKEY;
	}
}

/*
 * Function		: token_stream
 * Description	: read token infomation from source code and return the token structure.
 * 			  	  provided for syntax processor.
 */
Token token_stream(const char *src_code)
{
	static size_t line	 = 1;
	static size_t colend = 0;
	static size_t srclen = 0;
	static size_t seek	 = 0;

	Keyword keyword_type;
	Token token;

	srclen = strlen(src_code);

	if (!(token = (Token)malloc(sizeof(_Token))))
	{
		Log(ERROR, "Failed to allocate memory for new token.", line, seek - colend + 1);
		return NULL;
	}

	while (seek < srclen)
	{
		/* Update index of lines. */
		if (src_code[seek ++] == '\n')
		{
			line ++;
			colend = seek;
			continue;
		}

		if (isspace((unsigned char)src_code[seek - 1]))	continue;

		/* Comments. */
		if ((src_code[seek - 1] == '/' && src_code[seek] == '/')
		 ||	(src_code[seek - 1] == '#' && src_code[seek] == '!'))
		{
			while (src_code[seek] != '\n')
				if (++ seek >= srclen)	goto EOFLABEL;

			line ++;
			colend = ++ seek;
		}
		if (src_code[seek - 1] == '/' && src_code[seek] == '*')
		{
			seek ++;
			while (!(src_code[seek - 1] == '*' && src_code[seek] == '/'))
			{
				if ((src_code[seek]) == '\n')
				{
					line ++;
					colend = seek + 1;
				}

				if (++ seek >= srclen)
				{
					Log(WARN, "Unexpected EOF before \"*/\".", line, seek - colend + 1);
					goto EOFLABEL;
				}
			}
			seek ++;
		}
		
		/* Keywords & identifiers parsing. */
		if ((src_code[seek - 1] >= 'a' && src_code[seek - 1] <= 'z') || (src_code[seek - 1] >= 'A' && src_code[seek - 1] <= 'Z') || src_code[seek - 1] == '_')
		{
			size_t wordlen = 1;
			while (seek ++ < srclen	&&	((src_code[seek - 1] >= 'a' && src_code[seek] <= 'z')
									||	 (src_code[seek - 1] >= 'A' && src_code[seek] <= 'Z')
									||	 (src_code[seek - 1] >= '0' && src_code[seek] <= '9')
									||	  src_code[seek - 1] == '_'))	wordlen ++;
			if ((keyword_type = keyword_check(src_code + seek, wordlen)) != NOTKEY)
			{
				token -> value.kw_type = keyword_type;
				token -> line	= line;
				token -> col	= seek - colend + 1;
				token -> type	= KEYWORD;
				return token;
			}
			else
			{
				token -> value.string = (char *)malloc(wordlen + 1);
				strncpy(token -> value.string, src_code + seek, wordlen);
				token -> value.string[wordlen] = '\0';
				token -> line	= line;
				token -> col	= seek - colend + 1;
				token -> type	= IDENTFR;
				return token;
			}
		}

		/* String. */
		if (src_code[seek - 1] == '\"')
		{
			size_t stringlen = 0;
			while (src_code[seek] != '\"')
			{
				if (src_code[seek] == '\n')
				{
					line ++;
					colend = seek + 1;
				}

				if (++ seek >= srclen)
				{
					Log(ERROR, "Unexpected EOF before the end of string.", line, seek - colend + 1);
					free(token);
					return NULL;
				}
				stringlen ++;
			}
			seek ++;
			token -> value.string = (char *)malloc(stringlen + 1);
			strncpy(token -> value.string, src_code + seek, stringlen);
			token -> value.string[stringlen] = '\0';
			token -> line	= line;
			token -> col	= seek - colend + 1;
			token -> type	= STRINGV;
			return token;
		}

		if (src_code[seek - 1] == '\'')
		{
			if (seek + 1 >= srclen)
			{
				Log(ERROR, "Unexpected EOF before the end of character.", line, seek - colend + 1);
				free(token);
				return NULL;
			}
			if (src_code[seek] == '\\')	token -> value.character = src_code[++ seek];
			else if (src_code[seek] == '\n' || src_code[seek] == '\t')
			{
				Log(ERROR, "The character format must be \'<CHAR>\'.", line, seek - colend + 1);
				free(token);
				return NULL;
			}
			else	token -> value.character = src_code[seek];
			seek += 2;
			token -> line	= line;
			token -> col	= seek - colend + 1;
			token -> type	= CHARCTR;
			return token;
		}

		if (src_code[seek - 1] >= '0' && src_code[seek - 1] <= '9')
		{
			size_t int_length = 0;
			sscanf(src_code + seek - 1, "%i%n", (int *)&token -> value.uinteger, (int *)&int_length);
			seek += int_length - 1;
			token -> line	= line;
			token -> col	= seek - colend + 1;
			token -> type	= INTEGER;
			return token;
		}

		switch (src_code[seek - 1])
		{
			case '+':
				token -> value.op_type = OP_SUB;
				goto OPLABEL;
			case '-':
				if (seek >= srclen)
				{
					Msg(ERROR, "Unexpected EOF after operator.");
					free(token);
					return NULL;
				}
				token -> value.op_type = src_code[seek ++] == '>' ?	OP_ARW : OP_SUB;
				goto OPLABEL;
			case '*':
				token -> value.op_type = OP_MUL;
				goto OPLABEL;
			case '/':
				token -> value.op_type = OP_DIV;
				goto OPLABEL;
			case '%':
				token -> value.op_type = OP_MOD;
				goto OPLABEL;
			case '^':
				token -> value.op_type = OP_POW;
				goto OPLABEL;
			case '.':
				token -> value.op_type = OP_DOT;
				goto OPLABEL;
			case '&':
				if (seek >= srclen)
				{
					Msg(ERROR, "Unexpected EOF after operator.");
					free(token);
					return NULL;
				}
				token -> value.op_type = src_code[seek ++] == '&' ? OP_LAD : OP_BAD;
				goto OPLABEL;
			case '|':
				if (seek >= srclen)
				{
					Msg(ERROR, "Unexpected EOF after operator.");
					free(token);
					return NULL;
				}
				token -> value.op_type = src_code[seek ++] == '|' ? OP_LOR : OP_BOR;
				goto OPLABEL;
			case '!':
				if (seek >= srclen)
				{
					Msg(ERROR, "Unexpected EOF after operator.");
					free(token);
					return NULL;
				}
				token -> value.op_type = src_code[seek ++] == '=' ? OP_NEQ : OP_LNT;
				goto OPLABEL;
			case '~':
				token -> value.op_type = OP_BNT;
			case '=':
				if (seek >= srclen)
				{
					Msg(ERROR, "Unexpected EOF after operator.");
					free(token);
					return NULL;
				}
				token -> value.op_type = src_code[seek ++] == '=' ? OP_EQU : OP_EVL;
				goto OPLABEL;
			case '@':
				token -> value.op_type = OP_DCR;
				goto OPLABEL;
			case '?':
				token -> value.op_type = OP_QST;
				goto OPLABEL;
			case ':':
				token -> value.op_type = OP_CLN;
				goto OPLABEL;
			case '>':
				if (seek >= srclen)
				{
					Msg(ERROR, "Unexpected EOF after operator.");
					free(token);
					return NULL;
				}
				token -> value.op_type = src_code[seek ++] == '=' ? OP_GEQ : OP_GTN;
				goto OPLABEL;
			case '<':
				if (seek >= srclen)
				{
					Msg(ERROR, "Unexpected EOF after operator.");
					free(token);
					return NULL;
				}
				token -> value.op_type = src_code[seek ++] == '=' ? OP_LEQ : OP_LTN;
				goto OPLABEL;
			case '{':
				token -> value.general = NULL;
				token -> line	= line;
				token -> col	= seek - colend + 1;
				token -> type	= BLKBEGN;
			case '}':
				token -> value.general = NULL;
				token -> line	= line;
				token -> col	= seek - colend + 1;
				token -> type	= BLKTERM;
				return token;
			case '(':
				token -> value.general = NULL;
				token -> line	= line;
				token -> col	= seek - colend + 1;
				token -> type	= LFTBRAC;
				return token;
			case ')':
				token -> value.general = NULL;
				token -> line	= line;
				token -> col	= seek - colend + 1;
				token -> type	= RGTBRAC;
				return token;
		}
	}

EOFLABEL:
	token -> value.general = NULL;
	token -> line	= line;
	token -> col	= seek - colend + 1;
	token -> type	= EOFFILE;
	return token;

OPLABEL:
	token -> line	= line;
	token -> col	= seek - colend + 1;
	token -> type	= OPERATR;
	return token;
}
