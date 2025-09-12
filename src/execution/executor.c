#include "minishell.h"

static void	forking(t_cmd_node *curr, t_cmd_node *prev, t_env *ms_env,
		pid_t *pid, ssize_t *child_count, t_cmd_list *cmd_list)
{
	pid[*child_count] = fork();
	if (pid[*child_count] == 0)
		handle_child(curr, prev, ms_env, cmd_list);
	else if (pid[*child_count] > 0)
	{
		handle_parent(curr, prev);
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

static int	cmd_check(t_cmd_list *cmd_list)
{
	if (!cmd_list || !cmd_list->head || !cmd_list->head->cmd
		|| !cmd_list->head->cmd[0])
	{
		*exit_code() = 1;
		printf("empty cmd_list\n");
		return (true);
	}
	return (false);
}

// could add pid to cmd_node struct and adjust forking & wait_children.
// same with child_count
// or cmd_list into cmd_node for reference (cleanup and freeing in exit)
void	executor(t_cmd_list *cmd_list, t_env *ms_env)
{
	t_cmd_node	*curr;
	t_cmd_node	*prev;
	pid_t		pid[cmd_list->size];
	ssize_t		child_count;

	if (cmd_check(cmd_list))
		return ;
	curr = cmd_list->head;
	child_count = 0;
	prev = NULL;
	while (curr)
	{
		create_pipes(curr);
		if (cmd_list->size == 1 && check_builtin(curr))
		{
			handle_redirections(curr);
			run_builtin(curr, ms_env, cmd_list);
		}
		else
			forking(curr, prev, ms_env, pid, &child_count, cmd_list);
		prev = curr;
		curr = curr->next;
	}
	wait_children(pid, child_count);
	return ;
}
