
#include "minishell.h"

t_cmd_list *init_cmd_list(void)
{
    t_cmd_list *cmd_list = malloc(sizeof(t_cmd_list));
    if (!cmd_list)
        return NULL;
    cmd_list->head = NULL;
    cmd_list->tail = NULL;
    cmd_list->size = 0;
    return cmd_list;
}
t_file_node *create_file_node(char *filename, int redir_type)
{
    t_file_node *node = malloc(sizeof(t_file_node));
    if (!node)
        return NULL;
    
    node->filename = strdup(filename);
    node->redir_type = redir_type;
    node->next = NULL;
    return node;
}
t_cmd_node *create_cmd_node(void)
{
	t_cmd_node *node;

	node = malloc(sizeof(t_cmd_node));
	if(!node)
		return(NULL);
	
}


t_cmd_list    token_to_cmd(t_token *token,  t_cmd_list *cmd_list)
{
    (void)token;
	if (!token)
		return ;
	init_cmd_list;
	 t_token *current;
	 current = token;

	
}


