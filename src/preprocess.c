/*
 * File		: src/preprocess.c
 * Date		: 2025.11.27 (Thus) 16:41
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/preprocess.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int preprocess(const char *src_code)
{
	header headers		= NULL;
	header new_header	= NULL;
	size_t line			= 0;
	size_t subpos		= 0;
	size_t total_length	= strlen(src_code);

	for (size_t pos = 0; pos < total_length; pos ++)
	{
		if (!strncmp(src_code + pos, "header", 6))
		{
			pos += 6;
			subpos = 0;
			if (!(new_header = (header)malloc(sizeof(_header))))
			{
				fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to allocate memory for header record: ");
				perror("");
				return EXIT_FAILURE;
			}

			while (src_code[pos] != ';')
			{
				if (pos >= MAX_PATH_LENGTH)
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %lu: <Pre-process> header file path length is too long.", line);
					free(new_header);
					return EXIT_FAILURE;
				}

				else if (pos >= total_length)
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> ';' missed in expression \"header ...;\"");
					free(new_header);
					return EXIT_FAILURE;
				}

				if (src_code[pos] == '\n')
				{
					pos ++;
					line ++;
				}

				else if (src_code[pos] != ' ' && src_code[pos] != '\t' && src_code[pos] != '\r')
					new_header -> path[subpos ++] = src_code[pos ++];
			}

			new_header -> next = headers;
			headers = new_header;
		}

		else if (!strncmp(src_code + pos, "//", 2))
		{
			pos += 2;
			while (src_code[pos] != '\n' && pos ++ < total_length);
		}

		else if (!strncmp(src_code + pos, "/*", 2))
		{
			pos += 2;
			while (strncmp(src_code + pos, "*/", 2) && pos < total_length)
			{
				if (src_code[pos] == '\n')	line ++;
				pos ++;
			}
			pos += 2;
		}

		else if (src_code[pos] == '\n')	line ++;
	}

	return 0;
}
