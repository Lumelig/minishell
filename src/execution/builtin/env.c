#include "minishell.h"

void	env_builtin(t_cmd_node *curr, t_env *ms_env)
{
	t_envlist	*curr_env;

	curr_env = ms_env->head;
	if (curr->cmd[1])
	{
		printf("Too many arguments for env\n");
		return ;
	}
	// printf("debug env\n");
	while (curr_env)
	{
		if (curr_env->value)
			printf("%s=%s\n", curr_env->key, curr_env->value);
		curr_env = curr_env->next;
	}
}
