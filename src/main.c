/*
 * File		: main.c
 * Date		: 2025.11.26 (Wed) 16:52
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <io.h>
#define access _access
#else
#include <unistd.h>
#endif

//#include "shell.h"
//#include "compiler.h"

#define VERSION "Pre-build 0.1"

#if defined(__GNUC__) && !defined(__clang__)
#define VER_INFO() printf("Logic compiler (version: %s)\n    Build info: [%s %s] GCC %d.%d.%d\n", VERSION, __DATE__, __TIME__, __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(__clang__)
#define VER_INFO() printf("Logic compiler (version: %s)\n    Build info: [%s %s] Clang %d.%d.%d\n", VERSION, __DATE__, __TIME__, __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(_MSC_VER)
#define VER_INFO() printf("Logic compiler (version: %s)\n    Build info: [%s %s] MSC %d\n", VERSION, __DATE__, __TIME__, _MSC_VER);
#elif defined(__INTEL_COMPILER)
#define VER_INFO() printf("Logic compiler (version: %s)\n    Build info: [%s %s] Intel C++ %d\n", VERSION, __DATE__, __TIME__, __INTEL_COMPILER);
#else
#define VER_INFO() printf("Logic compiler (version: %s)\n    Build info: [%s %s] Unknown compiler\n", VERSION, __DATE__, __TIME__);
#endif

int main(int argc, char *argv[], char **envp)
{
	size_t	tgt_file = 0;
	size_t	src_file = 0;
	bool	asm_only = false;

	if (argc == 1)
	{
		//logic_shell();
		return 0;
	}

	for (size_t arg_index = 1; arg_index < argc; arg_index ++)
	{
		if		(!strcmp(argv[arg_index], "-h") || !strcmp(argv[arg_index], "--help"))		goto HELP_INFO;
		else if	(!strcmp(argv[arg_index], "-v") || !strcmp(argv[arg_index], "--version"))	goto VERN_INFO;
		else if	(!strcmp(argv[arg_index], "-o") || !strcmp(argv[arg_index], "--output"))	tgt_file = ++ arg_index;
		else if (!strcmp(argv[arg_index], "-s") || !strcmp(argv[arg_index], "--asm"))		asm_only = true;
		else	src_file = arg_index;
	}

	if (!src_file)
	{
		fprintf(stderr, "logicc: No source file INPUT.\n");
		return 0;
	}

	if (access(argv[src_file], 4))
	{
		fprintf(stderr, "logicc: Source file \"%s\" cannot be read.\n", argv[src_file]);
		return 0;
	}

	//compile(argv[src_file], tgt_file ? argv[tgt_file] : "a.out", asm_only);
	return 0;

VERN_INFO:
	VER_INFO();
	return 0;
HELP_INFO:
	VER_INFO();
	printf("\nUsage:\n    logicc [Args] <source file>\n\nArgs:\n    -h/--help\t\t: Get HELP infomation.\n    -v/--version\t: Get VERSION infomation.\n    -o/--output <file>\t: Set OUTPUT FILE (default: a.out)\n    -s/--asm\t\t: Output ASSEMBLY source code only.\n");
	return 0;
}
