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

void	clear_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

void	execute(char *cmd, char **envp)
{
	char	**argv;
	char	*program;

	argv = ft_split(cmd, ' ');
	if (!argv)
		print_error(5, "Command splitting went wrong");
	program = get_path(argv[0], envp);
	if (!program)
		print_error(5, "Could not find/create path");
	execve(program, argv, envp);
	clear_split(argv);
	free(program);
	exit(2);
}

void	set_fds(int i, int argc, char **argv, int *p)
{
	int	fd;

	if (i == 2)
	{
		close(p[0]);
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
			print_error(6, "Could not open infile for reading");
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
			print_error(6, "Could not open outfile for reading");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	close(p[1]);
}

void	print_error(int errno, char *errstr)
{
	write(1, errstr, ft_strlen(errstr));
	exit(errno);
}

int	main(int argc, char **argv, char **envp)
{
	int	id;
	int	i;
	int	status;
	int	p[2];

	if (argc < 5)
		print_error(3, "Not enough arguments");
	if (access(argv[1], R_OK) | access(argv[argc - 1], R_OK))
		print_error(4, "Infile or outfile does not exist or cannot be read");
	i = 1;
	while (++i < argc - 1)
	{
		pipe(p);
		id = fork();
		if (id < 0)
			print_error(1, "Fork failed");
		if (id == 0)
		{
			set_fds(i, argc, argv, p);
			execute(argv[i], envp);
		}
		else
		{
			wait(&status);
			if (WIFEXITED(status) && WEXITSTATUS(status))
				print_error(2, "Program did not execute correctly");
			dup2(p[0], STDIN_FILENO);
			close(p[0]);
			close(p[1]);
		}
	}
}
