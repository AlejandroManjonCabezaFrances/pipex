/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_examples.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amanjon- <amanjon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 15:54:04 by amanjon-          #+#    #+#             */
/*   Updated: 2023/09/18 15:54:28 by amanjon-         ###   ########.fr       */
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