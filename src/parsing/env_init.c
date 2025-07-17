/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:18:59 by jenne             #+#    #+#             */
/*   Updated: 2025/07/16 16:19:01 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_empty_env(t_env *env, char **argv, int argc)
{
	(void)argv;
	(void)argc;
	env->size = 0;
	env->shlvl = 1;
	env->head = NULL;
	env->tail = NULL;
	env->pid = getpid();
	env->exit_status = 0;
}

void	set_environment(t_env *my_env, char **key, char **value, char *env)
{
	char	*sep;
	int		shlvl;

	sep = ft_strchr(env, '=');
	if (!sep)
	{
		*key = ft_strdup(env);
		*value = ft_strdup("");
		return ;
	}
	*key = ft_substr(env, 0, sep - env);
	if (!ft_strcmp(*key, "SHLVL"))
	{
		shlvl = ft_atoi(sep + 1) + 1;
		my_env->shlvl = shlvl;
		*value = ft_itoa(shlvl);
	}
	else
		*value = ft_strdup(sep + 1);
}

bool	init_default_env(t_env *my_env)
{
	return (add_env_var(my_env, ft_strdup("PWD"), ft_strdup(getcwd(NULL, 0)))
		&& add_env_var(my_env, ft_strdup("SHLVL"), ft_strdup("1"))
		&& add_env_var(my_env, ft_strdup("_"),
			ft_strdup("/usr/bin/minishell")));
}

bool	process_env_vars(t_env *my_env, char **env)
{
	int		i;
	char	*key;
	char	*value;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "OLDPWD=", 7))
		{
			continue ;
		}
		key = NULL;
		value = NULL;
		set_environment(my_env, &key, &value, env[i]);
		if (!key || !add_env_var(my_env, key, value))
		{
			if (key)
				free(key);
			if (value)
				free(value);
			return (false);
		}
	}
	return (true);
}

bool	init_environment(t_env *my_env, char **env, char **argv, int argc)
{
	init_empty_env(my_env, argv, argc);
	if (!env || !env[0])
		return (init_default_env(my_env));
	return (process_env_vars(my_env, env) && check_required_vars(my_env));
}
