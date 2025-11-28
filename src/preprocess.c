/*
 * File		: src/preprocess.c
 * Date		: 2025.11.27 (Thus) 16:41
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#include <logic/preprocess.h>
#include <logic/AST.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

/* helper: skip spaces and update pos/line/col */
static void skip_spaces(const char *src, size_t total, size_t *pos, size_t *line, size_t *col)
{
	while (*pos < total && isspace((unsigned char)src[*pos]))
	{
		if (src[*pos] == '\n')
		{
			(*pos) ++;
			(*line) ++;
			*col = 1;
		}
		else
		{
			(*pos) ++;
			(*col) ++;
		}
	}
}

static FILE *preproc_error(FILE *pproc_file, size_t line, size_t col, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "logicc: \033[;91mERROR\033[0m: ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	if (pproc_file) fclose(pproc_file);
	return NULL;
}

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
	header headers		= NULL;
	header new_header	= NULL;
	header tail_header	= NULL;
	size_t line			= 1;
	size_t col			= 1;
	size_t subpos		= 0;
	size_t total_length	= strlen(src_code);
	FILE *pproc_file = NULL;

	if (!(pproc_file = tmpfile()))
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to create temporary file for pre-processed code: ");
		perror("");
		return NULL;
	}

	for (size_t pos = 0; pos < total_length; pos ++)
	{
		pos == 0 ? (col = 1) : col ++;
		if (!strncmp(src_code + pos, "header", 6) && (pos == 0 || isspace((unsigned char)src_code[pos - 1])))
		{
			pos += 6;
			subpos = 0;
			if (!(new_header = (header)malloc(sizeof(_header))))
			{
				fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to allocate memory for header record: ");
				perror("");
				fclose(pproc_file);
				return NULL;
			}

			new_header -> path[0] = '\0';
			new_header -> next = NULL;

			while (pos < total_length && isspace((unsigned char)src_code[pos]))
			{
				pos ++;
				col ++;
			}

			if (pos >= total_length || src_code[pos] == '\n')
			{
				free(new_header);
				return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> Header missing path.\n", line, col);
			}

			while (pos < total_length && src_code[pos] != ';')
			{
				if (subpos >= MAX_PATH_LENGTH - 1)
				{
					free(new_header);
					return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> Header file path length is too long.", line, col);
				}

				else if (pos >= total_length)
				{
					free(new_header);
					return preproc_error(pproc_file, line, col, "<Pre-process> ';' missed in expression \"header ...;\".");
				}

				if (src_code[pos] == '\n')
				{
					pos  ++;
					line ++;
					col = 1;
					continue;
				}

				else if (src_code[pos] == '\n')
				{
					free(new_header);
					return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> Invalid newline within header path.\n", line, col);
				}

				else if (src_code[pos] == ' ' || src_code[pos] == '\t' || src_code[pos] == '\r')
				{
					free(new_header);
					return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> Invalid space inside header path.\n", line, col);
				}

				else
				{
					if (isalnum((unsigned char)src_code[pos]) || src_code[pos] == '.' || src_code[pos] == '/' || src_code[pos] == '_' || src_code[pos] == '-')
						new_header -> path[subpos ++] = src_code[pos ++];
					else
					{
						fprintf(stderr, "logicc: \033[;91mERROR\033[0m: line %zu, col: %zu: <Pre-process> Invalid character '%c' in header path.\n", line, col, src_code[pos]);
						free(new_header);
						fclose(pproc_file);
						return NULL;
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
			skip_spaces(src_code, total_length, &pos, &line, &col);
			if (pos + 1 >= total_length || src_code[pos] != ':' || src_code[pos + 1] != ':')
				return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> '@entry' missing '::' after it.\n", line, col);

			pos += 2; col += 2;

			char prop_name[256] = {'\0'};
			char prop_args[1024] = {'\0'};
			size_t pn_idx = 0, pa_idx = 0;

			skip_spaces(src_code, total_length, &pos, &line, &col);
			if (pos >= total_length || src_code[pos] == '\n')
				return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> '@entry ::' missing property after it.\n", line, col);

			/* Parse property name: [A-Za-z0-9_\-.]+ */
			while (pos < total_length && (isalnum((unsigned char)src_code[pos]) || src_code[pos] == '_' || src_code[pos] == '-' || src_code[pos] == '.'))
			{
				if (pn_idx + 1 >= sizeof(prop_name))
					return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> Function's property too long.\n", line, col);
				prop_name[pn_idx ++] = src_code[pos ++];
				col ++;
			}
			prop_name[pn_idx] = '\0';

			skip_spaces(src_code, total_length, &pos, &line, &col);
			if (pos >= total_length || src_code[pos] != '(')
				return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> '@entry' missing '(' after property.\n", line, col);

			/* Consume '('. */
			pos ++; col ++;

			/* Parse args until ')'. */
			while (pos < total_length && src_code[pos] != ')')
			{
				if (src_code[pos] == '\n')
					return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> Invalid newline inside property argument.\n", line, col);


				if (pa_idx + 1 >= sizeof(prop_args))
					return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> Property argument too long.\n", line, col);

				prop_args[pa_idx ++] = src_code[pos ++];
				col ++;
			}
			prop_args[pa_idx] = '\0';
			if (pos >= total_length || src_code[pos] != ')')
			{
				return preproc_error(pproc_file, line, col, "line %zu, col: %zu: <Pre-process> ')' missing after property argument.\n", line, col);
			}
			pos ++; col ++;

			/* Skip trailing whitespace and optional ';'. */
			skip_spaces(src_code, total_length, &pos, &line, &col);
			if (pos < total_length && src_code[pos] == ';') { pos ++; col ++; }

			printf("Detected @entry: prop: \"%s\", arg: \"%s\"\n", prop_name, prop_args);
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
					pos  ++;
					line ++;
					col = 1;
				}
				pos ++;
			}

			if (pos + 1 >= total_length)	return preproc_error(pproc_file, line, col, "<Pre-process> '*/' missed in block comment.\n");
			pos += 2;
		}
	}

	/* free headers linked list before returning */
	header cur = headers;
	while (cur)
	{
		header nxt = cur -> next;
		free(cur);
		cur = nxt;
	}

	if (fwrite(src_code, 1, total_length, pproc_file) != total_length)
	{
		fprintf(stderr, "logicc: \033[;91mERROR\033[0m: <Pre-process> Failed to write to preprocessed temp file: ");
		perror("");
		fclose(pproc_file);
		return NULL;
	}

	rewind(pproc_file);
	return pproc_file;
}
