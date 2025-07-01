
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
t_cmd_node	*create_cmd_node(void)
{
	t_cmd_node	*node;

	node = malloc(sizeof(t_cmd_node));
	if (!node)
		return (NULL);
	node->cmd_type = 0;
	node->cmd = NULL;
	node->files = malloc(sizeof(t_file_list));
	if (!node->files)
	{
		free(node);
		return (NULL);
	}
	node->files->head = NULL;
	node->files->tail = NULL;
	node->files->size = 0;
	node->next = NULL;
	return (node);
}

t_cmd_list	*token_to_cmd(t_token *token)
{
	t_token	*current;
	t_cmd_list *cmd_list;

	if (!token)
		return ;
	cmd_list = init_cmd_list;
	current = token;
	while (current->type != TOKEN_EOF)
	{
		while (current->type == TOKEN_WORD)
		{
			//add cmd;
			current = current->next;
		}
		
	}
	return(cmd_list);
}
