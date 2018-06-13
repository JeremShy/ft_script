
#include <ft_script.h>
#include <errno.h>
#include <termios.h>

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
	while ((*mfd = open(mbuffer, O_RDWR)) == -1)
	{
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
	struct winsize w;


	setsid();

	ioctl(0, TIOCGSIZE, &w);

	read(pipe_to_read, sbuffer, 10);
	close(pipe_to_read);
	fd = open(sbuffer, O_RDWR);


	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);

	ioctl(0, TIOCSSIZE, &w);

	execve("/bin/bash", (char*[]){"/bin/bash", NULL}, envp);
	exit(0);
}

int	parent(int pipe_to_write)
{
	int	r;
	int	mfd;

	char	mbuffer[11];
	char	sbuffer[11];
	char	obuffer[2048];
	char	ibuffer[2048];
	int	pid;

	struct termios	old;
	struct termios	new;


	if (open_ttys(mbuffer, sbuffer, &mfd) == 0)
		return (0);
	write(pipe_to_write, sbuffer, 10);
	close(pipe_to_write);

	ioctl(0, TIOCGETA, &old);
	new = old;
	new.c_lflag &= ~ECHO;
	new.c_lflag &= ~ICANON;
	ioctl(0, TIOCSETA, &new);

	pid = fork();
	if (pid == 0)
	{
		while (1)
		{
			r = read(mfd, obuffer, 2048);
			if (r == -1)
				exit (4);
			write(1, obuffer, r);
		}
		exit(1);
	}
	else
	{
		while (1)
		{
			r = read(0, ibuffer, 2048);
			if (r == -1)
				exit (5);
			write(mfd, ibuffer, r);
		}
		exit(2);
	}
	exit(3);
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
