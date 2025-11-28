/*
 * File		: src/preprocess.c
 * Date		: 2025.11.27 (Thus) 16:41
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/preprocess.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * Function		: header_inc
 * Description	: header link table -> temp header file.
 */
int header_inc(const char *path, FILE *header_file)
{
	header_file = tmpfile();
	if (!header_file)
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to create temporary file for header records \"%s\": ", path);
		perror("");
		return EXIT_FAILURE;
	}
	; // ----------------------------------------------
	return EXIT_SUCCESS;
}

/*
 * Function		: preprocess
 * Description	: raw Logic source code -> control code.
 */
int preprocess(const char *src_code, FILE *pproc_file)
{
	header headers		= NULL;
	header new_header	= NULL;
	header tail_header	= NULL;
	size_t line			= 0;
	size_t line_offset	= 0;
	size_t subpos		= 0;
	size_t total_length	= strlen(src_code);

	if (!(pproc_file = tmpfile()))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to create temporary file for pre-processed code: ");
		perror("");
		return EXIT_FAILURE;
	}

	for (size_t pos = 0; pos < total_length; pos ++)
	{
		if (!strncmp(src_code + pos, "header", 6) && (pos == 0 || isspace((unsigned char)src_code[pos - 1])))
		{
			pos += 6;
			subpos = 0;
			if (!(new_header = (header)malloc(sizeof(_header))))
			{
				fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to allocate memory for header record: ");
				perror("");
				fclose(pproc_file);
				return EXIT_FAILURE;
			}

			new_header -> path[0] = '\0';
			new_header -> next = NULL;

			while (pos < total_length && isspace((unsigned char)src_code[pos]))	pos ++;
			if (pos >= total_length || src_code[pos] == '\n')
			{
				fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> Header missing path.\n", line, line_offset);
				free(new_header);
				fclose(pproc_file);
				return EXIT_FAILURE;
			}

			while (pos < total_length && src_code[pos] != ';')
			{
				if (subpos >= MAX_PATH_LENGTH - 1)
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> Header file path length is too long.", line, line_offset);
					free(new_header);
					fclose(pproc_file);
					return EXIT_FAILURE;
				}

				else if (pos >= total_length)
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> ';' missed in expression \"header ...;\".");
					free(new_header);
					fclose(pproc_file);
					return EXIT_FAILURE;
				}

				if (src_code[pos] == '\n')
				{
					line_offset = ++ pos;
					line ++;
				}

				else if (src_code[pos] == '\n')
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> Invalid newline within header path.\n", line, line_offset);
					free(new_header);
					fclose(pproc_file);
					return EXIT_FAILURE;
				}

				else if (src_code[pos] == ' ' || src_code[pos] == '\t' || src_code[pos] == '\r')
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> Invalid space inside header path.\n", line, line_offset);
					free(new_header);
					fclose(pproc_file);
					return EXIT_FAILURE;
				}

				else
				{
					if (isalnum((unsigned char)src_code[pos]) || src_code[pos] == '.' || src_code[pos] == '/' || src_code[pos] == '_' || src_code[pos] == '-')
						new_header -> path[subpos ++] = src_code[pos ++];
					else
					{
						fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> Invalid character '%c' in header path.\n", line, line_offset, src_code[pos]);
						free(new_header);
						fclose(pproc_file);
						return EXIT_FAILURE;
					}
				}
			}

			new_header -> path[subpos] = '\0';
			printf("Detected header: %s\n", new_header -> path);
			if (!headers)
			{
				tail_header	= new_header;
				headers		= new_header;
			}
			else
			{
				tail_header -> next = new_header;
				tail_header = new_header;
			}
		}

		else if (!strncmp(src_code + pos, "@entry", 6) && (pos == 0 || isspace((unsigned char)src_code[pos - 1])))
		{
			pos += 6;
			while (isspace((unsigned char)src_code[pos ++]))
			{
				line_offset ++;
				if (src_code[pos] == '\n')
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> '@entry' missing '::' after it.\n", line, line_offset);
					fclose(pproc_file);
					return EXIT_FAILURE;
				}
				else if (pos >= total_length)
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Unexpected end of file after '@entry'.\n");
					fclose(pproc_file);
					return EXIT_FAILURE;
				}
			}
			if (strncmp(src_code + pos - 1, "::", 2))
			{
				fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> '@entry' missing '::' after it.\n", line, line_offset);
				fclose(pproc_file);
				return EXIT_FAILURE;
			}

			pos += 2;
			while (pos < total_length && isspace((unsigned char)src_code[pos]))
			{
				pos ++;
				line_offset ++;
				if (src_code[pos] == '\n')
				{
					fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> '@entry ::' missing property after it.\n", line, line_offset);
					fclose(pproc_file);
					return EXIT_FAILURE;
				}
			}

			if (pos >= total_length)
			{
				fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Unexpected end of file after '@entry ::'.\n");
				fclose(pproc_file);
				return EXIT_FAILURE;
			}
		}

		else if (pos + 1 < total_length && !strncmp(src_code + pos, "//", 2))
		{
			pos += 2;
			while (pos < total_length && src_code[pos] != '\n')	pos ++;
		}

		else if (pos + 1 < total_length && !strncmp(src_code + pos, "/*", 2))
		{
			pos += 2;
			while (pos + 1 < total_length && strncmp(src_code + pos, "*/", 2))
			{
				if (src_code[pos] == '\n')
				{
					line_offset = pos + 1;
					line ++;
				}
				pos ++;
			}

			if (pos + 1 >= total_length)
			{
				fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> '*/' missed in block comment.\n");
				fclose(pproc_file);
				return EXIT_FAILURE;
			}
			pos += 2;
		}
	}

	return EXIT_SUCCESS;
}
