/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:45:39 by mring             #+#    #+#             */
/*   Updated: 2025/09/18 16:45:40 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_node(t_envlist *node_to_remove, t_env *ms_env)
{
	t_envlist	*current;

	if (ms_env->head == node_to_remove)
		ms_env->head = node_to_remove->next;
	else
	{
		current = ms_env->head;
		while (current->next != node_to_remove)
			current = current->next;
		current->next = node_to_remove->next;
		if (ms_env->tail == node_to_remove)
			ms_env->tail = current;
	}
	if (ms_env->tail == node_to_remove && ms_env->head == node_to_remove->next)
		ms_env->tail = NULL;
	free(node_to_remove->value);
	free(node_to_remove->key);
	free(node_to_remove);
	ms_env->size--;
}

void	unset_builtin(t_cmd_node *curr, t_env *ms_env)
{
	int			i;
	t_envlist	*node_to_remove;

	i = 1;
	while (curr->cmd[i])
	{
		node_to_remove = find_env_var(ms_env, curr->cmd[i]);
		if (node_to_remove)
			remove_env_node(node_to_remove, ms_env);
		i++;
	}
}
