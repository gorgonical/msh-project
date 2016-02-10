#include "shell.h"

/**
 *  parse.c - the parsing component of my shell. inspired strongly by
 *  cornell university's cs414 operating systems course assignment
 */


/* initialize the struct */
void init_info(struct cmdInfo *c)
{
		 c->background = FALSE;
		 c->commandTokens = malloc(c->numArgs*sizeof(char *));
		 c->inpipe = NULL;
		 c->output = NULL;
}

/* parse commandLine for spaces */
struct cmdInfo *parse(char *commandLine)
{
		 int i=0;
		 int numSpc=0;
		 struct cmdInfo *info = malloc(sizeof(struct cmdInfo));

		 // Count the tokens we will need
		 for (i=0; commandLine[i] != '\0'; i++) {
					if (commandLine[i] == ' ')
							 numSpc++;
		 }

		 // We have the number of tokens, initialize the space for them.
		 info->numArgs = numSpc;	
		 init_info(info);

		 // Tokenize the string through spaces.
		 i=0;
		 char commandDup[(int)strlen(commandLine)];
		 strcpy(commandDup, commandLine); // Needed because string constancy with strtok
		 char *token = strtok(commandDup, " "); // The strtok buffer
		 while (token) {
					//info->commandTokens[i] = (char*)malloc(50*sizeof(char)); //initialize this token's array
					strcpy (info->commandTokens[i++], token);
					if (strcmp(token, "<")==0) {
							 info->inpipe = malloc(50*sizeof(char));
							 token = strtok( NULL, " ");
							 strcpy (info->commandTokens[i++], token);
							 strcpy (info->inpipe, token);
					}
					if (strcmp(token, ">")==0) {
							 info->output = malloc(50*sizeof(char));
							 token = strtok( NULL, " ");
							 strcpy (info->commandTokens[i++], token);
							 strcpy (info->output, token);
					}
					token = strtok( NULL, " ");
		 }

		 // "Is this a backgrounded program?" check
		 if (strcmp(info->commandTokens[numSpc], "&")==0)
					info->background = TRUE;
	
		 return info;
}

/* prints the results of the parse */
void print_info(struct cmdInfo *cmdInformation)
{
		 printf("Program name: %s\n", cmdInformation->commandTokens[0]);
		 int i=0;
		 int argNum=1;
		 for (i=1; i<=cmdInformation->numArgs; i++) {
					if (strcmp(cmdInformation->commandTokens[i], "&")!=0 &&
							strcmp(cmdInformation->commandTokens[i], ">")!=0 &&
							strcmp(cmdInformation->commandTokens[i], "<")!=0)
							 printf("arg%d: %s\n", argNum++, cmdInformation->commandTokens[i]);		
		 }
		 if (cmdInformation->inpipe != NULL) {
					printf("inpipe: %s\n", cmdInformation->inpipe);
		 }
		 else {
					printf("inpipe: no\n");
		 }
		 if (cmdInformation->output != NULL) {
					printf("output: %s\n", cmdInformation->output);
		 }
		 else {
					printf("output: no\n");
		 }
		 printf("background: ");
		 switch (cmdInformation->background) {
		 case TRUE:
					printf("yes\n");
					break;
		 case FALSE:
					printf("no\n");
					break;
		 }
}

/* frees the memory taken by an info struct */
void free_info(struct cmdInfo *garbageInfo)
{
		 free(garbageInfo);
}

/* int main(int argc, char *argvp[]) { */
/* 	char * cmd = "ls > butt &"; */
/* 	struct cmdInfo *info = parse(cmd); */
/* 	print_info(info); */
/* 	return 0; */
/* } */
