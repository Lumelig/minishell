#include "minishell.h"

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**env_convert(t_env *ms_env)
{
	char		**envp;
	t_envlist	*curr;
	int			i;
	int			key_len;

	i = 0;
	curr = ms_env->head;
	envp = malloc(sizeof(char *) * (ms_env->size + 1));
	if (!envp)
		return (NULL);
	while (curr)
	{
		key_len = ft_strlen(curr->key);
		envp[i] = malloc(key_len + ft_strlen(curr->value) + 2);
		if (!envp[i])
		{
			return (free_envp(envp), NULL);
		}
		ft_strcpy(envp[i], curr->key);
		envp[i][key_len] = '=';
		ft_strcpy(envp[i] + key_len + 1, curr->value);
		i++;
		curr = curr->next;
	}
	return (envp[i] = NULL, envp);
}

char	*get_exec_path(t_env *ms_env)
{
	char *path;

	path = get_env_value(ms_env, "PATH");
	// find correct path for the executable
	return (path);
}