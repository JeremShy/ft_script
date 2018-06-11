#include <ft_script.h>

int	get_next_pty_name(char current[11])
{
	char	*letter;
	char	*nbr;

	 letter = current + 8;
	 nbr = current + 9;
	 if (*nbr != '9')
	 {
		 (*nbr)++;
		 return (1);
	 }
	 else if (*letter != 'z' && *letter != 'e')
	 {
		 *nbr = '0';
		 (*letter)++;
		 return (1);
	 }
	 else if (*letter == 'z')
	 {
		 *nbr = '0';
		 (*letter) = 'a';
		 return (1);
	 }
	 else
		 return (0);
}

int	open_ttys(char mbuffer[11], char sbuffer[11], int *mfd)
{
	ft_strcpy(mbuffer, "/dev/ptyp0");
	printf("Trying to open %s\n", mbuffer);
	while ((*mfd = open(mbuffer, O_RDWR)) == -1)
	{
		printf("Tried to open %s\n", mbuffer);
		if (get_next_pty_name(mbuffer) == 0)
			return (0);
	}
	ft_strcpy(sbuffer, mbuffer);
	sbuffer[5] = 't';
	return (1);
}

int	child(int pipe_to_read, char **envp)
{
	char	sbuffer[11];
	int	fd;

	setsid();
	// write(pipe_to_read, "caca", 4);
	read(pipe_to_read, sbuffer, 10);
	close(pipe_to_read);
	fd = open(sbuffer, O_RDWR);
	close(1);
	dup(fd);
	// printf("coucou\n");
	// dprintf(fd, "pouet\n");
	execve("/usr/bin/tty", (char*[]){"/bin/ls", NULL}, envp);
	// ioctl(-, TIOCSTART);
	// printf("mfd : %s\n", ttyname(mfd));
	// dprintf(sfd, "pouet! mfd = %d, sfd = %d\n", mfd, sfd);
	// dprintf(mfd, "pouet! mfd = %d, sfd = %d\n", mfd, sfd);
      //
	exit(0);
}

int	parent(int pipe_to_write)
{
	int	r;
	int	mfd;
	char	mbuffer[11];
	char	sbuffer[11];
	char	buffer[2048];

	if (open_ttys(mbuffer, sbuffer, &mfd) == 0)
		return (0);
	write(pipe_to_write, sbuffer, 10);
	close(pipe_to_write);
	// while(1)
	// {
		if ((r = read(mfd, buffer, 2047)) == -1)
			return (0);
		buffer[r] = '\0';
		printf("[%s]\n", buffer);
	// }
	exit(0);
	return (1);
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	int	pid;
	int	pipes[2] = {0, 0};

	if (pipe(pipes) == -1)
	{
		ft_putstr_fd("Error while piping.\n", 2);
		return (2);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("Error while forking.\n", 2);
		return (1);
	}

	if (pid != 0)	// Parent
	{
		parent(pipes[1]);
	}
	else			// Child
	{
		child(pipes[0], envp);
	}

	return (0);
}
