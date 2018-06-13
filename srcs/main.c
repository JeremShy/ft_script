
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
	struct winsize w;


	setsid();

	// write(pipe_to_read, "caca", 4);
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
	char	obuffer[5];
	char	ibuffer[5];
	int	flg1;
	int	flg2;

	struct termios	old;
	struct termios	new;


	if (open_ttys(mbuffer, sbuffer, &mfd) == 0)
		return (0);
	write(pipe_to_write, sbuffer, 10);
	close(pipe_to_write);
	flg1 = fcntl(mfd, F_GETFL, 0);
	fcntl(mfd, F_SETFL, flg1 | O_NONBLOCK);

	flg2 = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, flg2 | O_NONBLOCK);


	ioctl(0, TIOCGETA, &old);
	new = old;
	new.c_lflag &= ~ECHO;
	new.c_lflag &= ~ICANON;
	ioctl(0, TIOCSETA, &new);

	while(1)
	{
		while (1)
		{
			r = read(mfd, obuffer, 5);
			if (r == -1 && errno == EAGAIN)
				break;
			else if (r == -1)
				return (0);
			else if (r == 0)
				break;
			write(1, obuffer, r);
		}
		while (1)
		{
			r = read(0, ibuffer, 5);
			if (r == -1 && errno == EAGAIN)
				break;
			else if (r == -1)
				return (0);
			else if (r == 0)
				break;
			write(mfd, ibuffer, r);
		}
	}
	exit(0);
	return (1);
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	int	pid;
	int	pipes[2] = {0, 0};

	printf("isatty_main : %d\n", isatty(0));
	printf("isatty_main : %d\n", isatty(1));
	printf("isatty_main : %d\n", isatty(2));


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
