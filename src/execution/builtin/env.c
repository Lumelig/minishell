#include "minishell.h"

void	env_builtin(t_env *ms_env)
{
	t_envlist	*curr;

	curr = ms_env->head;
	// printf("debug env\n");
	while (curr)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
}
