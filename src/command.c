/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanjon- <amanjon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 12:27:09 by amanjon-          #+#    #+#             */
/*   Updated: 2023/08/31 15:03:40 by amanjon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	ft_check_command(char *cmd)
{
	if (cmd[0] == '.' || cmd[0] == '/')
	{
		if (access(cmd, X_OK) == 0)
			return (0);
		else
		{
			perror(cmd);
			exit (127);
		}
	}
	return (1);
}

char	*ft_get_command(char **path, char *cmd)
{
	char	*aux;
	char	*command;
	int		i;

	if (ft_check_command(cmd) == 0)
		return (cmd);
	aux = 0;
	command = 0;
	i = 0;
	while (path[i])
	{
		aux = ft_strjoin(path[i], "/"); //	/usr/bin->  /usr/bin/
		command = ft_strjoin(aux, cmd);			//	/usr/bin/ls
		free(aux);
		if (access(command, F_OK & R_OK) == 0)
			return (command);
		free(command);
		i++;
	}
	return (NULL);
}

void	ft_comand_child1(t_process process, char **argv, char **env)
{
	close(process.fd[READ]);
	dup2(process.fd[WRITE], STDOUT_FILENO);
	close(process.fd[WRITE]);
	dup2(process.infile, STDIN_FILENO);
	close(process.infile);
	process.cmd_argv = ft_split(argv[2], ' ');//  "ls -l" ->  "ls"
	process.command = ft_get_command(process.split_path, process.cmd_argv[0]); //	"/usr/bin/ls"  ejemplo.
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

void	ft_comand_child2(t_process process, char **argv, char **env)
{
	close(process.fd[WRITE]);
	dup2(process.fd[READ], STDIN_FILENO);
	close(process.fd[READ]);
	dup2(process.outfile, STDOUT_FILENO);
	close(process.outfile);
	process.cmd_argv = ft_split(argv[3], ' ');  //  "wc -l" -> "wc"
	process.command = ft_get_command(process.split_path, process.cmd_argv[0]); //	"/usr/bin/ls"  ejemplo.
	if (!process.command)
	{
		ft_free_childs(&process);
		perror("Error: comand not found child 2!\n");
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
		perror("Error: fork()\n");
		exit(1);
	}
	else if (process.pid1 == 0)
		ft_comand_child1(process, argv, env);
	process.pid2 = fork();
	if (process.pid2 < 0)
	{
		perror("Error: fork()\n");
		exit(1);
	}
	else if (process.pid2 == 0)
		ft_comand_child2(process, argv, env);
}