/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanjon- <amanjon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:50:28 by amanjon-          #+#    #+#             */
/*   Updated: 2023/09/08 10:45:37 by amanjon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	ft_free_father(t_process *process)
{
	int	i;

	i = 0;
	if (process->split_path != NULL)
	{
		while (process->split_path[i])
		{
			free(process->split_path[i]);
			i++;
		}
		free(process->split_path);
		close(process->infile);
		close(process->outfile);
	}
}

void	ft_free_childs(t_process *process)
{
	int	i;

	i = 0;
	while (process->cmd_argv[i])
	{
		free(process->cmd_argv[i]);
		i++;
	}
	free(process->cmd_argv);
	free(process->command);
}

char	*ft_get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (0);
}

void	ft_open_files(char **argv, t_process *process)
{
	process->infile = open(argv[1], O_RDONLY, 0644);
	if (access(argv[1], R_OK) < 0)
	{
		perror(argv[1]);
		exit(errno);
	}
	if (process->infile < 0)
	{
		perror("Error: don't open properly infile\n");
		exit(2);
	}
	process->outfile = open(argv[4], O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (access(argv[4], W_OK) < 0)
	{
		perror(argv[4]);
		exit(2);
	}
	if (process->outfile < 0)
	{
		perror("Error: don't open properly outfile\n");
		exit(2);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_process	process;

	if (argc != 5)
	{
		perror("Error: number of wrong arguments (5)\n");
		return (1);
	}
	process.split_path = NULL;
	ft_open_files(argv, &process);
	pipe(process.fd);
	process.path = ft_get_path(env);
	if (process.path != NULL)
		process.split_path = ft_split(process.path, ':');
	ft_commands_childs(process, argv, env);
	close(process.fd[WRITE]);
	close(process.fd[READ]);
	close(process.infile);
	close(process.outfile);
	waitpid(process.pid1, NULL, 0);
	waitpid(process.pid2, NULL, 0);
	ft_free_father(&process);
	return (0);
}
