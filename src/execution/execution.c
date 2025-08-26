#include "minishell.h"

void	run_process(t_cmd_node *curr, t_env *ms_env)
{
	pid_t	pid;
	char	**envp;
	int		status;

	pid = fork();
	if (pid == 0) // child
	{
		envp = env_convert(ms_env);
		if (!envp)
		{
			printf("Failed to convert environment\n");
			exit(1);
		}
		execve(get_exec_path(ms_env), curr->cmd, envp);
		free_envp(envp);
		perror("execve failed");
		exit(127);
	}
	else if (pid < 0)
	{
		perror("fork failed");
		*exit_code() = 1;
		return ;
	}
	else // parent
	{
		waitpid(pid, &status, 0);
	}
}

void	executor(t_cmd_list *cmd_list, t_env *ms_env)
{
	t_cmd_node	*curr;
	int			id;

	if (!cmd_list || !cmd_list->head || !cmd_list->head->cmd
		|| !cmd_list->head->cmd[0])
	{
		*exit_code() = 1;
		return ;
	}
	curr = cmd_list->head;
	while (curr)
	{
		id = builtin_check(curr);
		if (id <= 7 && id >= 1)
		{
			run_builtin(id, curr, ms_env, cmd_list);
		}
		else // execve
		{
			run_process(curr, ms_env);
		}
		curr = curr->next;
	}
	return ;
}

/*
Decisions executor makes:
Nothing to execute? Return cleanly.
One command only, and it's a builtin that must affect the parent?
	No fork() → Call builtin directly
Anything else (multiple commands or external command)?
	Pass to the main execution loop:
		Set up pipes
		Fork each command
		Setup redirections
		Wait for all children
		Collect status
*/
/*
Execution	logic = loop over command nodes, and for each:

	Check if it's a builtin (in parent or child?) in parent duh. we didn't fork yet
	Set up redirections (<, >, >>, <<)
	Set up pipes if needed
	Fork (unless parent-only builtin)
	In child: handle redirs + execve or builtin
	In parent: track child PID, wait later
	After loop: wait for all child processes
*/

/*
get_env() function? don't need to pass env around
*/

/*
Warning for Signals:
Be careful. This global variable cannot provide any other
information or data access than the number of a received signal.
Therefore, using "norm" type structures in the global scope is
forbidden.
*/