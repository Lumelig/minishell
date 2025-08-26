/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:25:46 by jenne             #+#    #+#             */
/*   Updated: 2025/08/26 11:04:15 by jpflegha         ###   ########.fr       */
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
	t_file_node	*f_node;
	char		*filename;
	t_token		*n_token;

	if (token->type == TOKEN_PIPE)
	{
		filename = NULL;
		n_token = token->next;
	}
	else
	{
		if (!token->next || token->next->type != TOKEN_WORD)
			return (NULL);
		filename = token->next->value;
		n_token = token->next->next;
	}
	f_node = create_file_node(filename, token->type);
	if (!f_node)
		return (NULL);
	if (!cmd_node->files->head)
		cmd_node->files->head = f_node;
	else
		cmd_node->files->tail->next = f_node;
	return (cmd_node->files->tail = f_node, cmd_node->files->size++, n_token);
}

t_token	*process_command(t_token *current, t_cmd_node *cmd_node)
{
	if (current && current->type == TOKEN_WORD)
		current = add_cmd(current, cmd_node);
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_HEREDOC)
		{
			current = add_file_to_node(current, cmd_node);
		}
		else if (current->type == TOKEN_PIPE || current->type == TOKEN_END_CMD)
		{
			if (current->type == TOKEN_PIPE)
				current = add_file_to_node(current, cmd_node);
			else
				current = current->next;
			break ;
		}
		else
			current = current->next;
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
	}
	return (cmd_list);
}
