#include "minishell.h"

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
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

	// do i need ms_env null check?
	envp = malloc(sizeof(char *) * (ms_env->size + 1));
	if (!envp)
		return (NULL);
	i = 0;
	curr = ms_env->head;
	while (curr)
	{
		key_len = ft_strlen(curr->key);
		envp[i] = malloc(key_len + ft_strlen(curr->value) + 2);
		if (!envp[i])
			return (free_envp(envp), NULL);
		ft_strcpy(envp[i], curr->key);
		envp[i][key_len] = '=';
		ft_strcpy(envp[i] + key_len + 1, curr->value);
		i++;
		curr = curr->next;
	}
	return (envp[i] = NULL, envp);
}

static void	free_split_path(char **split_path)
{
	int	i;

	// do i need null check?
	i = 0;
	while (split_path[i])
		free(split_path[i++]);
	free(split_path);
	return ;
}

static char	*cmd_path_append(char *path, char *cmd)
{
	char	*joined;
	int		len_1;
	int		len_2;

	// path/cmd null check?
	len_1 = ft_strlen(path);
	len_2 = ft_strlen(cmd);
	joined = malloc(len_1 + 1 + len_2 + 1);
	if (!joined)
		return (NULL);
	ft_strcpy(joined, path);
	joined[len_1] = '/';
	ft_strcpy(joined + len_1 + 1, cmd);
	joined[len_1 + len_2 + 1] = '\0';
	return (joined);
}

char	*get_exec_path(t_cmd_node *curr, t_env *ms_env)
{
	char	**split_path;
	char	*full_path;
	char	*appended;
	int		i;

	// null check? curr, curr->cmd, curr->cmd[0]
	if (ft_strchr(curr->cmd[0], '/'))
	{
		if (access(curr->cmd[0], F_OK | X_OK) == 0)
			return (curr->cmd[0]);
		return (NULL);
	}
	full_path = get_env_value(ms_env, "PATH");
	if (!full_path || !*full_path)
		return (NULL);
	split_path = ft_split(full_path, ':');
	if (!split_path)
		return (NULL);
	i = 0;
	while (split_path[i])
	{
		appended = cmd_path_append(split_path[i], curr->cmd[0]);
		if (appended && access(appended, F_OK | X_OK) == 0)
			return (free_split_path(split_path), appended);
		free(appended);
		i++;
	}
	return (free_split_path(split_path), NULL);
}
