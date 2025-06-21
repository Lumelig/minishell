
#include "minishell.h"


int	is_special_expansion(char *str, int i)
{
	if (str[i + 1] == '$' || str[i + 1] == '?')
		return (1);
	if (str[i + 1] == '{' && str[i + 2] 
		&& (str[i + 2] == '$' || str[i + 2] == '?') && str[i + 3] == '}')
		return (1);
	return (0);
}

void	copy_special_var(char *result, int *j, char *str, int *i, t_env *env)
{
	char	*tmp;
	int		check_pos;

	check_pos = *i + 1;
	if (str[check_pos] == '{')
		check_pos++;
	if (str[check_pos] == '$')
	{
		tmp = ft_itoa(env->pid);
		ft_strcpy(result + *j, tmp);
		*j += ft_strlen(tmp);
		free(tmp);
	}
	else if (str[check_pos] == '?')
	{
		tmp = ft_itoa(env->exit_status);
		ft_strcpy(result + *j, tmp);
		*j += ft_strlen(tmp);
		free(tmp);
	}
	*i += get_special_var_skip(str, *i);
}
static char	*get_var_value(char *str, int start, int len, t_envlist *envlist)
{
	char		*var_name;
	t_envlist	*current;

	var_name = ft_substr(str, start, len);
	current = envlist;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
		{
			free(var_name);
			return (current->value);
		}
		current = current->next;
	}
	free(var_name);
	return (NULL);
}
void	copy_variable(char *result, int *j, char *str, int *i,
		t_envlist *envlist)
{
	int		var_start;
	int		var_end;
	int		var_len;
	char	*var_value;

	var_start = *i + 1;
	if (str[var_start] == '{')
		var_start++;
	var_len = get_var_length(str, *i + 1, &var_end);
	if (var_len <= 0)
	{
		result[(*j)++] = str[(*i)++];
		return ;
	}
	var_value = get_var_value(str, var_start, var_len, envlist);
	if (var_value)
	{
		ft_strcpy(result + *j, var_value);
		*j += ft_strlen(var_value);
	}
	*i = var_end;
}