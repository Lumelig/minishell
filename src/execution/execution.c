#include "minishell.h"

void	execution(t_token *token)
{
	// int for sdtin and stdout
	// execution loop
	// std function
	// reset redir
	// close files
}

int	executor(t_cmd_list *cmd_list, t_env *env)
// start of the entire execution chain
{
	t_cmd_node *curr = cmd_list->head;
	pid_t pid;
	int status;

	while (curr)
	{
		if (is_builtin(curr))
			run_builtin(curr, env);
		else
		{
			pid = fork();
			if (pid == 0)
			{
				execve();
				perror("execve failed");
				exit(1);
			}
			else if (pid < 0)
				perror("fork fialed");
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

	Check if it's a builtin (in parent or child?)
	Set up redirections (<, >, >>, <<)
	Set up pipes if needed
	Fork (unless parent-only builtin)
	In child: handle redirs + execve or builtin
	In parent: track child PID, wait later
	After loop: wait for all child processes
*/