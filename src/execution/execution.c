#include "minishell.h"

void	run_cmd(t_cmd_node *curr, t_env *ms_env, int p[2])
{
	pid_t	pid;
	char	**envp;
	int		status;
	char	*exec_path;

	// need pipes before forks
	// loop for pipe creation?
	// N pipes need N pipe() pairs, 2 fds each pipe
	// count pipes by counting | as redir_type
	//
	// child:
	// if i > 0 dup2(pipes[i-1][0], STDIN_FILENO)
	// if i < N dup2(pipes[i][1], STDOUT_FILENO)
	//
	// parent:
	// close(pipes[i][1]) once the child that writes to it is forked
	// close(pipes[i-1][0]) after the reader child has been forked (or close them as soon as safe
	// collect child pids into an array and wait for them all after spawning
	// don't wait inside run_process, so wait before it.
	// always close every fd opened
	// special cases:
	// 0 pipes, skip pipe allocation, close fds necessary
	// TODO: parent-only -> builtins -> no forking or piping in builtins
	// redirections take priority: do pipe dup2 first,
	// then apply redirects which replace the fds
	// error handling: on pipe() failure free allocated memory and close any opened fds
	/*
	Quick tips
	- Use index-based traversal (assign an index to each node) to decide which pipe fds to use.
	- Always close fds in both branches (child and parent) even on error paths.
	- If you currently wait inside run_process,
	move waiting to after the spawn loop and return child pids from run_process or refactor runner to do spawn-only.
	If you want, I can:
	- show a minimal refactor of executor to create pipes,
	spawn children and wait (not full execve details), or
	- show safe helper functions for pipe allocation/cleanup and for closing
	*/
	(void)p;
	pid = fork();
	if (pid == 0) // child
	{
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
		exit(126);
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
	int			p[2];
	int			id;

	if (!cmd_list || !cmd_list->head || !cmd_list->head->cmd
		|| !cmd_list->head->cmd[0])
	{
		*exit_code() = 1;
		return ;
	}
	curr = cmd_list->head;
	if (pipe(p) == -1) // bytes written on p[1] can be read on p[0]
		return (perror("pipe:"));
	while (curr)
	{
		id = builtin_check(curr);
		if (id <= 7 && id >= 1)
		{
			run_builtin(id, curr, ms_env, cmd_list);
		}
		else
		{
			run_cmd(curr, ms_env, p);
		}
		curr = curr->next;
	}
	return ;
}

// get_env() function? don't need to pass env around
