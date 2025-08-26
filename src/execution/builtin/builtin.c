#include "minishell.h"

int	builtin_check(t_cmd_node *cmd)
{
	if (!cmd || !cmd->cmd[0] || !cmd->cmd)
		return (0);
	if (ft_strncmp(cmd->cmd[0], "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd[0], "env", 4) == 0)
		return (2);
	else if (ft_strncmp(cmd->cmd[0], "echo", 5) == 0)
		return (3);
	else if (ft_strncmp(cmd->cmd[0], "exit", 5) == 0)
		return (4);
	else if (ft_strncmp(cmd->cmd[0], "export", 7) == 0)
		return (5);
	else if (ft_strncmp(cmd->cmd[0], "cd", 3) == 0)
		return (6);
	else if (ft_strncmp(cmd->cmd[0], "unset", 6) == 0)
		return (7);
	else
		return (0);
}

void	run_builtin(int id, t_cmd_node *curr, t_env *ms_env,
		t_cmd_list *cmd_list)
{
	if (id == 1)
		pwd_builtin();
	else if (id == 2)
		env_builtin(curr, ms_env);
	else if (id == 3)
		echo_builtin(curr);
	else if (id == 4)
		exit_builtin(curr, ms_env, cmd_list);
	else if (id == 5)
		export_builtin(curr, ms_env);
	else if (id == 6)
		cd_builtin(curr, ms_env);
	else if (id == 7)
		unset_builtin(curr, ms_env); // delete/free env entry
}
