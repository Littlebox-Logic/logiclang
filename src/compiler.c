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

int AST_generate(const char *code, AST_Root astree)
{
	if (!(astree = (AST_Root)malloc(sizeof(_AST_Root))))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Failed to allocate memory for AST root node: ");
		perror("");
		return EXIT_FAILURE;
	}
	astree -> entry = NULL;
	astree -> variable = NULL;
	astree -> next[0] = NULL;

	for (size_t index = 0; index < strlen(code); index ++)
	{
		switch (code[index])
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
					return EXIT_FAILURE;
				}
				strcpy(astree -> entry -> name, "main");
				astree -> entry -> rtn_type = INT32;
				astree -> entry -> paras = NULL;
				astree -> entry -> addr = NULL;
				break;
			default: break;
		}
	}

	return EXIT_SUCCESS;
}

int build(const char *src_file, const char *tgt_file, bool asm_only)
{
	FILE	*src_main;
	size_t	src_length;
	char	*src_code;

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

	preprocess(src_code);
	// asm_build(src_file, tgt_file, NULL);

	return EXIT_SUCCESS;
}
