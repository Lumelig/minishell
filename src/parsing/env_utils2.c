/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:19:17 by jenne             #+#    #+#             */
/*   Updated: 2025/07/16 16:19:20 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_array(t_env *env)
{
	char		**env_array;
	t_envlist	*current;
	int			i;

	i = 0;
	if (!env || env->size == 0)
		return (NULL);
	env_array = malloc(sizeof(char *) * (env->size + 1));
	if (!env_array)
		return (NULL);
	current = env->head;
	while (current && i < env->size)
	{
		env_array[i] = create_env_string(current);
		if (!env_array[i])
		{
			free_env_array(env_array, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
