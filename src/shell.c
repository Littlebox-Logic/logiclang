/*
 * File		: src/shell.c
 * Date		: 2025.11.27 (Thus) 10:59
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef _WIN32
#ifdef __APPLE__
#include <editline/readline.h>
#include <editline/history.h>
#else
#include <readline/readline.h>
#include <readline/history.h>
#endif
#endif

/*
 * Function		: logic_shell
 * Description	: interactive shell for Logic language.
 */
int logic_shell(void)
{
	#ifdef _WIN32
	char input[1024];
	#else
	char *input;
	using_history();
	#endif

	puts("\nCurrent Mode: SHELL\n");
	while (true)
	{
		#ifdef _WIN32
		printf("\033[;92m< Logic >\033[0m ");
		if (scanf("%[^\n]", input))	return 0;
		getchar();
		#else
		input = readline("\033[;92m< Logic >\033[0m ");
		if (!input)	return 0;
		if (*input)	add_history(input);
		#endif

		if (!strcmp(input, "exit()"))	return 0;
	}

	return EXIT_SUCCESS;
}
