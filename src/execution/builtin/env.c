#include "minishell.h"

void	env_builtin(t_env *env)
{
	t_envlist	*curr;

	curr = env->head;
	while (curr)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
}
