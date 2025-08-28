/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:31:27 by jenne             #+#    #+#             */
/*   Updated: 2025/08/28 15:28:52 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// maybe change funktion to int for error checks.
void	free_environment(t_env *env)
{
	t_envlist	*tmp;
	t_envlist	*current;

	current = env->head;
	while (current)
	{
		tmp = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = tmp;
	}
	env->head = NULL;
	env->tail = NULL;
	env->size = 0;
}

static void	free_file_list(t_file_list *file_list)
{
	t_file_node	*file_node;
	t_file_node	*next_node;

	if (!file_list)
		return ;
	file_node = file_list->head;
	while (file_node)
	{
		next_node = file_node->next;
		free(file_node->filename);
		free(file_node);
		file_node = next_node;
	}
	free(file_list);
}

static void	free_cmd_node(t_cmd_node *cmd_node)
{
	int	i;

	if (!cmd_node)
		return ;
	i = 0;
	if (cmd_node->cmd)
	{
		while (cmd_node->cmd[i])
		{
			free(cmd_node->cmd[i]);
			i++;
		}
		free(cmd_node->cmd);
	}
	free_file_list(cmd_node->files);
	free(cmd_node);
}

void	clean_cmd_list(t_cmd_list *cmd_list)
{
	t_cmd_node	*cmd_node;
	t_cmd_node	*cmd_next;

	if (!cmd_list)
		return ;
	cmd_node = cmd_list->head;
	while (cmd_node)
	{
		cmd_next = cmd_node->next;
		free_cmd_node(cmd_node);
		cmd_node = cmd_next;
	}
	free(cmd_list);
}
