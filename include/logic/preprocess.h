/*
 * File		: include/logic/preprocess.h
 * Date		: 2025.11.27 (Thus) 17:10
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#ifndef LOGIC_PREPROCESS_H
#define LOGIC_PREPROCESS_H

#define MAX_PATH_LENGTH 256

#include <stdio.h>

typedef struct _Header
{
	char path[MAX_PATH_LENGTH];
	struct _Header *next;
}	_header, *header;

int header_inc(const char *, FILE *);
int preprocess(const char *, FILE *);

#endif
