/*
 * File		: src/compiler.c
 * Date		: 2025.11.27 (Thus) 10:36
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logic/compiler.h>
#include <logic/preprocess.h>
#include <logic/asmbuild.h>
#include <logic/AST.h>

/*
 * Function		: AST_generate
 * Description	: control code -> AST tree.
 */
AST AST_generate(FILE *code)
{
	size_t code_length		= 0;
	size_t ast_next_count	= 1;
	char *code_buffer		= NULL;
	AST astree				= NULL;

	if (!code) return NULL;

	fseek(code, 0, SEEK_END);
	code_length = ftell(code);
	fseek(code, 0, SEEK_SET);
	if (code_length == 0)
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Empty pre-processed control code file provided for AST generation.\n");
		return NULL;
	}

	if (!(code_buffer = (char *)malloc(code_length + 1)))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Failed to allocate memory for reading pre-processed control code: ");
		perror("");
		return NULL;
	}

	code_buffer[code_length] = '\0';
	fread(code_buffer, 1, code_length, code);

	if (!(astree = (AST)malloc(sizeof(_AST) + ast_next_count * sizeof(struct _AST_Node *))))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Failed to allocate memory for AST root node: ");
		perror("");
		free(code_buffer);
		return NULL;
	}
	astree -> entry = NULL;
	astree -> variable = NULL;
	astree -> next[0] = NULL;

	for (size_t index = 0; index < code_length; index ++)
	{
		switch (code_buffer[index])
		{
			case IF: break;
			case ELSE: break;
			case ELIF: break;
			case WHILE: break;
			case FOR: break;
			case DO: break;
			case SWITCH: break;
			case CASE: break;
			case DEFAULT: break;
			case BREAK: break;
			case CONTINUE: break;
			case RETURN: break;
			case CALL: break;
			case FUNC: break;
			case VAR_DECLARE: break;
			case VAR_ASSIGN: break;
			case CONST_DECLARE: break;
			case CONST_ASSIGN: break;
			case DECORATOR: break;
			case LENGTH: break;
			case TYPEOF: break;
			case PLUS: break;
			case MINUS: break;
			case MULTIPLY: break;
			case DIVIDE: break;
			case MODULO: break;
			case AND: break;
			case OR: break;
			case NOT: break;
			case XOR: break;
			case LEFT_SHIFT: break;
			case RIGHT_SHIFT: break;
			case ASSIGN: break;
			case PLUS_ASSIGN: break;
			case MINUS_ASSIGN: break;
			case MULTIPLY_ASSIGN: break;
			case DIVIDE_ASSIGN: break;
			case MODULO_ASSIGN: break;
			case AND_ASSIGN: break;
			case OR_ASSIGN: break;
			case XOR_ASSIGN: break;
			case LEFT_SHIFT_ASSIGN: break;
			case RIGHT_SHIFT_ASSIGN: break;
			case EQUAL: break;
			case NOT_EQUAL: break;
			case GREATER_THAN: break;
			case LESS_THAN: break;
			case GREATER_EQUAL: break;
			case LESS_EQUAL: break;
			case INCREMENT: break;
			case DECREMENT: break;
			case ADDRESS_OF: break;
			case VALUE_OF: break;
			case CAST: break;
			case RANGE: break;
			case INDEX: break;
			case MEMBER_ACCESS: break;
			case POINTER_MEMBER_ACCESS: break;
			case FUNCTION_CALL: break;
			case EXPRESSION_GROUPING: break;
			case TERMINAL: break;
			case NO_OP: break;
			case END_OF_FILE: break;
			case UNKNOWN: break;
			case ENTRY:
				if (!(astree -> entry = (func)malloc(sizeof(_func))))
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Failed to allocate memory for AST entry function node: ");
					perror("");
					free(code_buffer);
					free(astree);
					return NULL;
				}
				strcpy(astree -> entry -> name, "mainx");
				astree -> entry -> rtn_type	= INT32;
				astree -> entry -> paras	= NULL;
				astree -> entry -> addr		= NULL;
				break;
			default: break;
		}
	}

	free(code_buffer);
	return astree;
}

/*
 * Function		: build
 * Description	: source file -> assembly file.
 */
int build(const char *src_file, const char *tgt_file, bool asm_only)
{
	char	*src_code	= NULL;
	FILE	*src_main	= NULL;
	FILE	*ppro_file	= NULL;
	AST		astree		= NULL;
	size_t	src_length	= 0;

	header  headers     = NULL;

	if (!(src_main = fopen(src_file, "r")))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Couldn't read file \"%s\": ", src_file);
		perror("");
		return EXIT_FAILURE;
	}

	fseek(src_main, 0, SEEK_END);
	src_length = ftell(src_main);
	fseek(src_main, 0, SEEK_SET);

	if (!(src_code = (char *)malloc(src_length + 1)))
	{
		fclose(src_main);
		return EXIT_FAILURE;
	}

	fread(src_code, 1, src_length, src_main);
	fclose(src_main);
	src_code[src_length] = '\0';

	headers = preprocess(src_code, &ppro_file);
	if (!ppro_file)
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Preprocessing failed.\n");
		free(src_code);
		return EXIT_FAILURE;
	}

	astree = AST_generate(ppro_file);
	if (!astree)
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: AST generation failed.\n");
		fclose(ppro_file);
		free(src_code);
		return EXIT_FAILURE;
	}
	asm_build(src_file, tgt_file, astree);

	/* Clean up AST. */
	if (astree)
	{
		if (astree -> entry)
		{
			free(astree -> entry);
			astree -> entry = NULL;
		}
		free(astree);
	}

	fclose(ppro_file);
	free(src_code);

	/* Free header list returned by preprocess */
	if (headers)
	{
		header cur = headers;
		while (cur)
		{
			header nxt = cur->next;
			free(cur);
			cur = nxt;
		}
	}
	return EXIT_SUCCESS;
}
