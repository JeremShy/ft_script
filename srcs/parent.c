#include <ft_script.h>

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
			write(fd, obuffer, r);
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
			write(mfd, ibuffer, r);
		}
	}
}
