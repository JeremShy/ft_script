#include <ft_script.h>

struct termios	*singelton_tty(struct termios *old)
{
	static char i = 0;
	static struct termios save;

	if (i == 0)
	{
		ft_memcpy(&save, old, sizeof(save));
		i = 1;
	}
	return &save;
}

void			reset_terminal()
{
	ioctl(0, TIOCSETA, singelton_tty(NULL));
}

int				output_file_singelton(int param)
{
	static int	fd = -1;

	if (fd == -1)
		fd = param;
	return fd;
}
