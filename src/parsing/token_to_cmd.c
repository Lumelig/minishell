/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:25:46 by jenne             #+#    #+#             */
/*   Updated: 2025/07/23 14:47:29 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_node	*create_cmd_node(t_cmd_list *cmd_list)
{
	t_cmd_node	*node;

	node = malloc(sizeof(t_cmd_node));
	if (!node)
		return (NULL);
	node->cmd_type = 0;
	node->cmd = NULL;
	node->files = init_file_list();
	node->next = NULL;
	if (!node->files)
	{
		free(node);
		return (NULL);
	}
	if (cmd_list->head == NULL)
		cmd_list->head = node;
	else
		cmd_list->tail->next = node;
	cmd_list->tail = node;
	cmd_list->size += 1;
	return (node);
}

t_token	*add_cmd(t_token *token, t_cmd_node *cmd_node)
{
	t_token	*count;
	int		i;

	count = token;
	i = 0;
	if (!token || !cmd_node)
		return (NULL);
	while (count && count->type == TOKEN_WORD)
	{
		i++;
		count = count->next;
	}
	cmd_node->cmd = malloc(sizeof(char *) * (i + 1));
	if (!cmd_node->cmd)
		return (NULL);
	i = 0;
	while (token && token->type == TOKEN_WORD)
	{
		cmd_node->cmd[i++] = ft_strdup(token->value);
		token = token->next;
	}
	cmd_node->cmd[i] = NULL;
	return (token);
}

t_token	*add_file_to_node(t_token *token, t_cmd_node *cmd_node)
{
	t_file_node	*file_node;

	if (!token || !cmd_node)
		return (NULL);
	if (!token->next || token->next->type != TOKEN_WORD)
		return (NULL);
	file_node = create_file_node(token->next->value, token->type);
	if (!file_node)
		return (NULL);
	if (!cmd_node->files->head)
		cmd_node->files->head = file_node;
	else
		cmd_node->files->tail->next = file_node;
	cmd_node->files->tail = file_node;
	cmd_node->files->size++;
	return (token->next->next);
}

t_token	*process_command(t_token *current, t_cmd_node *cmd_node)
{
	if (current && current->type == TOKEN_WORD)
		current = add_cmd(current, cmd_node);
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_HEREDOC
			|| current->type == TOKEN_PIPE)
		{
			current = add_file_to_node(current, cmd_node);
		}
		else
			break ;
	}
	return (current);
}

t_cmd_list	*token_to_cmd(t_token *token)
{
	t_token		*current;
	t_cmd_list	*cmd_list;
	t_cmd_node	*cmd_node;

	if (!token)
		return (NULL);
	cmd_list = init_cmd_list();
	if (!cmd_list)
		return (NULL);
	current = token;
	while (current && current->type != TOKEN_EOF)
	{
		cmd_node = create_cmd_node(cmd_list);
		if (!cmd_node)
			return (NULL);
		current = process_command(current, cmd_node);
		if (current && current->type == TOKEN_PIPE)
			current = current->next;
	}
	return (cmd_list);
}

// void free_cmd_list(t_cmd_list *cmd_list);
// void free_cmd_node(t_cmd_node *node);
// void free_file_list(t_file_list *file_list);