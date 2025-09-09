#include "minishell.h"

// TODO: adjust returns, errors and exits.
int	run_builtin(t_cmd_node *curr, t_env *ms_env, t_cmd_list *cmd_list)
{
	if (!curr || !curr->cmd[0] || !curr->cmd)
		return (0); // no cmd
	if (ft_strncmp(curr->cmd[0], "pwd", 4) == 0)
		return (pwd_builtin(), 1); // 1
	else if (ft_strncmp(curr->cmd[0], "env", 4) == 0)
		return (env_builtin(curr, ms_env), 1); // 2
	else if (ft_strncmp(curr->cmd[0], "echo", 5) == 0)
		return (echo_builtin(curr), 1); // 3
	else if (ft_strncmp(curr->cmd[0], "exit", 5) == 0)
		return (exit_builtin(curr, ms_env, cmd_list), 1); // 4
	else if (ft_strncmp(curr->cmd[0], "export", 7) == 0)
		return (export_builtin(curr, ms_env), 1); // 5
	else if (ft_strncmp(curr->cmd[0], "cd", 3) == 0)
		return (cd_builtin(curr, ms_env), 1); // 6
	else if (ft_strncmp(curr->cmd[0], "unset", 6) == 0)
		return (unset_builtin(curr, ms_env), 1); // 7
	else
		return (0); // not builtin 0
}
