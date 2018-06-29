#include <ft_script.h>

void	command_user_multiplex(int mfd, int fd, struct termios *old)
{
	char	obuffer[2048];
	int	r;

	while (1)
	{
		r = read(mfd, obuffer, 2048);
		if (r == -1)
		{
			ioctl(0, TIOCSETA, old);
			_exit (4);
		}
		write(fd, obuffer, r);
		write(1, obuffer, r);
	}
}

void	user_command_multiplex(int mfd, int fd, struct termios *old, t_opt *opt)
{
	char	ibuffer[2048];
	int	r;

	while (1)
	{
		r = read(0, ibuffer, 2048);
		if (r == -1)
		{
			ioctl(0, TIOCSETA, old);
			_exit (5);
		}
		if (opt->options & K_OPT)
			write(fd, ibuffer, r);
		write(mfd, ibuffer, r);
	}
}
