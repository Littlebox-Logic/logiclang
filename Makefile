# File		: Makefile
# Date		: 2025.11.26 (Wed) 16:42
# Author	: Logic
# Project	: logiclang (https://github.com/Littlebox-Logic/logiclang)

ifeq ($(OS), Windows_NT)
	CC		= x86_64-w64-mingw32-gcc
	TARGET	= bin/logicc.exe
	OBJ_EXT	= .win.o
	LDFLAGS	=
else ifeq ($(shell uname), Darwin)
	CC		= gcc
	TARGET	= bin/logicc
	OBJ_EXT	= .mac.o
	LDFLAGS	= -ledit
else
	CC		= gcc
	TARGET	= bin/logicc
	OBJ_EXT	= .o
	LDFLAGS	= -lreadline
endif

LD			= ld
DEBUG		?= false
SRCS		= main compiler shell asmbuild preprocess lexer

ifeq ($(DEBUG), false)
	DEBUG_FLAGS =
else
	DEBUG_FLAGS = -g -DDEBUG
endif

CFLAGS		= -c -std=gnu23 -fshort-enums -Wall -O2 -I./include $(DEBUG_FLAGS)
INCS		= include/logic/asmbuild.h include/logic/AST.h include/logic/compiler.h include/logic/preprocess.h include/logic/shell.h include/logic/lexer.h
OBJS		= $(addprefix obj/, $(addsuffix $(OBJ_EXT), $(SRCS)))

all:	$(TARGET)

$(TARGET): $(OBJS) $(INCS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

obj/%$(OBJ_EXT):	src/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf obj/*
	rm -rf $(TARGET)
