#include "minishell.h"

static void	update_oldpwd(t_env *ms_env)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return ;
	if (!env_key_update(ms_env, "OLDPWD", old_pwd))
		if (!insert_env_node(ft_strdup("OLDPWD"), old_pwd, ms_env))
		{
			free(old_pwd);
			printf("failed to insert oldpwd\n");
		}
}

static void	update_pwd(t_env *ms_env)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	if (!env_key_update(ms_env, "PWD", new_pwd))
		if (!insert_env_node(ft_strdup("PWD"), new_pwd, ms_env))
		{
			free(new_pwd);
			printf("cd: failed to update PWD\n");
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
			printf("cd: HOME not set\n");
			return (NULL);
		}
		return (dir);
	}
	return (curr->cmd[1]);
}

void	cd_builtin(t_cmd_node *curr, t_env *ms_env)
{
	char	*target_dir;

	if (curr->cmd[2])
	{
		printf("cd: too many arguments\n");
		return ;
	}
	update_oldpwd(ms_env);
	target_dir = get_target_directory(curr, ms_env);
	if (!target_dir)
		return ;
	if (chdir(target_dir) != 0)
	{
		perror("cd");
		return ;
	}
	update_pwd(ms_env);
}
