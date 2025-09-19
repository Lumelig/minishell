/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:44:41 by mring             #+#    #+#             */
/*   Updated: 2025/09/19 18:26:48 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_builtin(t_cmd_node *curr, t_env *ms_env)
{
	t_envlist	*curr_env;

	curr_env = ms_env->head;
	if (curr->cmd[1])
	{
		write(STDERR_FILENO, "env: too many arguments\n", 24);
		return ;
	}
	while (curr_env)
	{
		if (curr_env->value)
		{
			write(STDOUT_FILENO, curr_env->key, ft_strlen(curr_env->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, curr_env->value, ft_strlen(curr_env->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		curr_env = curr_env->next;
	}
}
