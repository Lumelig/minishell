#include "minishell.h"

// static void	execution(t_token *token)
// {
// int for sdtin and stdout
// execution loop
// std function
// reset redir
// close files
// }

int	executor(t_cmd_list *cmd_list, t_env *ms_env)
// start of the entire execution chain
{
	pid_t pid;
	int status;

	// debug_test(cmd_list, ms_env);
	if (!cmd_list || !cmd_list->head)
		return (printf("bad cmd_list or head\n"), 1);
	t_cmd_node *curr = cmd_list->head;

	// printf("debug 00: entry\n");
	// printf("debug 01: %s\n", curr->cmd[0]);
	if (!curr->cmd[0])
		return (printf("debug 88: empty curr\n"), free(curr->cmd), 1);
	while (curr)
	{
		int id = builtin_check(curr);
		if (id <= 7 && id >= 1)
			run_builtin(id, curr, ms_env);
		else // execve // TODO: put it into a function
		{
			// TODO: function to get path
			printf("debug 99: execve not implemented yet\n");
			break ;
			pid = fork();
			if (pid == 0)
			{
				// execve(path, curr, ms_env);
				perror("execve failed");
				exit(1);
			}
			else if (pid < 0)
				perror("fork failed");
			else
				waitpid(pid, &status, 0);
		}
		curr = curr->next;
	}
	return (0);
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
}

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