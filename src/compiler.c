/*
 * File		: src/compiler.c
 * Date		: 2025.11.27 (Thus) 10:36
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <stdio.h>
#include <stdlib.h>
#include <logic/compiler.h>
#include <logic/preprocess.h>
#include <logic/asmbuild.h>
#include <logic/AST.h>

int build(const char *src_file, const char *tgt_file, bool asm_only)
{
	FILE	*src_main;
	size_t	src_length;
	char	*src_code;

	if (!(src_main = fopen(src_file, "r")))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Couldn't read file \"%s\": ", src_file);
		perror("");
		return 0;
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

	return 0;
}
