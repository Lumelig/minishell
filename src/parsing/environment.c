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

	//add_env_var(env, ft_strdup("SHLVL"), ft_strdup("1"));
}
void set_environment(t_env *my_env, char **key,  char  **value, char *env)
{
	char	*sep;
	char	*val_raw;
	int		shlvl;

	sep = ft_strchr(env, '=');
	if (!sep)
	{
		*key = ft_strdup(env);
		*value = NULL;
		return ;
	}

	*key = ft_substr(env, 0, sep - env);
	val_raw = sep + 1;

	if (!ft_strcmp(*key, "SHLVL"))
{
		shlvl = ft_atoi(val_raw) + 1;
		my_env->shlvl = shlvl;

		char *itoa_val = ft_itoa(shlvl);
		*value = ft_strjoin("=", itoa_val);
		free(itoa_val);  // ✅ free the temporary string
	}

	else
	{
		*value = ft_strjoin("=", val_raw);
	}
}

bool	add_env_var(t_env *env, char *key, char *value)
{
	t_envlist	*new_node;

	new_node = (t_envlist *)malloc(sizeof(t_envlist));
	if (!new_node)
		return (false);
	new_node->key = key;
	new_node->delimiter  = '=';
	new_node->value = value;
	new_node->next = NULL;
	if (!env->head)
	{
		env->head = new_node;
		env->tail = new_node;
	}
	else
	{
		env->tail->next = new_node;
		env->tail = new_node;
	}
	env->size++;
	return (true);

}


//maybe exit  if argv is a command
bool	init_environment(t_env *my_env, char **env, char **argv, int argc)
{
	int		i;
	char	*key;
	char	*value;

	init_empty_env(my_env, argv, argc);
	i = 0;
	while (env[i])
	{
		if (!ft_strcmp(env[i], "OLDPWD"))
		{
			i++;
			continue;
		}
		key = NULL;
		value = NULL;
		set_environment(my_env, &key, &value, env[i]);
		if (!add_env_var(my_env, key, value))
		{
			free(key);
			free(value);
			return (false);
		}
		i++;
	}
	if (!my_env->shlvl)
		add_env_var(my_env, ft_strdup("SHLVL"), ft_strdup("=1"));
	return (true);
}