/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:19:32 by jenne             #+#    #+#             */
/*   Updated: 2025/07/16 16:19:34 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	add_env_var(t_env *env, char *key, char *value)
{
	t_envlist	*new_node;

	if (!key)
		return (false);
	new_node = (t_envlist *)malloc(sizeof(t_envlist));
	if (!new_node)
		return (false);
	new_node->key = key;
	new_node->delimiter = '=';
	new_node->value = value;
	new_node->next = NULL;
	if (!env->head)
	{
		env->head = new_node;
		env->tail = new_node;
	}
	else
	{
		env->tail->next = new_node;
		env->tail = new_node;
	}
	env->size++;
	return (true);
}

t_envlist	*find_env_var(t_env *env, const char *key)
{
	t_envlist	*current;

	current = env->head;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
			return (current);
		current = current->next;
	}
	return (NULL);
}

bool	update_or_add_env_var(t_env *env, char *key, char *value)
{
	t_envlist	*existing;

	existing = find_env_var(env, key);
	if (existing)
	{
		free(existing->value);
		existing->value = value;
		free(key);
		return (true);
	}
	else
		return (add_env_var(env, key, value));
}

char	*get_env_value(t_env *env, const char *key)
{
	t_envlist	*var;

	var = find_env_var(env, key);
	if (var)
		return (var->value);
	else
		return (NULL);
}

void	cleanup_env(t_env *env)
{
	t_envlist	*current;
	t_envlist	*next;

	current = env->head;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	env->head = NULL;
	env->tail = NULL;
	env->size = 0;
}
