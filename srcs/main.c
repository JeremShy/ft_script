#include <ft_script.h>

int	child(int pipe_to_read, char **envp)
{
	char	sbuffer[11];
	int	fd;
	struct winsize w;
	struct termios old;

	setsid();

	ioctl(0, TIOCGETA, &old);
	ioctl(0, TIOCGSIZE, &w);

	read(pipe_to_read, sbuffer, 10);
	close(pipe_to_read);
	fd = open(sbuffer, O_RDWR);

	ioctl(fd, TIOCSCTTY, 0);

	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);

	ioctl(0, TIOCSSIZE, &w);
	ioctl(0, TIOCSETA, &old);
	close(fd);

	return (execve("/bin/bash", (char*[]){"/bin/bash", NULL}, envp));
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

	ignore_signals();
	if (open_ttys(mbuffer, sbuffer, &mfd) == 0)
		return (0);
	write(pipe_to_write, sbuffer, 10);
	close(pipe_to_write);

	ioctl(0, TIOCGETA, &old);
	singelton_tty(&old);
	ft_memcpy(&new, &old, sizeof(new));
	new.c_lflag &= ~ECHO;
	new.c_lflag &= ~ICANON;
	new.c_lflag &= ~ISIG;
	ioctl(0, TIOCSETA, &new);

	pid = fork();
	int fd = open("/tmp/a", O_WRONLY | O_CREAT | O_TRUNC, 0444);
	if (pid == 0) // child2
	{
		while (1)
		{
			r = read(mfd, obuffer, 2048);
			if (r == -1)
			{
				ioctl(0, TIOCSETA, &old);
				_exit (4);
			}
			write(1, obuffer, r);
		}
	}
	else // Parent
	{
		handler(pid);
		while (1)
		{
			r = read(0, ibuffer, 2048);
			if (r == -1)
			{
				ioctl(0, TIOCSETA, &old);
				_exit (5);
			}
			write(fd, ibuffer, r);
			write(mfd, ibuffer, r);
		}
	}
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	int	pid;
	int	pipes[2];

	ft_bzero(&pipes, sizeof(pipes));
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
	else			// Child1
	{
		child(pipes[0], envp);
	}

	return (0);
}
