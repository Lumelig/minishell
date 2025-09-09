#include "minishell.h"

void	wait_children(pid_t *pid, ssize_t child_count)
{
	int		status;
	ssize_t	i;

	i = 0;
	while (i < child_count)
	{
		waitpid(pid[i], &status, 0);
		if (i == child_count - 1)
		{
			if (status == 0)
				*exit_code() = 0; // Clean exit
			else if (status < 256)
				*exit_code() = 128 + status; // Signal
			else
				*exit_code() = status >> 8; // Normal exit with code
		}
		i++;
	}
}

// set up child pipes
void	handle_child(t_cmd_node *curr, t_env *ms_env)
{
	exec_cmd(curr, ms_env);
}

// set up parent pipes
void	handle_parent(t_cmd_node *curr, t_env *ms_env)
{
	(void)curr;
	(void)ms_env;
}
