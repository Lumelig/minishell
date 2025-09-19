/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:46:02 by mring             #+#    #+#             */
/*   Updated: 2025/09/19 18:45:27 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_oldpwd(t_env *ms_env)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return ;
	if (!env_key_update(ms_env, "OLDPWD", old_pwd))
	{
		if (!insert_env_node(ft_strdup("OLDPWD"), old_pwd, ms_env))
		{
			free(old_pwd);
			write(STDERR_FILENO, "failed to insert oldpwd\n", 24);
		}
	}
}

static void	update_pwd(t_env *ms_env)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	if (!env_key_update(ms_env, "PWD", new_pwd))
	{
		if (!insert_env_node(ft_strdup("PWD"), new_pwd, ms_env))
		{
			free(new_pwd);
			write(STDERR_FILENO, "cd: failed to update PWD\n", 25);
		}
	}
}

static char	*get_target_directory(t_cmd_node *curr, t_env *ms_env)
{
	char	*dir;

	if (curr->cmd[1] == NULL)
	{
		dir = get_env_value(ms_env, "HOME");
		if (!dir)
		{
			write(STDERR_FILENO, "cd: HOME not set\n", 17);
			return (NULL);
		}
		return (dir);
	}
	return (curr->cmd[1]);
}

void	cd_builtin(t_cmd_node *curr, t_env *ms_env)
{
	char	*target_dir;

	if (curr->cmd[1] && curr->cmd[2])
	{
		write(STDERR_FILENO, "cd: too many arguments\n", 23);
		*exit_code() = 1;
		return ;
	}
	update_oldpwd(ms_env);
	target_dir = get_target_directory(curr, ms_env);
	if (!target_dir)
		return ;
	if (chdir(target_dir) != 0)
	{
		perror("cd");
		*exit_code() = 1;
		return ;
	}
	update_pwd(ms_env);
}
