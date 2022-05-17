#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include <wait.h>

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

int main(int argc, char **argv, char **envp)
{
	int id;
	int i = 1;

	if (argc < 4)
		exit(0);
	while (++i < argc - 1)
	{
		int p[2];
		pipe(p);
		char **child_argv = ft_split(argv[i], ' ');
		char *child_command = get_path(child_argv[0], envp);
		if (!child_command)
			exit(0);
		id = fork();
		if (id < 0)
			exit(1);
		if (id == 0)
		{
			if (i != argc - 2)
			{
				dup2(p[1], STDOUT_FILENO);
				close(p[1]);
			}
			else
				close(p[1]);
			execve(child_command, child_argv, envp);
		}
		else {
			waitpid(id, NULL, 0);
			dup2(p[0], STDIN_FILENO);
			close(p[0]);
			close(p[1]);
			clear_split(child_argv);
			free(child_command);
			fflush(stdout);
		}
	}
}
