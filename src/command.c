/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanjon- <amanjon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 12:27:09 by amanjon-          #+#    #+#             */
/*   Updated: 2023/09/08 10:45:11 by amanjon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	ft_check_command(char *cmd, char **argv)
{
	if (ft_strlen(&argv[2][0]) == 0 || ft_strlen(&argv[3][0]) == 0)
	{
		perror("Error: invalid arguments of commands both childs\n");
		exit(1);
	}
	if (cmd[0] == '.' || cmd[0] == '/')
	{							
		if (access(cmd, F_OK & R_OK & X_OK) == 0)
			return (1);
		else
		{
			perror(cmd);
			exit (1);
		}
	}
	return (0);
}

char	*ft_get_command(char **path, char *cmd, char **argv, t_process process)
{
	char	*aux;
	char	*command;
	int		i;

	aux = 0;
	command = 0;
	i = 0;
	if (ft_check_command(cmd, argv) == 1)
		return (cmd);
	if (process.split_path != NULL)
	{
		while (path[i])
		{
			aux = ft_strjoin(path[i], "/");
			command = ft_strjoin(aux, cmd);
			free(aux);
			if (access(command, F_OK & R_OK & X_OK) == 0)
				return (command);
			free(command);
			i++;
		}
	}
	return (NULL);
}

void	ft_comand_child2(t_process process, char **argv, char **env)
{
	close(process.fd[WRITE]);
	dup2(process.fd[READ], STDIN_FILENO);
	close(process.fd[READ]);
	dup2(process.outfile, STDOUT_FILENO);
	close(process.outfile);
	process.cmd_argv = ft_split(argv[3], ' ');
	process.command = ft_get_command(process.split_path,
			process.cmd_argv[0], argv, process);
	if (!process.command)
	{
		ft_free_childs(&process);
		perror("Error: comand not found child 2!\n");
		exit(127);
	}
	if (execve(process.command, process.cmd_argv, env) == -1)
	{
		perror("Error function execve");
		exit(1);
	}
}

void	ft_comand_child1(t_process process, char **argv, char **env)
{
	dup2(process.infile, STDIN_FILENO);
	close(process.infile);
	close(process.fd[READ]);
	dup2(process.fd[WRITE], STDOUT_FILENO);
	close(process.fd[WRITE]);
	process.cmd_argv = ft_split(argv[2], ' ');
	process.command = ft_get_command(process.split_path,
			process.cmd_argv[0], argv, process);
	if (!process.command)
	{
		ft_free_childs(&process);
		perror("Error: comand not found child 1!\n");
		exit(127);
	}
	if (execve(process.command, process.cmd_argv, env) == -1)
	{
		perror("Error function execve");
		exit(1);
	}
}

void	ft_commands_childs(t_process process, char **argv, char **env)
{
	process.pid1 = fork();
	if (process.pid1 < 0)
	{
		perror("Error: fork() pid1\n");
		exit(1);
	}
	else if (process.pid1 == 0)
		ft_comand_child1(process, argv, env);
	process.pid2 = fork();
	if (process.pid2 < 0)
	{
		perror("Error: fork()pid2\n");
		exit(1);
	}
	else if (process.pid2 == 0)
		ft_comand_child2(process, argv, env);
}
