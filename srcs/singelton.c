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
