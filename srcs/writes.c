#include <ft_script.h>

void	write_time(int fd, char *started_or_done)
{
	struct timeval	act_time;

	write(fd, "Script ", 7);
	write(fd, started_or_done, ft_strlen(started_or_done));
	write(fd, " on ", 4);


	gettimeofday(&act_time, NULL);
	write(fd, ctime(&(act_time.tv_sec)), 25);
}
