#include "minishell.h"

//maybe change funktion to int for error checks.
void	free_environment(t_env *env)
{
	t_envlist	*tmp;
	t_envlist	*current = env->head;

	while (current)
	{
		tmp = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = tmp;
	}
	env->head = NULL;
	env->tail = NULL;
	env->size = 0;
}