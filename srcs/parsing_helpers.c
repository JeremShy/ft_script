#include <ft_script.h>

int8_t	is_option(char c)
{
	if (c == 'a' || c == 'd' || c == 'F' || c == 'k' || c == 'p' || c == 'q'
		|| c == 'r' || c == 't')
		return (1);
	return (0);
}

int8_t	is_parametrized_option(char c)
{
	if (c == 'F' || c == 't')
		return (1);
	return (0);
}

int	get_bit_for_option(char c)
{
	if (c == 'D')
		return (D_OPT);
	if (c == 'F')
		return (F_OPT);
	if (c == 'K')
		return (K_OPT);
	if (c == 'P')
		return (P_OPT);
	if (c == 'Q')
		return (Q_OPT);
	if (c == 'R')
		return (R_OPT);
	return (0);
}

void	init_opt(t_opt *opt)
{
	opt->options = 0;
	opt->output_file = "typescript";
	opt->open_flags = 0;
	opt->flush_interval = 30;
	opt->argv = (char*[2]){"/bin/bash", NULL};
}
