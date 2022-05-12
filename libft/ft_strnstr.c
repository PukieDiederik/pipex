/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drobert- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 11:30:59 by drobert-          #+#    #+#             */
/*   Updated: 2022/03/02 10:39:48 by drobert-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	unsigned int	i;
	unsigned int	j;

	if (!(*needle))
		return ((char *)haystack);
	i = -1;
	while (++i < n && *(haystack + i))
	{
		j = 0;
		while (*(needle + j) && *(haystack + i + j) && i + j < n
			&& *(needle + j) == *(haystack + i + j))
			j++;
		if (*(needle + j) == 0)
			return ((char *)(haystack + i));
	}
	return (0);
}
