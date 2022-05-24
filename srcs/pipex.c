/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drobert- <drobert-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 13:55:00 by drobert-          #+#    #+#             */
/*   Updated: 2022/05/23 13:58:18 by drobert-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "pipex.h"
#include <sys/wait.h>
#include <sys/fcntl.h>

void	execute(char *cmd, char **envp)
{
	char	**argv;
	char	*program;

	argv = ft_split(cmd, ' ');
	if (!argv || !argv[0])
		error();
	program = get_path(argv[0], envp);
	if (!program)
		print_error(5, "\033[31mCommand not found\n");
	execve(program, argv, envp);
	clear_split(argv);
	free(program);
	exit(2);
}

void	parent(int *p)
{
	dup2(p[0], STDIN_FILENO);
	close(p[0]);
	close(p[1]);
}

void	set_fds(int i, int argc, char **argv, int *p)
{
	int	fd;

	if (i == 2)
	{
		close(p[0]);
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
			error();
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (i != argc - 2)
	{
		dup2(p[1], STDOUT_FILENO);
	}
	else
	{
		fd = open (argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd < 0)
			error();
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	close(p[1]);
}

int	main(int argc, char **argv, char **envp)
{
	int	id;
	int	i;
	int	p[2];

	if (argc < 5)
		err_bad_args();
	if (access(argv[1], R_OK))
		error();
	i = 1;
	while (++i < argc - 1)
	{
		pipe(p);
		id = fork();
		if (id < 0)
			error();
		if (id == 0)
		{
			set_fds(i, argc, argv, p);
			execute(argv[i], envp);
		}
		else
			parent(p);
	}
}
