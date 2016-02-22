#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <errno.h>
#include <sys/wait.h>

typedef int bool;
#define TRUE 1
#define FALSE 0

#define TOKENARRAYSIZE 50

struct cmdInfo
{
		 char *inpipe;
		 char *output;
		 bool background;
		 // char (*commandTokens)[50];
		 char **commandTokens;
		 int numArgs;		 
};

// Parser.c
void init_info(struct cmdInfo *c);
void free_info(struct cmdInfo *garbageInfo);
struct cmdInfo *parse(char *commandLine);
void print_info(struct cmdInfo *cmdInformation);

// Shell.c
bool isBuiltInCommand(struct cmdInfo *cmd);
int executeBuiltIn(struct cmdInfo *cmd);
void printError(int err);
int executeCommand(struct cmdInfo *cmd);
int addCommandToHistory(char **historyAddress, char *commandToStore, int *index);
void printHistory(char **historyAddress, int index);
