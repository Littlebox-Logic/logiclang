/*
 * File		: src/compiler.c
 * Date		: 2025.11.27 (Thus) 10:36
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <stdio.h>
#include <logic/compiler.h>

int compile(const char *src_file, const char *tgt_file, bool asm_only)
{
	FILE *src_main;
	if (!(src_main = fopen(src_file, "r")))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Couldn't read file \"%s\": ", src_file);
		perror("\0");
		return 0;
	}
	return 0;
}
