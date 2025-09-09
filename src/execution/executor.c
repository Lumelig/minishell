#include "minishell.h"

static void	forking(t_cmd_node *curr, t_env *ms_env, pid_t *pid,
		ssize_t *child_count)
{
	pid[*child_count] = fork();
	if (pid[*child_count] == 0)
		handle_child(curr, ms_env);
	else if (pid[*child_count] > 0)
	{
		handle_parent(curr, ms_env);
		(*child_count)++;
	}
	else
	{
		perror("fork");
		*exit_code() = 1;
	}
}

static void	create_pipes(t_cmd_node *curr)
{
	if (curr->next != NULL)
	{
		if (pipe(curr->p_fd) == -1)
		{
			perror("pipe failed");
			*exit_code() = 1;
			return ;
		}
	}
}

void	executor(t_cmd_list *cmd_list, t_env *ms_env)
{
	t_cmd_node	*curr;
	pid_t		pid[cmd_list->size];
	ssize_t		child_count;

	if (!cmd_list || !cmd_list->head || !cmd_list->head->cmd
		|| !cmd_list->head->cmd[0])
	{
		*exit_code() = 1;
		return ;
	}
	curr = cmd_list->head;
	child_count = 0;
	while (curr)
	{
		if (run_builtin(curr, ms_env, cmd_list))
		{
			curr = curr->next;
			continue ;
		}
		create_pipes(curr);
		forking(curr, ms_env, pid, &child_count);
		curr = curr->next;
	}
	wait_children(pid, child_count);
	return ;
}
