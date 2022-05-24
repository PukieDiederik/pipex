/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drobert- <drobert-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 13:55:33 by drobert-          #+#    #+#             */
/*   Updated: 2022/05/23 02:33:26 by drobert-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

//getting the paths
char	*get_path(char *cmd, char **envp);
void	clear_split(char **split);
void	print_error(int errno, char *errstr);
void	err_bad_args(void);
void	error(void);

#endif
