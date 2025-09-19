/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:47:11 by mring             #+#    #+#             */
/*   Updated: 2025/09/19 18:06:54 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sigint_received;

static void	forking(t_cmd_node *curr, t_cmd_node *prev, t_env *ms_env,
		t_cmd_list *cmd_list)
{
	cmd_list->pids[cmd_list->child_count] = fork();
	if (cmd_list->pids[cmd_list->child_count] == 0)
		handle_child(curr, prev, ms_env, cmd_list);
	else if (cmd_list->pids[cmd_list->child_count] > 0)
	{
		handle_parent(curr, prev);
		cmd_list->child_count++;
	}
	else
	{
		perror("fork");
		*exit_code() = 1;
	}
}

// TODO: proper exit, returns but continues exec
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
	t_cmd_node	*curr;

	if (!cmd_list || !cmd_list->head)
	{
		*exit_code() = 1;
		write(STDERR_FILENO, "NULL cmd_list\n", 14);
		return (true);
	}
	curr = cmd_list->head;
	while (curr)
	{
		if (!curr->cmd && curr->files && curr->files->size > 0)
		{
			curr = curr->next;
			continue ;
		}
		if (!curr->cmd || !curr->cmd[0])
		{
			*exit_code() = 1;
			write(STDERR_FILENO, "empty command\n", 14);
			return (true);
		}
		curr = curr->next;
	}
	return (false);
}

static void	executor_loop(t_cmd_list *cmd_list, t_env *ms_env)
{
	t_cmd_node	*curr;
	t_cmd_node	*prev;

	curr = cmd_list->head;
	prev = NULL;
	while (curr)
	{
		create_pipes(curr);
		if (cmd_list->size == 1 && check_builtin(curr))
			handle_single_builtin(curr, ms_env, cmd_list);
		else
			forking(curr, prev, ms_env, cmd_list);
		prev = curr;
		curr = curr->next;
	}
}

void	executor(t_cmd_list *cmd_list, t_env *ms_env)
{
	if (cmd_check(cmd_list))
		return ;
	cmd_list->child_count = 0;
	cmd_list->pids = malloc(sizeof(pid_t) * cmd_list->size);
	if (!cmd_list->pids)
	{
		*exit_code() = 1;
		return ;
	}
	preprocess_heredocs(cmd_list);
	if (g_sigint_received)
	{
		g_sigint_received = 0;
		*exit_code() = 130;
		return ;
	}
	executor_loop(cmd_list, ms_env);
	cleanup_unused_heredocs(cmd_list);
	wait_children(cmd_list);
}
