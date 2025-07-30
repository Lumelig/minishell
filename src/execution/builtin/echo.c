#include "minishell.h"

void	echo_builtin(t_cmd_node *curr)
{
	int	i;
	int	newline_flag;

	// -nnnn flag handling
	i = 1;
	newline_flag = 1;
	if (curr->cmd[i] && ft_strncmp(curr->cmd[i], "-n", 3) == 0)
	{
		newline_flag = 0;
		i++;
	}
	while (curr->cmd[i])
	{
		printf("%s", curr->cmd[i]);
		if (curr->cmd[i + 1])
			printf(" ");
		i++;
	}
	if (newline_flag == 1)
		printf("\n");
}
