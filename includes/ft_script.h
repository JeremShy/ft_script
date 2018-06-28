#ifndef FT_SCRIPT_H
# define FT_SCRIPT_H

# include <libft.h>
# include <sys/ioctl.h>
# include <unistd.h>
# include <termios.h>
# include <signal.h>

# define D_OPT (1<<0)
# define F_OPT (1<<1)
# define K_OPT (1<<2)
# define P_OPT (1<<3)
# define Q_OPT (1<<4)
# define R_OPT (1<<5)

typedef struct s_m_args
{
	int			ac;
	char		**av;
	char		**envp;
}				t_m_args;

typedef struct	s_opt {
	char	options;
	char	*output_file;
	int		open_flags;
	int		flush_interval;
	char	**argv;
}				t_opt;

int parse(int ac, char **av, char **envp, t_opt *opt);

void			handler(int a);
void			ignore_signals(void);

int			get_next_pty_name(char current[11]);
int			open_ttys(char mbuffer[11], char sbuffer[11], int *mfd);

struct termios	*singelton_tty(struct termios *old);
void			reset_terminal();

int			parent(int pipe_to_write);
int			child(int pipe_to_read, char **envp);

#endif
