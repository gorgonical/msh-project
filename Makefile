CFLAGS=
DEBUG=-g
CC=clang
LINKLIBS=-lreadline -lncurses

all: shell

parser: parser.c
	$(CC) $(DEBUG) $(LINKLIBS) parser.c -o parse.o

shell: shell.c parser.c
	$(CC) $(DEBUG) $(LINKLIBS) shell.c parser.c -o shell
