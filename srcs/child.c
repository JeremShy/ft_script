#include <ft_script.h>

int	child(int pipe_to_read, t_opt *opt)
{
	char	sbuffer[11];
	int	fd;
	struct winsize w;
	struct termios old;
	char	*shell;

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

	if (opt->argv == NULL)
	{
		shell = get_shell(opt->default_args.envp);
		fd = execve(shell, (char*[]){shell, NULL}, opt->default_args.envp);
	}
	else
	{
		shell = opt->argv[0];
		fd = execve(opt->argv[0], opt->argv, opt->default_args.envp);
	}
	ft_putstr_fd(shell, 2);
	ft_putstr_fd(": Error while trying to exec this file.\n", 2);
	return (0);
}
