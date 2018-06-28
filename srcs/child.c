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
