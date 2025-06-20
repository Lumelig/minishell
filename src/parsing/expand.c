#include "minishell.h"

void	find_var(t_token *token, t_envlist  *envlist, t_env *env)
{
	int i;
	(void) envlist;

	i =  0;
	while (token->value[i++])
	{
		if(token->value[i]  ==  '$')
		{
			if(token->value[i  =  1]  ==  '$')
			{
				token->value = ft_itoa(env->pid);
			}
			else if  (token->value[i]  == '0')
				token->value = ft_itoa(env->exit_status);
			
		}
	}
	
}

void expand_tokens(t_token *token, t_envlist *envlist, t_env *env)
{
    // Find $ characters
	if (token->type == TOKEN_WORD)
	{
		find_var(token, envlist, env);
		// Identify variable names
    	// Look up in environment
    	// Replace with values
    	// Handle special cases ($?, $$, etc.)
	}
	else
		return;	
    
}