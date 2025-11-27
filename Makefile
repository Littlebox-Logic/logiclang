# File		: Makefile
# Date		: 2025.11.26 (Wed) 16:42
# Author	: Logic
# Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)

ifeq ($(OS), Windows_NT)
	CC		= x86_64-w64-mingw32-gcc
	LDFLAGS	=
else
	CC		= gcc
	LDFLAGS	= -lreadline
endif

LD		= ld
DEBUG	?= false

ifeq ($(DEBUG), false)
	CFLAGS = -c -std=gnu23 -fshort-enums -Wall -O2 -I./include
else
	CFLAGS = -c -std=gnu23 -fshort-enums -Wall -O2 -I./include -g
endif

TARGET	= bin/logicc
OBJS	= obj/main.o obj/compiler.o obj/shell.o obj/asmbuild.o obj/preprocess.o
INCS	= include/logic/asmbuild.h include/logic/AST.h include/logic/compiler.h include/logic/preprocess.h include/logic/shell.h

all:	bin/logicc

bin/logicc:			$(OBJS) $(INCS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

obj/main.o:			src/main.c
	$(CC) $(CFLAGS) $< -o $@

obj/compiler.o:		src/compiler.c
	$(CC) $(CFLAGS) $< -o $@

obj/shell.o:		src/shell.c
	$(CC) $(CFLAGS) $< -o $@

obj/asmbuild.o:		src/asmbuild.c
	$(CC) $(CFLAGS) $< -o $@

obj/preprocess.o:	src/preprocess.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf obj/*
	rm -rf $(TARGET)
