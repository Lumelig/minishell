/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:40:11 by mring             #+#    #+#             */
/*   Updated: 2025/09/18 19:00:44 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	free_split_path(char **split_path)
{
	int	i;

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

static char	*append_path(t_cmd_node *curr, char **split_path)
{
	char	*appended;
	int		i;

	i = 0;
	while (split_path[i])
	{
		appended = cmd_path_append(split_path[i], curr->cmd[0]);
		if (appended && access(appended, F_OK | X_OK) == 0)
			return (appended);
		free(appended);
		i++;
	}
	return (NULL);
}

char	*get_exec_path(t_cmd_node *curr, t_env *ms_env)
{
	char	**split_path;
	char	*full_path;
	char	*result;

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
	result = append_path(curr, split_path);
	free_split_path(split_path);
	return (result);
}
