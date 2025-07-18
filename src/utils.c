/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:31:27 by jenne             #+#    #+#             */
/*   Updated: 2025/07/18 01:31:28 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// maybe change funktion to int for error checks.
void	free_environment(t_env *env)
{
	t_envlist	*tmp;
	t_envlist	*current;

	current = env->head;
	while (current)
	{
		tmp = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = tmp;
	}
	env->head = NULL;
	env->tail = NULL;
	env->size = 0;
}
