
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
int is_operator(t_token *token)
{
	if (!token) 
        return (0);
    return(token->type == TOKEN_PIPE || token->type == TOKEN_REDIR_APPEND ||
	token->type == TOKEN_REDIR_OUT || token->type == TOKEN_REDIR_IN);
}

int syntax_validation(t_token *token)
{
    t_token	*current;
    t_token *preveious;

    current = token;
    preveious = NULL;
    while (current && current->type != TOKEN_EOF)
    {
        if(!preveious && current->type == TOKEN_PIPE)
		{
			printf("minishell: syntax error near unexpected token '|'\n");
			return (2);
		}
		preveious = current;
        current = current->next;

    }
    return (0);
}

t_cmd_list *parsing(t_env *my_env, t_token *token)
{
    t_cmd_list *cmd_list;
    
    cmd_list = init_cmd_list();
	expand_tokens(token, my_env->head, my_env);
    my_env->exit_status = syntax_validation(token);
    printf("value: %s Key: %d\n", token->value, token->type);
    //token_to_cmd(token, cmd_list);

    return(cmd_list);
}