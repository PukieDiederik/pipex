#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void parent_process(int *p)
{
	printf("PARENT fd: %d\n", *p);
}

void child_process(int *p)
{
	printf("CHILD fd: %d\n", *(p + 1));
}

int main(int argc, char **argv)
{
//	char str[] = "ls";
//	char **ls = malloc(sizeof (char *));
//	*ls = str;
//	execve("/bin/ls", ls, envp);

	int p[2];
	pipe(p);
	int id = fork();
	if (id < 0)
		exit(1);
	else if (id == 0)
		child_process(p);
	else
		parent_process(p);
	printf("argc: %d, name: %s\n", argc, *argv);
}
