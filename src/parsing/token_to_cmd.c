
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

// void    check_builtinsand_cmd(t_token *token)
// {

// }

void    token_to_cmd(t_token *token)
{
    t_cmd_list *cmd_list;
    t_cmd_node
    cmd_list = init_cmd_list();
	if (!cmd_list)
		return (NULL);
    if(token->type == TOKEN_WORD)
    {
        //check_builtins_cmd(token);
    }
    else
    {
        printf("Minishell: %s: syntax error near unexpected token", token->value);
        return ;
    }
}