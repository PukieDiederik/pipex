#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void parent_process(int *p, char **envp)
{
	close(p[1]);
	dup2(*p, STDIN_FILENO);

	char str[] = "wc";
	char **ls = malloc(sizeof (char *));
	*ls = str;
	execve("/usr/bin/wc", ls, envp);
	free(ls);
	close(p[0]);
}

void child_process(int *p, char **envp)
{
	close(p[0]);
	dup2(p[1], STDOUT_FILENO);

	char str[] = "ls";
	char **ls = malloc(sizeof (char *));
	*ls = str;
	execve("/bin/ls", ls, envp);
	free(ls);
	close(p[1]);
}

int main(int argc, char **argv, char **envp)
{
	int p[2];
	pipe(p);
	int id = fork();
	if (id < 0)
		exit(1);
	else if (id == 0)
		child_process(p, envp);
	else {
		wait(0);
		parent_process(p, envp);
	}
	printf("argc: %d, name: %s\n", argc, *argv);
}
