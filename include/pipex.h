/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanjon- <amanjon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 09:29:49 by amanjon-          #+#    #+#             */
/*   Updated: 2023/09/08 10:18:38 by amanjon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/* --- LIBRARIES --- */
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>

/* --- NEW --- */
# include <sys/types.h>
# include <sys/wait.h> 
# include <sys/stat.h>
# include <errno.h>

/* ---------------- PATH ---------------- */
# include "../libft/Libft/include/libft.h"
# include "../libft/ft_printf/include/ft_printf.h"
# include "../libft/Gnl/include/get_next_line.h"

/* --- STRUCT --- */
typedef struct s_process
{
	int		fd[2];
	int		infile;
	int		outfile;
	char	*path;
	char	**split_path;
	char	**cmd_argv;
	char	*command;
	pid_t	pid1;
	pid_t	pid2;
}	t_process;

/* --- MACROS --- */
# define READ       0
# define WRITE      1

void	ft_open_files(char **argv, t_process *process);
char	*ft_get_path(char **env);
void	ft_comand_child1(t_process process, char **argv, char **env);
void	ft_comand_child2(t_process process, char **argv, char **env);
char	*ft_get_command(char **path, char *cmd, char **argv, t_process process);
void	ft_free_childs(t_process *process);
void	ft_free_father(t_process *process);
int		ft_check_command(char *cmd, char **argv);
void	ft_commands_childs(t_process process, char **argv, char **env);

#endif