/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:44:41 by mring             #+#    #+#             */
/*   Updated: 2025/08/12 12:11:41 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_builtin(t_cmd_node *curr, t_env *ms_env)
{
	t_envlist	*curr_env;

	curr_env = ms_env->head;
	if (curr->cmd[1])
	{
		printf("env: too many arguments\n");
		return ;
	}
	while (curr_env)
	{
		if (curr_env->value)
			printf("%s=%s\n", curr_env->key, curr_env->value);
		curr_env = curr_env->next;
	}
}
