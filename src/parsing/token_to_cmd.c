
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
t_file_node	*create_file_node(char *filename, int redir_type)
{
	t_file_node	*node;

	node = malloc(sizeof(t_file_node));
	if (!node)
		return (NULL);
	node->filename = strdup(filename);
	node->redir_type = redir_type;
	node->next = NULL;
	return (node);
}
t_cmd_node	*create_cmd_node(t_cmd_list *cmd_list)
{
	t_cmd_node	*node;

	node = malloc(sizeof(t_cmd_node));
	if (!node)
		return (NULL);
	node->cmd_type = 0;
	node->cmd = NULL;
	node->files = malloc(sizeof(t_file_list));
	node->next = NULL;
	if (!node->files)
	{
		free(node);
		return (NULL);
	}
	node->files->head = NULL;
	node->files->tail = NULL;
	node->files->size = 0;
	node->next = NULL;
	if (cmd_list->head == NULL)
		cmd_list->head = node;
	cmd_list->tail = node;
	cmd_list->size += 1;
	return (node);
}
t_token *add_cmd(t_token *token, t_cmd_node *cmd_node)
{
	t_token *count;
	int		i;

	count = token;
	i = 0;
	if (!token || !cmd_node)
		return ;
	while (count && count->type == TOKEN_WORD)
	{
		i++;
		count = count->next;
	}
	cmd_node->cmd = malloc(sizeof(char*) * (i + 1));
	if (!cmd_node->cmd)
		return ;
	i = 0;
	while (token && token->type == TOKEN_WORD)
	{
		cmd_node->cmd[i++] = ft_strdup(token->value);
		token = token->next;
	}
	cmd_node->cmd[i] = NULL;
	return (token);
}
t_token *process_command(t_token *current, t_cmd_node *cmd_node)
{
	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
		{
			current = add_cmd(current, cmd_node);
		}
		while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
		{
				current = add_file_to_node(current, cmd_node);
		}
	}
}

t_cmd_list *token_to_cmd(t_token *token)
{
    t_token    *current;
    t_cmd_list *cmd_list;
    t_cmd_node *cmd_node;

    if (!token)
        return (NULL);
    cmd_list = init_cmd_list();
    current = token;

    while (current && current->type != TOKEN_EOF)
    {
        // Always create node at start of new command
        cmd_node = create_cmd_node(cmd_list);
        
        // Process one complete command (until pipe or EOF)
        current = process_command(current, cmd_node);
        
        // Skip pipe token to move to next command
        if (current && current->type == TOKEN_PIPE)
            current = current->next;
    }
    return (cmd_list);
}
