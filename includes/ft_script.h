#ifndef FT_SCRIPT_H
# define FT_SCRIPT_H

# include <libft.h>
# include <sys/ioctl.h>
# include <unistd.h>
# include <termios.h>
# include <signal.h>

void			handler(int a);
void			ignore_signals(void);

int			get_next_pty_name(char current[11]);
int			open_ttys(char mbuffer[11], char sbuffer[11], int *mfd);

struct termios	*singelton_tty(struct termios *old);
void			reset_terminal();

int			parent(int pipe_to_write);
int			child(int pipe_to_read, char **envp);

#endif
