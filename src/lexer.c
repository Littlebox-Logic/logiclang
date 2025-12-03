/*
 * File		: src/lexer.c
 * Date		: 2025.12.03 (Wed) 14:13
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/lexer.h>
#include <logic/preprocess.h>
#include <ctype.h>

int lexi_append(Wordlist list, Wordtype type, void *value, size_t line, size_t col)
{
	Word new_node = NULL;
	if (!(list -> head = (Word)malloc(sizeof(_Word))))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Failed to allocate memory for new word node.\n");
		return EXIT_FAILURE;
	}

	new_node -> type	= type;
	new_node -> value	= value;
	new_node -> line	= line;
	new_node -> col		= col;
	new_node -> next	= NULL;

	if (!list -> head)	list -> tail = list -> head = new_node;
	else				list -> tail = list -> tail -> next = new_node;

	list -> length ++;
	return EXIT_SUCCESS;
}

void error_report(const char *msg);

Keyword keyword_check(const char *word, size_t wordlen)
{
	uint16_t wordbyte16;
	uint32_t wordbyte32;

	switch (wordlen)
	{
		case 2:
			if (word[0] == 'i' && word[1] == 'f')	return IF;
			return NOTKEY;
		case 3:
			wordbyte16 = *(const uint16_t *)word;
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
			wordbyte32 = *(const uint32_t *)word & 0x00ffffff;
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
					return NOTKEY
				case 'v':
					if (wordbyte32 == 0x6f6964)	return VOID;
					return NOTKEY;
				default:
					return NOTKEY;
			}
		case 5:
			wordbyte32 = *(const uint32_t *)(word + 1);
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
			// developing.
		case 7:
			// developing.
		case 8:
			switch (*(const uint64_t *)word)
			{
				case 0x636f6e74696e7565:	return CONTINUE;
				case 0x726561646f6e6c79:	return READONLY;
				default:	return NOTKEY;
			}
		default:	return NOTKEY;
	}
}

Token lexi_load(const char *src_code)
{
	static size_t line	 = 1;
	static size_t colend = 0;
	static size_t srclen = strlen(src_code);
	static size_t seek	 = 0;
	uint32_t
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
			{
				if (++ seek >= srclen)
				{
					token -> value	= NULL;
					token -> line	= line;
					token -> col	= seek - colend;
					token -> type	= EOFFILE;
					return token;
				}
			}
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
					token -> value	= NULL;
					token -> line	= line;
					token -> col	= seek - colend;
					token -> type	= EOFFILE;
					return token;
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
			keyword_check(src_code + seek, wordlen);
		}
	}
}

