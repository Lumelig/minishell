/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:45:46 by mring             #+#    #+#             */
/*   Updated: 2025/09/20 17:35:25 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!isdigit((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

static void	cleanup_and_exit(t_cmd_list *cmd_list, t_env *ms_env, int code)
{
	clean_cmd_list(cmd_list);
	free_environment(ms_env);
	rl_clear_history();
	exit(code);
}

void	exit_builtin(t_cmd_node *curr, t_env *ms_env, t_cmd_list *cmd_list)
{
	if (curr->cmd[1] && curr->cmd[2])
	{
		write(STDERR_FILENO, "exit: too many arguments\n", 25);
		*exit_code() = 1;
		if (!isatty(STDIN_FILENO))
		{
			if (!is_numeric(curr->cmd[1]))
				exit(2);
			exit(1);
		}
		return ;
	}
	if (curr->cmd[1])
	{
		if (!is_numeric(curr->cmd[1]))
		{
			write(STDERR_FILENO, "exit: numeric argument required\n", 32);
			cleanup_and_exit(cmd_list, ms_env, 2);
		}
		cleanup_and_exit(cmd_list, ms_env,
			(unsigned char)ft_atol(curr->cmd[1]));
	}
	cleanup_and_exit(cmd_list, ms_env, 0);
}