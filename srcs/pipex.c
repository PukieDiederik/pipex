#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"

void clear_split(char **split)
{
	int i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

char *create_path_str(char *path, char *cmd)
{
	char *str;
	int i = 0;

	str = malloc((ft_strlen(path) + ft_strlen(cmd) + 2) * sizeof(char));
	if (!str)
		return (0);
	while (*path)
		str[i++] = *path++;
	str[i++] = '/';
	while (*cmd)
		str[i++] = *cmd++;
	str[i] = 0;
	return (str);
}

char *get_path(char *cmd, char **envp)
{
	char **paths;
	int i;
	char *path;

	while(*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	paths = ft_split((*envp) + 5, ':');
	i = 0;
	while (paths[i] != 0)
	{
		path = create_path_str(paths[i], cmd);
		if(!access(path, F_OK))
		{
			i = 0;
			clear_split(paths);
			return (path);
		}
		free(path);
		i++;
	}
	i = 0;
	clear_split(paths);
	return (0);
}

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

	if (argc != 5)
		exit(0);
	printf("%s", get_path(argv[2], envp));
//	int id = fork();
//	if (id < 0)
//		exit(1);
//	else if (id == 0)
//		child_process(p, envp);
//	else {
//		wait(0);
//		parent_process(p, envp);
//	}
}
