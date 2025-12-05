/*
 * File		: src/preprocess.c
 * Date		: 2025.11.27 (Thus) 16:41
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/preprocess.h>
#include <logic/lexer.h>
#include <logic/AST.h>
#include <logic/log.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/*
 * Function		: header_inc
 * Description	: header link table -> temp header file.
 */
FILE *header_inc(const char *path)
{
	FILE *header_file = tmpfile();
	if (!header_file)
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to create temporary file for header records \"%s\": ", path);
		perror("");
		return NULL;
	}
	; // ----------------------------------------------
	return header_file;
}

/*
 * Function		: preprocess
 * Description	: raw Logic source code -> control code.
 */
FILE *preprocess(const char *src_code)
{
	Token	token		 = NULL;
	FILE	*pproc_file	 = NULL;

	if (!(pproc_file = tmpfile()))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to create temporary file for pre-processed code: ");
		perror("");
		return NULL;
	}

	while (true)
	{
		token = token_stream(src_code);
		switch (token -> type)
		{
			case KEYWORD:
			case INTEGER:
			case RESERVD:
			case IDENTFR:
			case STRINGV:
			case CHARCTR:
			case PROPRTY:
			case OPERATR:
			case REALNUM:
			case NEGREAL:
			case EOFLINE:
			case EOFFILE:	goto END;
			case LFTBRAC:
			case RGTBRAC:
			case BLKBEGN:
			case BLKTERM:
		}
	}
END:
	return pproc_file;
}
