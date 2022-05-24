#include "libft.h"
#include <stdio.h>

void	print_error(int errno, char *errstr)
{
	write(2, errstr, ft_strlen(errstr));
	exit(errno);
}

void	error(void)
{
	perror("\033[31mError");
	exit(1);
}

void	err_bad_args(void)
{
	ft_putstr_fd("\033[31mError: Wrong amount ofarguments", 2);
	exit(1);
}