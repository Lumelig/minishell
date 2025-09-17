#include "minishell.h"

void	exec_cmd(t_cmd_node *curr, t_env *ms_env)
{
	char	**envp;
	char	*exec_path;

	envp = env_convert(ms_env);
	if (!envp)
	{
		perror("minishell: Failed to convert environment");
		exit(1);
	}
	exec_path = get_exec_path(curr, ms_env);
	if (!exec_path)
	{
		perror(curr->cmd[0]);
		free_envp(envp);
		exit(127);
	}
	execve(exec_path, curr->cmd, envp);
	perror("execve: failure in execution");
	free_envp(envp);
	free(exec_path);
	exit(126);
}
