#include <ft_script.h>

void	init_opt(t_opt *opt)
{
	opt->options = 0;
	opt->output_file = "typescript";
	opt->open_flags = 0;
	opt->flush_interval = 30;
	opt->argv = (char*[2]){"/bin/bash", NULL};
}

// int parse_argument(int ac, int *i, char **av, )

int	finish_parsing(t_m_args m_args, int i, t_opt *opt)
{
	// Finish parsing ignoring options-looking arguments
	return (1);
}

int parse(int ac, char **av, char **envp, t_opt *opt)
{
	int	i;
	t_m_args m_args;

	ft_memcpy(&m_args, &(t_m_args){ac, av, envp}, sizeof(m_args));
	init_opt(opt);
	i = 0;
	while (i < ac)
	{
		if (av[i][0] == '-')
		{
			if (ft_strequ(av[i], "--"))
			{
				return (finish_parsing(m_args, i, opt));
			}
			else
			{

			}
		}
		else
		{
			return (finish_parsing(m_args, i, opt));
		}
		i++;
	}
	return (1);
}
