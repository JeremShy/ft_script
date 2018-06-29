#include <ft_script.h>

int		parent(int pipe_to_write, t_opt *opt, int child_pid)
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

	int		fd;

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

	fd = open(opt->output_file, opt->open_flags | O_CLOEXEC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd(opt->output_file, 2);
		ft_putstr_fd(": Can't open file.\n", 2);
		reset_terminal();
		kill(child_pid, SIGKILL);
		_exit(1);
	}

	if (!(opt->options & Q_OPT))
	{
		output_file_singelton(fd);
		write_started_message("started", opt->output_file);
		write_time(fd, "started");
		if (opt->argv)
			write_command(fd, opt->argv);
	}
	pid = fork();
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
			if (opt->options & K_OPT)
				write(fd, ibuffer, r);
			write(mfd, ibuffer, r);
		}
	}
}
