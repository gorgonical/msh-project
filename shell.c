#include "shell.h"

/**
 * shell.c - the interactable shell portion of my shell. heavily
 * inspired by cornell university's cs414 operating systems course
 * assignment
 */

int main(int argc, char *argv[])
{
		 char workDir[150];
		 char *prompt = malloc(7*sizeof(char));
		 char *origPrompt; // We need to rebuild the pwd bit each time.
		 // Simple defaults for prompt. Maybe consider using a struct to store the info?
		 char **history = malloc(1000*sizeof(char *));
		 int historyIndex = 0;
		 int specialFunc = 0;
		 
		 strcpy(prompt, "[msh]: ");
		 origPrompt = strdup(prompt);

		 struct cmdInfo *shellInfo = malloc(sizeof(struct cmdInfo));
		 init_info(shellInfo);
		 while(1) {
					
					strcpy(prompt, origPrompt);
					getcwd(workDir, sizeof(workDir));
					strcat(prompt, workDir);
					strcat(prompt, ": ");
					
					char *cmdLine = readline((const char *)prompt);
					if (strcmp(cmdLine, "") == 0) { // Skip empty lines
							 continue;
					}
					
					shellInfo = parse(cmdLine);
					addCommandToHistory(history, cmdLine, &historyIndex);
					free(cmdLine);

					executeBlock:
					specialFunc = isBuiltInCommand(shellInfo);
					if (specialFunc != -1) {
							 switch (specialFunc) {
							 case 2:
										printHistory(history, historyIndex);
										break;
							 case 3:
										runCommandFromHistory(shellInfo, history);
										goto executeBlock;
										// We need to 'restart' command execution, and a goto is the easiest way, having changed the shell string
							 default:
										executeBuiltIn(shellInfo);
							 }
					} else {
							 
							 pid_t childPid = fork();
							 pid_t wpid;
							 int status;
							 if (childPid == 0) {
										executeCommand(shellInfo);
										exit(0);
							 } else {
										if (0 == 1) { //line for background handling, currently empty
												 ;
										} else {
												 do {
															wpid = waitpid(childPid, &status, WUNTRACED);
												 } while (!WIFEXITED(status) && !WIFSIGNALED(status));
										}
							 }
					}
					//print_info(shellInfo); //for testing the parser only
		 }

		 free_info(shellInfo);
		 free(workDir);
		 free(origPrompt);
		 free(prompt);
}

int isBuiltInCommand(struct cmdInfo *cmd)
{
		 char *program = cmd->commandTokens[0];
		 if (strcmp(program, "cd") == 0) {
					return 0;
		 } else if (strcmp(program, "exit") == 0) {
					return 1;
		 } else if (strcmp(program, "history") == 0) {
					return 2;
		 } else if (program[0] == '!') {
					return 3;
		 }
		 return -1;
}

int executeBuiltIn(struct cmdInfo *cmd)
{
		 if (strcmp(cmd->commandTokens[0], "cd") == 0) {
					if (chdir(cmd->commandTokens[1]) != -1) {
							 return 0;
					} else {
							 printError(errno);
					}					 
		 } else if (strcmp(cmd->commandTokens[0], "exit") == 0) {
					exit(0);
		 }
		 return 1;
}

int executeCommand(struct cmdInfo *cmd)
{
		 char **cmdArgs = malloc(cmd->numArgs+2*sizeof(char *));
		 for (int i=0; i<=cmd->numArgs; i++) {
					cmdArgs[i]=cmd->commandTokens[i];
		 }
		 cmdArgs[cmd->numArgs+1] = NULL;
		 if (execvp(cmdArgs[0], cmdArgs) == -1) {
					printError(errno);
		 }
		 free(cmdArgs);
		 return 0;
}

void printError(int err)
{
		 switch (err) {
		 case EACCES:
					printf("Insufficient privileges to access directory\n"); break;
		 case ENOENT:
					printf("Directory does not exist\n"); break;
		 case EFAULT:
					printf("EFAULT error.\n"); break;
		 default:
					printf("Error number: %d", errno);
		 }
}

int addCommandToHistory(char **historyAddress, char *commandToStore, int *index) {
		 historyAddress[*index] = malloc(strlen(commandToStore)*sizeof(char));
		 strcpy(historyAddress[*index], commandToStore);
		 *index = *index+1;
		 return 0;
}

void printHistory(char **historyAddress, int index) {
		 for (int i=0; i<index; i++) {
					printf("%d: %s\n", i, historyAddress[i%1000]);
		 }
}

void runCommandFromHistory(struct cmdInfo *commandStruct, char **history) {
		 int numChars = strlen(commandStruct->commandTokens[0]);
		 char *indexAsString = malloc(sizeof(char)*numChars);
		 memcpy(indexAsString, commandStruct->commandTokens[0]+1, numChars-1); 
		 int index = atoi(indexAsString);

		 struct cmdInfo *tempCmd = malloc(sizeof(struct cmdInfo));
		 init_info(tempCmd);

		 
		 tempCmd = parse(history[index]);
		 *commandStruct = *tempCmd;
}
