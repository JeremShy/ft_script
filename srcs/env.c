#include <ft_script.h>

char	*get_shell(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnequ(envp[i], "SHELL=", 6))
			return (envp[i] + 6);
		i++;
	}
	return (NULL);
}

char	*get_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnequ(envp[i], "PATH=", 5))
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char *find_in_path(char *file, char **envp, char buffer[1024])
{
	char *path;

	path = get_path(envp);
	return NULL;
}
