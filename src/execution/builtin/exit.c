/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:45:46 by mring             #+#    #+#             */
/*   Updated: 2025/09/18 16:45:54 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// cleanup_and_exit - double check for double free
void	exit_builtin(t_cmd_node *curr, t_env *ms_env, t_cmd_list *cmd_list)
{
	if (curr->cmd[1])
	{
		printf("exit: too many arguments\n");
		return ;
	}
	else
	{
		clean_cmd_list(cmd_list);
		free_environment(ms_env);
		rl_clear_history();
		exit(0);
	}
}
