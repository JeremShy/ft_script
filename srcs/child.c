#include <ft_script.h>

int	child(int pipe_to_read, t_opt *opt)
{
	char	sbuffer[11];
	int	fd;
	struct winsize w;
	struct termios old;
	char	shell[1024];

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
		ft_strncpy(shell, get_shell(opt->default_args.envp), sizeof(shell));
		fd = execve(shell, (char*[]){shell, NULL}, opt->default_args.envp);
	}
	else
	{
		if (opt->argv[0][0] == '/')
			ft_strncpy(shell, opt->argv[0], 1024);
		else if (!find_in_path(opt->argv[0], opt->default_args.envp, shell))
		{
			ft_putstr_fd(opt->argv[0], 2);
			ft_putstr_fd(": No such file or directory.\n", 2);
			return (0);
		}
		fd = execve(shell, opt->argv, opt->default_args.envp);
	}
	ft_putstr_fd(shell, 2);
	ft_putstr_fd(": Error while trying to exec this file.\n", 2);
	perror("");
	_exit(88);
	return (0);
}
