#include "minishell.h"

void	exit_builtin(t_cmd_node *curr, t_env *ms_env)
{
	// TODO: garbage collector, cleanup
	printf("builtin: exit\n");
	if (curr->cmd[1])
	{
		free_environment(ms_env);
		rl_clear_history();
		exit(ft_atoi(curr->cmd[1]));
	}
	else
	{
		free_environment(ms_env);
		rl_clear_history();
		exit(0);
	}
}
