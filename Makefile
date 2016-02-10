CFLAGS=
DEBUG=-g
LINKLIBS=-lreadline -lcurses

all: shell

parser: parser.c
	$(CC) $(DEBUG) $(LINKLIBS) parser.c -o parse.o

shell: shell.c parser.c
	$(CC) $(DEBUG) $(LINKLIBS) shell.c parser.c -o shell
