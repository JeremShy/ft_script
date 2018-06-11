#include <ft_script.h>

int	get_next_pty_name(char current[11])
{
	char	*letter;
	char	*nbr;

	 letter = current + 8;
	 nbr = current + 9;
	 if (*nbr != '9')
	 {
		 (*nbr)++;
		 return (1);
	 }
	 else if (*letter != 'z' && *letter != 'e')
	 {
		 *nbr = '0';
		 (*letter)++;
		 return (1);
	 }
	 else if (*letter == 'z')
	 {
		 *nbr = '0';
		 (*letter) = 'a';
		 return (1);
	 }
	 else
		 return (0);
}

int	open_ttys(char mbuffer[11], char sbuffer[11], int *mfd, int *sfd)
{
	ft_strcpy(mbuffer, "/dev/ttyp0");
	ft_strcpy(sbuffer, mbuffer);
	printf("Trying to open %s\n", mbuffer);
	while ((*mfd = open(mbuffer, O_RDWR)) == -1)
	{
		printf("Tried to open %s\n", mbuffer);
		if (get_next_pty_name(mbuffer) == 0)
			return (0);
	}
	ft_strcpy(sbuffer, mbuffer);
	sbuffer[5] = 'p';
	*sfd = open(mbuffer, O_RDWR);
	return (1);
}

int	child()
{
	int	default_tty_fd;
	int	mfd;
	int	sfd;
	char	mbuffer[11];
	char	sbuffer[11];

	default_tty_fd = dup(0);
	if (open_ttys(mbuffer, sbuffer, &mfd, &sfd) == 0)
		return (0);
	dprintf(default_tty_fd, "pouet! mfd = %d, sfd = %d\n", mfd, sfd);

	return (1);
}

int	parent()
{
	return (1);
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	int pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("Error while forking.\n", 2);
		return (1);
	}

	if (pid != 0)	// Parent
	{
		parent();
	}
	else			// Child
	{
		child();
	}

	return (0);
}
