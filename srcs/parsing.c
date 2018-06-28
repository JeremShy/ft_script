#include <ft_script.h>

void	apply_simple_option(char option, t_opt *opt)
{
	if (option == 'a')
		opt->open_flags |= O_APPEND;
	else
		opt->options |= (get_bit_for_option(option));
}

int parse_argument(t_m_args *m_args, int *i, t_opt *opt)
{
	int	j;

	j = 1;
	while (m_args->av[*i][j])
	{
		if (!is_option(m_args->av[*i][j]))
		{
			ft_putstr_fd("Illegal option: ", 2);
			ft_putchar_fd(m_args->av[*i][j], 2);
			ft_putchar_fd('\n', 2);
			return (0);
		}
		if (is_parametrized_option(m_args->av[*i][j]))
		{

		}
		else
		{
			apply_simple_option(m_args->av[*i][j], opt);
		}
		j++;
	}
	return (1);
}

int	finish_parsing(t_m_args *m_args, int i, t_opt *opt)
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
	i = 1;
	while (i < ac)
	{
		if (av[i][0] == '-')
		{
			if (ft_strequ(av[i], "--"))
			{
				return (finish_parsing(&m_args, i + 1, opt));
			}
			else
			{
				if (!parse_argument(&m_args, &i, opt))
					return (0);
			}
		}
		else
		{
			return (finish_parsing(&m_args, i, opt));
		}
		i++;
	}
	return (1);
}
