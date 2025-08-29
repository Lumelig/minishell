/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:43:36 by jenne             #+#    #+#             */
/*   Updated: 2025/08/29 13:43:07 by jpflegha         ###   ########.fr       */
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
	if (redir_type == TOKEN_PIPE)
		node->filename = ft_strdup("PIPE");
	else
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
