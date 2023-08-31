/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanjon- <amanjon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:50:28 by amanjon-          #+#    #+#             */
/*   Updated: 2023/08/31 07:44:25 by amanjon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/* int	main(void)
{
	int fd[2];
	pid_t pidC;
	char buf[10];
	int num;
	
	if (pipe(fd) != 0)
		return (0);
	pidC = fork(); // crear hijo
	if (pidC == 0) //Hijo
	{
		close(fd[0]); //el hijo escribe
		write(fd[1], "abcde", 5);
		close(fd[1]);
		printf("ejecutándose el hijo\n");
		exit(0);
	}
	if (pidC == -1)
		printf("no se habrá creado un hijo\n");
	if (pidC != 0 && pidC != -1) //Padre
	{
		close(fd[1]); //el padre lee
		num = read(fd[0], buf, sizeof(buf)); //devuelve número de caracteres leídos
		printf("Padre lee %d bytes: %s\n", num, buf);
		close(fd[0]);
		printf("Se estará ejecutando el padre y ese pidC será del hijo\n");
	}	
	return (0);
}

int	main(void)
{
	pid_t pidC;
	int var;

	var = 22;
	printf("proceso PID = %d PADRE\n", getpid());
	pidC = fork();
	
	printf("proceso PID = %d, pidC = %d ejecutándose\n", getpid(), pidC);
	if (pidC > 0)
	{
		var = 44;
		printf("se esta ejecutando el proceso padre correctamente\n");
	}
	else if (pidC == 0)
	{
		var = 33;
		printf("Se está ejecutando el proceso hijo\n");
	}
	else
		printf("Error: el valor será -1 y no se habrá creado el hijo\n");
	while (1)
	{
		sleep(2); //dormir un par de segundos.
		printf("esto lo ejecute el padre y el hijo:\n");
		printf("proceso PID = %d, var = %d ejecutándose\n", getpid(), var);
	}
	return (0);
} */

/* int main(void) //programar  **** ls-l | wc > file.txt ****
{
	int fd1[2]; //escritura y lectura
	int fd2;	//.txt
	int status;
	int pid;
	
	pipe(fd1);	//comunica "ls" y "wc"
	pid = fork();	
	if (pid == 0)	//hijo1: ejecuta "ls-l"
	{
		char *argv[] = {"ls", "-l", NULL};
		char *env[] = {NULL};

		close(fd1[READ]);	//cerrar extremo no necesario
		
		dup2(fd1[WRITE], STDOUT_FILENO); //reedirige salida de "ls -l" al pipe
		close(fd1[WRITE]);
		
		execve("/bin/ls", argv, env); //uniendo "ls -l" al extremo del pipe
	}
	else		// PADRE
	{
		close(fd1[WRITE]);	//cerrar extremo no necesario del padre

		pid = fork();
		if (pid == 0)	//hijo2: ejecuta "wc"
		{
			char *argv[] = {"wc", NULL};
			char *env[] = {NULL};
		
			fd2 = open(FILE_NAME, O_WRONLY);	//abrir file.txt en solo escritura
			dup2(fd1[READ], STDIN_FILENO);		//reedirige la entrada del pipe a "wc"
			close(fd1[READ]);

			dup2(fd2, STDOUT_FILENO);
			
			execve("/usr/bin/wc", argv, env);
		}
		else	//PADRE
		{
				close(fd1[READ]);	//cerramos extremo no necesario
		}
	}
	wait(&status);	//wait para cada hijo
	wait(&status);
	
	return (0);
}  */

/* int main(void)	//programar  **** ls -l | grep u | wc -l **** (múltiples pipes) WhileTrueThenDream en cascada
{
	int		fd1[2];
	int		fd2[2];
	//int		status;
	int		pid;

	pipe(fd1);
	
	pid = fork();	//HIJO 1
	if (pid == 0)
	{
		char	*argv[] = {"ls", "-l", NULL};
		char	*env[] = {NULL};
		
		close(fd1[READ]);
		
		dup2(fd1[WRITE], STDOUT_FILENO);
		close(fd1[WRITE]);
		
		execve("/bin/ls", argv, env);
		//execlp("/bin/ls", "ls", "-l", NULL);
	}
	else
	{
		close(fd1[WRITE]);	//PADRE

		pipe(fd2);
		pid = fork();	//HIJO 2
		if (pid == 0)
		{
			char	*argv[] = {"grep", "u", NULL};
			char	*env[] = {NULL};

			close(fd2[READ]);
			
			dup2(fd1[READ], STDIN_FILENO);
			close(fd1[READ]);
			
			dup2(fd2[WRITE], STDOUT_FILENO);
			close(fd2[WRITE]);
			
			execve("usr/bin/grep", argv, env);
			//execlp("usr/bin/grep","grep", "u",NULL);
		}
		else
		{
			close(fd1[READ]);	//PADRE (cierro fd del dup2();)
			close(fd2[WRITE]);

			pid = fork();
			if(pid == 0)	//HIJO 3
			{
				char	*argv[] = {"wc", "-l", NULL};
				char	*env[] = {NULL};

				dup2(fd2[READ], STDIN_FILENO);
				close(fd2[READ]);

				execve("/usr/bin/wc", argv, env);
				//execlp("/usr/bin/wc","wc", "-l",NULL);
			}
			else
			{
				close(fd2[READ]); // PADRE cerrar extremo que falta
			}
		}
	}
		//wait(&status); //wait por cada hijo.
		//wait(&status);
		//wait(&status);
		waitpid(pid, NULL, 0); //waitpid por cada hijo.
		waitpid(pid, NULL, 0);
		waitpid(pid, NULL, 0);

	return (0);
} */

/* void	ft_error(char *word)
{
	write(2, word, ft_strlen(word));
	exit(1);
}

void	ft_close(int *fd)
{
	close(fd[WRITE]);
	close(fd[READ]);
}

int	main(int argc, char **argv , char **env )		//programar  **** ls -l | grep u | wc -l **** -EDU- sin cascada
{
	int 	fd1[2];
	int 	fd2[2];
	pid_t	pid1;
	pid_t	pid2;
	pid_t	pid3;
	int i;

	(void) argc;
	(void) argv;
	i = 0 ;
	//while (env[i])
	//{
	//	i++;
	//	printf("envp[%d] = %s\n", i, env[i]);
	//}
	pipe(fd1);
	pipe(fd2);

	pid1 = fork();
	if (pid1 == 0)
	{
		char	*argv[] = {"ls", "-l", NULL};
		char	*env[] = {NULL};
		
		dup2(fd1[WRITE], STDOUT_FILENO);
		ft_close(fd1);
		ft_close(fd2);

		execve("/bin/ls", argv, env);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		char	*argv[] = {"grep", "u", NULL};
		char	*env[] = {NULL};
			
		dup2(fd1[READ], STDIN_FILENO);
		dup2(fd2[WRITE], STDOUT_FILENO);
		ft_close(fd1);
		ft_close(fd2);
		
		execve("/usr/bin/grep", argv, env);
	}
	pid3 = fork();
	if (pid3 == 0)
	{
		char	*argv[] = {"wc", "-l", NULL};
		char	*env[] = {NULL};
				
		dup2(fd2[READ], STDIN_FILENO);
		ft_close(fd1);
		ft_close(fd2);

		execve("/usr/bin/wc", argv, env);
	}
		ft_close(fd1);
		ft_close(fd2);
		
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
		waitpid(pid3, NULL, 0);
	return (0);
} */

void	ft_free_father(t_process *process)
{
	int i;

	i = 0;
	while(process->split_path[i])
	{
		free(process->split_path[i]);
		i++;	
	}
	free(process->split_path);
	close(process->infile);
	close(process->outfile);
}

void	ft_free_childs(t_process *process)
{
	int	i;
	
	i  = 0;
	while(process->cmd_argv[i])
	{
		free(process->cmd_argv[i]);
		i++;
	}
	free(process->cmd_argv);
	free(process->command);
}

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
	while(path[i])
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

void	ft_comand_child1(t_process *process, char **argv, char **env)
{
	close(process->fd[READ]);
	dup2(process->fd[WRITE], STDOUT_FILENO);
	close(process->fd[WRITE]);
	dup2(process->infile, STDIN_FILENO);
	close(process->infile);

	process->cmd_argv = ft_split(argv[2], ' ');	//  "ls -l" ->  "ls"
	process->command = ft_get_command(process->split_path, process->cmd_argv[0]); //	"/usr/bin/ls"  ejemplo.
	if (!process->command)
	{
		ft_free_childs(process);
		perror("Error: comand not found child 1!\n");
		exit(127);
	}
	if (execve(process->command, process->cmd_argv, env) == -1)
	{
		perror("");
		exit(STDERR_FILENO);
	}
}

void	ft_comand_child2(t_process *process, char **argv, char **env)
{
	close(process->fd[WRITE]);
	dup2(process->fd[READ], STDIN_FILENO);
	close(process->fd[READ]);
	dup2(process->outfile, STDOUT_FILENO);
	close(process->outfile);

	process->cmd_argv = ft_split(argv[3], ' ');  //  "wc -l" -> "wc"
	process->command = ft_get_command(process->split_path, process->cmd_argv[0]); //	"/usr/bin/ls"  ejemplo.
	if (!process->command)
	{
		ft_free_childs(process);
		perror("Error: comand not found child 2!\n");
		exit(127);
	}
	if (execve(process->command, process->cmd_argv, env) == -1)
	{
		perror("");
		exit(STDERR_FILENO);
	}
}

char	*ft_get_path(char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (0);
}

void	ft_commands_childs(t_process process, char **argv, char **env)
{
	if (process.pid1 < 0)
	{
		perror("Error: fork()\n");
		exit(1);
	}
	else if (process.pid1 == 0)
		ft_comand_child1(&process, argv, env);
	if (process.pid2 < 0)
	{
		perror("Error: fork()\n");
		exit(1);
	}
	else if (process.pid2 == 0)
		ft_comand_child2(&process, argv, env);
}

void	ft_open_files(char **argv, t_process *process)
{
	process->infile = open(argv[1], O_RDONLY, 0644);
	if (access(argv[1], R_OK) < 0)
	{
		perror(argv[1]);
		exit(STDERR_FILENO);
	}
	if (process->infile < 0)
	{
		perror("Error: don't open properly infile");
		exit(1);
	}
	process->outfile = open(argv[4], O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (access(argv[4], W_OK | R_OK) < 0)
	{
		perror(argv[4]);
		exit(STDERR_FILENO);
	}
	if (process->outfile < 0)
	{
		perror("Error: don't open properly outfile");
		exit(1);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_process process;
	
	if (argc != 5)
		perror("Error: number of wrong arguments (5)");
	ft_open_files(argv, &process);
	pipe(process.fd);
	process.path = ft_get_path(env);
	process.split_path = ft_split(process.path, ':'); // /usr/bin
	process.pid1 = fork();
	process.pid2 = fork();
	ft_commands_childs(process, argv, env);
	
	close(process.fd[WRITE]);
	close(process.fd[READ]);

	waitpid(process.pid1, NULL, 0);
	waitpid(process.pid2, NULL, 0);
	
	ft_free_father(&process);
	return (0);
}
