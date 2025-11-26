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
CFLAGS	= -c -std=gnu23 -Wall -O2
TARGET	= bin/logicc

all:	bin/logicc

bin/logicc:	obj/main.o
	$(CC) $< -o $@

obj/main.o:	src/main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf obj/*
	rm -rf $(TARGET)
