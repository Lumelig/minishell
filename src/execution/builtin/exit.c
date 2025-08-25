#include "minishell.h"

void	exit_builtin(t_cmd_node *curr, t_env *ms_env)
{
	if (curr->cmd[1])
	{
		printf("exit: too many arguments\n");
		return ;
	}
	else
	{
		// cleanup_and_exit - double check for double free
		free_environment(ms_env);
		rl_clear_history();
		exit(0);
	}
}
