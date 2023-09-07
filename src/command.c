/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanjon- <amanjon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 12:27:09 by amanjon-          #+#    #+#             */
/*   Updated: 2023/09/07 11:03:22 by amanjon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	ft_check_command(char *cmd, char **argv)
{
	if (ft_strlen(&argv[2][0]) == 0 || ft_strlen(&argv[3][0]) == 0)
	{
		perror("Error: invalid arguments of commands both childs\n");
		exit(STDERR_FILENO);
	}
	if (cmd[0] == '.' || cmd[0] == '/')
	{							
		if (access(cmd, X_OK) == 0)
			return (1);
		else
		{
			perror(cmd);
			exit (STDERR_FILENO);
		}
	}
	return (0);
}

char	*ft_get_command(char **path, char *cmd, char **argv)
{
	char	*aux;
	char	*command;
	int		i;

	if (ft_check_command(cmd, argv) == 1)
		return (cmd);
	aux = 0;
	command = 0;
	i = 0;
	while (path[i])
	{
		aux = ft_strjoin(path[i], "/");
		command = ft_strjoin(aux, cmd);
		free(aux);
		if (access(command, F_OK & R_OK) == 0)
			return (command);
		free(command);
		i++;
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
			process.cmd_argv[0], argv);
	if (!process.command)
	{
		ft_free_childs(&process);
		perror("Error: comand not found child 2!\n");
		exit(STDERR_FILENO);
	}
	if (execve(process.command, process.cmd_argv, env) == -1)
	{
		perror("");
		exit(STDERR_FILENO);
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
			process.cmd_argv[0], argv);
	if (!process.command)
	{
		ft_free_childs(&process);
		perror("Error: comand not found child 1!\n");
		exit(127);
	}
	if (execve(process.command, process.cmd_argv, env) == -1)
	{
		perror("");
		exit(STDERR_FILENO);
	}
}

void	ft_commands_childs(t_process process, char **argv, char **env)
{
	process.pid1 = fork();
	if (process.pid1 < 0)
	{
		perror("Error: fork() pid1\n");
		exit(STDERR_FILENO);
	}
	else if (process.pid1 == 0)
		ft_comand_child1(process, argv, env);
	process.pid2 = fork();
	if (process.pid2 < 0)
	{
		perror("Error: fork()pid2\n");
		exit(STDERR_FILENO);
	}
	else if (process.pid2 == 0)
		ft_comand_child2(process, argv, env);
}
