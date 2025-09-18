/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:40:01 by mring             #+#    #+#             */
/*   Updated: 2025/09/18 18:51:04 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**env_convert(t_env *ms_env)
{
	char		**envp;
	t_envlist	*curr;
	int			i;
	int			key_len;

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

void	exec_cmd(t_cmd_node *curr, t_env *ms_env)
{
	char	**envp;
	char	*exec_path;

	envp = env_convert(ms_env);
	if (!envp)
	{
		perror("minishell: Failed to convert environment");
		exit(1);
	}
	exec_path = get_exec_path(curr, ms_env);
	if (!exec_path)
	{
		perror(curr->cmd[0]);
		free_envp(envp);
		exit(127);
	}
	execve(exec_path, curr->cmd, envp);
	perror("execve: failure in execution");
	free_envp(envp);
	free(exec_path);
	exit(126);
}
