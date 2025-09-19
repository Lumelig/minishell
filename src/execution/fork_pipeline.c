/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:44:21 by mring             #+#    #+#             */
/*   Updated: 2025/09/19 19:50:24 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: check if the exit_codes are correct
void	wait_children(t_cmd_list *cmd_list)
{
	int		status;
	ssize_t	i;

	i = 0;
	while (i < cmd_list->child_count)
	{
		waitpid(cmd_list->pids[i], &status, 0);
		if (WIFEXITED(status))
			*exit_code() = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_code() = 128 + WTERMSIG(status);
		i++;
	}
}

static void	child_helper(t_cmd_node *curr, t_cmd_node *prev)
{
	if (!curr->cmd && curr->files && curr->files->size > 0)
	{
		if (curr->next && curr->p_fd[1] != -1)
			dup2(curr->p_fd[1], STDOUT_FILENO);
		handle_redirections(curr);
		if (prev && prev->p_fd[0] != -1)
			close(prev->p_fd[0]);
		if (prev && prev->p_fd[1] != -1)
			close(prev->p_fd[1]);
		if (curr->p_fd[0] != -1)
			close(curr->p_fd[0]);
		if (curr->p_fd[1] != -1)
			close(curr->p_fd[1]);
		exit(0);
	}
}

void	handle_child(t_cmd_node *curr, t_cmd_node *prev, t_env *ms_env,
		t_cmd_list *cmd_list)
{
	child_helper(curr, prev);
	if (prev && prev->p_fd[0] != -1)
		dup2(prev->p_fd[0], STDIN_FILENO);
	if (curr->next && curr->p_fd[1] != -1)
		dup2(curr->p_fd[1], STDOUT_FILENO);
	handle_redirections(curr);
	if (prev && prev->p_fd[0] != -1)
		close(prev->p_fd[0]);
	if (prev && prev->p_fd[1] != -1)
		close(prev->p_fd[1]);
	if (curr->p_fd[0] != -1)
		close(curr->p_fd[0]);
	if (curr->p_fd[1] != -1)
		close(curr->p_fd[1]);
	if (!curr->cmd)
		exit(0);
	if (curr->builtin_type)
		run_builtin(curr, ms_env, cmd_list);
	else
		exec_cmd(curr, ms_env);
}

void	handle_parent(t_cmd_node *curr, t_cmd_node *prev)
{
	if (curr->next && curr->p_fd[1] != -1)
	{
		close(curr->p_fd[1]);
		curr->p_fd[1] = -1;
	}
	if (prev && prev->p_fd[0] != -1)
	{
		close(prev->p_fd[0]);
		prev->p_fd[0] = -1;
	}
}
