
#include "minishell.h"


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
    
    
	expand_tokens(token, my_env->head, my_env);
    my_env->exit_status = syntax_validation(token);
    printf("value: %s Key: %d\n", token->value, token->type);
    cmd_list = token_to_cmd(token);
	(void)cmd_list;
    return(cmd_list);
}