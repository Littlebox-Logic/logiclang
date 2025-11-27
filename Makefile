# File		: Makefile
# Date		: 2025.11.26 (Wed) 16:42
# Author	: Logic
# Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)

ifeq ($(OS), Windows_NT)
	CC	= x86_64-w64-mingw32-gcc
else
	CC	= gcc
endif

LD		= ld
CFLAGS	= -c -std=gnu23 -Wall -O2 -I./include
TARGET	= bin/logicc
OBJS	= obj/main.o obj/compiler.o obj/shell.o

all:	bin/logicc

bin/logicc:	$(OBJS)
	$(CC) $(OBJS) -lreadline -o $@

obj/main.o:	src/main.c
	$(CC) $(CFLAGS) $< -o $@

obj/compiler.o:	src/compiler.c
	$(CC) $(CFLAGS) $< -o $@

obj/shell.o:	src/shell.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf obj/*
	rm -rf $(TARGET)
