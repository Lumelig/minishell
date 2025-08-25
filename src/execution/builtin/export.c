/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:45:26 by mring             #+#    #+#             */
/*   Updated: 2025/08/12 11:06:31 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	insert_env_node(char *key, char *value, t_env *ms_env)
{
	t_envlist	*new_node;

	new_node = (t_envlist *)malloc(sizeof(t_envlist));
	if (!new_node)
		return (false);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	if (ms_env->head)
	{
		ms_env->tail->next = new_node;
		ms_env->tail = new_node;
	}
	else
	{
		ms_env->head = new_node;
		ms_env->tail = new_node;
	}
	ms_env->size++;
	return (true);
}

bool	env_key_update(t_env *ms_env, char *key, char *new_value)
{
	t_envlist	*current;

	current = ms_env->head;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
		{
			free(current->value);
			current->value = new_value;
			return (true);
		}
		current = current->next;
	}
	return (false);
}

static bool	validate_key(char *key)
{
	int	i;

	if (ft_isalpha(key[0]) == false && key[0] != '_')
	{
		return (false);
	}
	i = 1;
	while (key[i])
	{
		if (ft_isalnum(key[i]) == false && key[i] != '_')
		{
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	parse_export_args(char *cmd, t_env *ms_env)
{
	char	*sep;
	char	*key;
	char	*value;

	sep = ft_strchr(cmd, '=');
	if (!sep)
	{
		key = ft_strdup(cmd);
		value = ft_strdup("");
	}
	else
	{
		key = ft_substr(cmd, 0, sep - cmd);
		value = ft_strdup(sep + 1);
	}
	if (!validate_key(key))
		return (free(key), free(value), false);
	if (env_key_update(ms_env, key, value))
		return (free(key), true);
	if (insert_env_node(key, value, ms_env))
		return (true);
	return (false);
}

void	export_builtin(t_cmd_node *curr, t_env *ms_env)
{
	int	i;

	i = 1;
	if (!curr->cmd[1])
	{
		env_builtin(curr, ms_env);
		return ;
	}
	while (curr->cmd[i])
	{
		if (!parse_export_args(curr->cmd[i], ms_env))
		{
			printf("export: '%s': not a valid identifier\n", curr->cmd[i]);
		}
		i++;
	}
}
