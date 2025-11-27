/*
 * File		: src/asmbuild.c
 * Date		: 2025.11.27 (Thus) 13:14
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/asmbuild.h>
#include <stdio.h>
#include <stdlib.h>

int asm_build(const char *src_file, const char *tgt_file, AST astree)
{
	FILE *target;
	if (!(target = fopen(tgt_file, "w")))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: Failed to access file \"%s\": ", tgt_file);
		perror("");
		return EXIT_FAILURE;
	}

	fprintf(target, "\t.file\t\"%s\"\n\t.text\n\t.globl %s\n\n\
%s:\n\tpushq\t%%rbp\n\
\tmovq\t%%rsp, %%rbp\n\n\
\tmovl\t%%edi, -4(%%rbp)\n\
\tmovl\t%%rsi, -16(%%rbp)\n\
\tmovl\t%%rdx, -24(%%rbp)\n\n", src_file, astree -> function -> name, astree -> function -> name);
	;
	fprintf(target, "\tmovl\t$0, %%eax\n\tpopq\t%%rbp\n\
\tret\n\n\t.size\t%s, .-%s\n.ident\t\"<Logic Compiler>\"",\
			astree -> function -> name, astree -> function -> name);

	fclose(target);
	return EXIT_SUCCESS;
}
