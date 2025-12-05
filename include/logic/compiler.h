/*
 * File		: include/logic/compiler.h
 * Date		: 2025.11.27 (Thus) 10:11
 * Author	: Logic
 * Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)
 */

#ifndef LOGIC_COMPILER_H
#define LOGIC_COMPILER_H

#include <logic/AST.h>
#include <stdbool.h>
#include <stdio.h>

AST AST_generate(FILE *);
int build(const char *, const char *, bool);

#endif
