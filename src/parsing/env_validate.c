/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:19:49 by jenne             #+#    #+#             */
/*   Updated: 2025/08/25 19:19:15 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	add_shlvl_if_missing(t_env *my_env)
{
	if (!find_env_var(my_env, "SHLVL"))
		return (add_env_var(my_env, ft_strdup("SHLVL"), ft_strdup("1")));
	return (true);
}

bool	add_pwd_if_missing(t_env *my_env)
{
	char	*cwd;

	if (!find_env_var(my_env, "PWD"))
	{
		cwd = getcwd(NULL, 0);
		if (cwd && !add_env_var(my_env, ft_strdup("PWD"), cwd))
		{
			free(cwd);
			return (false);
		}
	}
	return (true);
}

bool	check_required_vars(t_env *my_env)
{
	return (add_shlvl_if_missing(my_env) && add_pwd_if_missing(my_env));
}

char	*create_env_string(t_envlist *current)
{
	char	*env_str;
	char	*temp;

	if (current->value)
	{
		env_str = ft_strjoin_char(current->key, '=');
		if (!env_str)
			return (NULL);
		temp = ft_strjoin(env_str, current->value);
		free(env_str);
		env_str = temp;
	}
	else
		env_str = ft_strjoin_char(current->key, '=');
	return (env_str);
}

void	free_env_array(char **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}
