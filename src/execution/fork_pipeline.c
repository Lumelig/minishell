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

void	handle_child(t_cmd_node *curr, t_cmd_node *prev, t_env *ms_env,
		t_cmd_list *cmd_list)
{
	// cmd2 reads from file, writes to file
	// cmd1 | cmd2 < input.txt > output.txt | cmd3
	// Last redirection wins (file2.txt)
	// cmd1 | cmd2 > file1.txt > file2.txt
	// file2.txt won
	// Connect stdin to previous command's output
	if (prev && prev->p_fd[0] != -1)
	{
		dup2(prev->p_fd[0], STDIN_FILENO);
		// stdin now reads from prev pipe
	}
	// Connect stdout to current command's pipe
	if (curr->next && curr->p_fd[1] != -1)
	{
		dup2(curr->p_fd[1], STDOUT_FILENO);
		// stdout now writes to curr pipe
	}
	// Handle file redirections (can override pipes)
	// Can override pipe stdin
	handle_redirections(curr);
	// need to move closing, could close pipes still in need
	// broken pipes, sigpipes, deadlocks
	if (prev && prev->p_fd[0] != -1)
		close(prev->p_fd[0]);
	if (prev && prev->p_fd[1] != -1)
		close(prev->p_fd[1]);
	if (curr->p_fd[0] != -1)
		close(curr->p_fd[0]);
	if (curr->p_fd[1] != -1)
		close(curr->p_fd[1]);
	if (curr->builtin_type)
		run_builtin(curr, ms_env, cmd_list);
	else
		exec_cmd(curr, ms_env);
}

void	handle_parent(t_cmd_node *curr, t_cmd_node *prev)
{
	// close write end of current child/pipe
	if (curr->next && curr->p_fd[1] != -1)
	{
		close(curr->p_fd[1]);
		curr->p_fd[1] = -1;
	}
	// close read end of previous pipe
	if (prev && prev->p_fd[0] != -1)
	{
		close(prev->p_fd[0]);
		prev->p_fd[0] = -1;
	}
}

/*
echo hello world | cat < /dev/null | wc -l

ls -la | grep ".txt" > files.txt | wc -l

echo "test1" | cat > output1.txt > output2.txt | echo "test2"

cat < /etc/passwd | head -5 > users.txt | tail -2 > last_users.txt | wc

echo "line1" | tee file.txt | cat >> file.txt | wc -c
*/