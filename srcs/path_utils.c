/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drobert- <drobert-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 13:49:05 by drobert-          #+#    #+#             */
/*   Updated: 2022/05/18 13:49:47 by drobert-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include <stdlib.h>

void	clear_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

static char	*create_path_str(char *path, char *cmd)
{
	char	*str;
	int		i;

	i = 0;
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

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	paths = ft_split((*envp) + 5, ':');
	if (!paths || !cmd)
		return (0);
	i = 0;
	while (paths[i])
	{
		path = create_path_str(paths[i], cmd);
		if (path && !access(path, X_OK))
		{
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
