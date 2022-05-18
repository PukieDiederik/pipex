/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drobert- <drobert-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 13:55:00 by drobert-          #+#    #+#             */
/*   Updated: 2022/05/18 13:55:10 by drobert-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "pipex.h"
#include <sys/wait.h>
#include <sys/fcntl.h>

void	print_error(int errno, char *errstr);

void	execute(char *cmd, char **envp)
{
	char	**argv;
	char	*program;

	argv = ft_split(cmd, ' ');
	if (!argv || !argv[0])
		print_error(5, "Command splitting went wrong\n");
	program = get_path(argv[0], envp);
	if (!program)
		print_error(5, "Could not find/create path\n");
	execve(program, argv, envp);
	clear_split(argv);
	free(program);
	exit(2);
}

void	parent(int *p)
{
	int	status;

	wait(&status);
	if (WIFEXITED(status) && WEXITSTATUS(status))
		print_error(2, "Program did not execute correctly\n");
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
			print_error(6, "Could not open infile for reading\n");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (i != argc - 2)
	{
		dup2(p[1], STDOUT_FILENO);
	}
	else
	{
		fd = open (argv[argc - 1], O_WRONLY);
		if (fd < 0)
			print_error(6, "Could not open outfile for reading\n");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	close(p[1]);
}

void	print_error(int errno, char *errstr)
{
	write(2, errstr, ft_strlen(errstr));
	exit(errno);
}

int	main(int argc, char **argv, char **envp)
{
	int	id;
	int	i;
	int	p[2];

	if (argc < 5)
		print_error(3, "Not enough arguments\n");
	if (access(argv[1], R_OK) | access(argv[argc - 1], R_OK))
		print_error(4, "Infile or outfile does not exist or cannot be read\n");
	i = 1;
	while (++i < argc - 1)
	{
		pipe(p);
		id = fork();
		if (id < 0)
			print_error(1, "Fork failed\n");
		if (id == 0)
		{
			set_fds(i, argc, argv, p);
			execute(argv[i], envp);
		}
		else
			parent(p);
	}
}
