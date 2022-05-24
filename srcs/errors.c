/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drobert- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 12:12:49 by drobert-          #+#    #+#             */
/*   Updated: 2022/05/24 12:13:33 by drobert-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	ft_putstr_fd("\033[31mError: Wrong amount of arguments", 2);
	exit(1);
}
