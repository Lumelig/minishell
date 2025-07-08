#include "minishell.h"

void	exit_builtin(t_cmd_node *curr)
{
	// TODO: garbage collector, cleanup.
	printf("builtin: exit\n");
	if (curr->cmd[1])
		exit(ft_atoi(curr->cmd[1]));
	else
		exit(0);
}
