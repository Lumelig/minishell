/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:44:27 by mring             #+#    #+#             */
/*   Updated: 2025/09/18 21:24:54 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin(t_cmd_node *curr, t_env *ms_env, t_cmd_list *cmd_list)
{
	if (curr->builtin_type == 1)
		return (pwd_builtin(), 1);
	else if (curr->builtin_type == 2)
		return (env_builtin(curr, ms_env), 1);
	else if (curr->builtin_type == 3)
		return (echo_builtin(curr), 1);
	else if (curr->builtin_type == 4)
		return (exit_builtin(curr, ms_env, cmd_list), 1);
	else if (curr->builtin_type == 5)
		return (export_builtin(curr, ms_env), 1);
	else if (curr->builtin_type == 6)
		return (cd_builtin(curr, ms_env), 1);
	else if (curr->builtin_type == 7)
		return (unset_builtin(curr, ms_env), 1);
	else
		return (0);
}

int	check_builtin(t_cmd_node *curr)
{
	if (!curr || !curr->cmd || !curr->cmd[0])
		return (curr->builtin_type = 0, 0);
	if (ft_strncmp(curr->cmd[0], "pwd", 4) == 0)
		curr->builtin_type = 1;
	else if (ft_strncmp(curr->cmd[0], "env", 4) == 0)
		curr->builtin_type = 2;
	else if (ft_strncmp(curr->cmd[0], "echo", 5) == 0)
		curr->builtin_type = 3;
	else if (ft_strncmp(curr->cmd[0], "exit", 5) == 0)
		curr->builtin_type = 4;
	else if (ft_strncmp(curr->cmd[0], "export", 7) == 0)
		curr->builtin_type = 5;
	else if (ft_strncmp(curr->cmd[0], "cd", 3) == 0)
		curr->builtin_type = 6;
	else if (ft_strncmp(curr->cmd[0], "unset", 6) == 0)
		curr->builtin_type = 7;
	else
		curr->builtin_type = 0;
	return (curr->builtin_type);
}

void	handle_single_builtin(t_cmd_node *curr, t_env *ms_env,
		t_cmd_list *cmd_list)
{
	int	stdin;
	int	stdout;

	stdin = dup(STDIN_FILENO);
	stdout = dup(STDOUT_FILENO);
	handle_redirections(curr);
	if (curr->builtin_type == 4)
	{
		close(stdin);
		close(stdout);
		exit_builtin(curr, ms_env, cmd_list);
		return ;
	}
	run_builtin(curr, ms_env, cmd_list);
	dup2(stdin, STDIN_FILENO);
	dup2(stdout, STDOUT_FILENO);
	close(stdin);
	close(stdout);
}
