#include <ft_script.h>

void	handler(int a)
{
	static int	to_kill = 0;

	if (to_kill == 0)
	{
		to_kill = a;
		return;
	}
	kill(to_kill, SIGKILL);
	ioctl(0, TIOCSETA, singelton_tty(NULL));
	exit(87);
}

void	ignore_signals(void)
{
	int	i;
	struct sigaction new;

	i = 1;
	new.sa_handler = SIG_IGN;
	while (i <= 31)
	{
		if (i == SIGCHLD)
		{
			new.sa_handler = handler;
			sigaction(i, &new, NULL);
			new.sa_handler = SIG_IGN;
		}
		else
			sigaction(i, &new, NULL);
		i++;
	}
}
