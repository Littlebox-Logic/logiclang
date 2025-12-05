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
#include <stdint.h>

typedef struct _Header_Node
{
	char path[MAX_PATH_LENGTH];
	struct _Header_Node *next;
}	_Header, *Header;

FILE *header_inc(const char *);
FILE *preprocess(const char *);

#endif
