#include <ft_script.h>

struct termios	*singelton_tty(struct termios *old)
{
	static char i = 0;
	static struct termios save;

	if (i == 0)
	{
		save = *old;
		i = 1;
	}
	return &save;
}
