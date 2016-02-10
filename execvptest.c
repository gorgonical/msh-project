#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main()
{
	char *args[] = {"ls", "/home/nmg", NULL};
	execvp(args[0], args);
	return 0;
}
