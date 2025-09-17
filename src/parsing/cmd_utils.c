/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:43:36 by jenne             #+#    #+#             */
/*   Updated: 2025/09/17 15:51:28 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_list	*init_cmd_list(void)
{
	t_cmd_list	*cmd_list;

	cmd_list = malloc(sizeof(t_cmd_list));
	if (!cmd_list)
		return (NULL);
	cmd_list->head = NULL;
	cmd_list->tail = NULL;
	cmd_list->size = 0;
	return (cmd_list);
}

t_file_list	*init_file_list(void)
{
	t_file_list	*file_list;

	file_list = malloc(sizeof(t_file_list));
	if (!file_list)
		return (NULL);
	file_list->head = NULL;
	file_list->size = 0;
	file_list->tail = NULL;
	return (file_list);
}

t_file_node	*create_file_node(char *filename, t_token_type redir_type)
{
	t_file_node	*node;

	node = malloc(sizeof(t_file_node));
	if (!node)
		return (NULL);
	node->filename = ft_strdup(filename);
	if (!node->filename)
	{
		free(node);
		return (NULL);
	}
	node->redir_type = redir_type;
	node->next = NULL;
	return (node);
}

void	set_heredoc_index(t_cmd_node *cmd_node, int *heredoc_counter)
{
	t_file_node	*file;

	file = cmd_node->files->head;
	while (file)
	{
		file->heredocs_total = (*heredoc_counter);
		file = file->next;
	}
}
