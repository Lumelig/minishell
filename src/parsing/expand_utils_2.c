/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:19:58 by jenne             #+#    #+#             */
/*   Updated: 2025/08/28 15:21:31 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_special_var_skip(char *str, int i)
{
	if (str[i + 1] == '$' || str[i + 1] == '?' || str[i + 1] == '0')
		return (2);
	if (str[i + 1] == '{' && str[i + 2] && (str[i + 2] == '$'
			|| str[i + 2] == '?' || str[i + 2] == '0') && str[i + 3] == '}')
		return (4);
	return (0);
}

int	is_special_var(char *str, int pos)
{
	if (str[pos] == '$' || str[pos] == '?')
		return (1);
	return (0);
}

int	get_var_length(char *str, int start, int *end_pos)
{
	int	i;

	i = start;
	if (str[start] == '{')
	{
		i++;
		if (is_special_var(str, i) && str[i + 1] == '}')
		{
			*end_pos = i + 2;
			return (1);
		}
		while (str[i] && str[i] != '}')
		{
			if (!ft_isalnum(str[i]) && str[i] != '_')
			{
				while (str[i] && str[i] != '}')
					i++;
				if (str[i] == '}')
					i++;
				*end_pos = i;
				return (-1);
			}
			i++;
		}
		if (str[i] != '}')
		{
			*end_pos = i;
			return (-1);
		}
		*end_pos = i + 1;
		return (i - start - 1);
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (*end_pos = i, i - start);
}

int	calculate_special_var_size(char *str, int i, t_env *env)
{
	char	*tmp;
	int		size;
	int		check_pos;

	size = 0;
	check_pos = i + 1;
	if (str[check_pos] == '{')
		check_pos++;
	if (str[check_pos] == '$')
	{
		tmp = ft_itoa(env->pid);
		size = ft_strlen(tmp);
		free(tmp);
	}
	else if (str[check_pos] == '?')
	{
		tmp = ft_itoa(*exit_code());
		size = ft_strlen(tmp);
		free(tmp);
	}
	return (size);
}

int	calculate_var_size(char *str, int i, t_envlist *envlist, t_env *env)
{
	int			var_end;
	int			var_len;
	int			var_start;
	char		*var_name;
	t_envlist	*current;

	if (is_special_expansion(str, i))
		return (calculate_special_var_size(str, i, env));
	var_start = i + 1;
	if (str[var_start] == '{')
		var_start++;
	var_len = get_var_length(str, i + 1, &var_end);
	if (var_len <= 0)
		return (1);
	var_name = ft_substr(str, var_start, var_len);
	current = envlist;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (free(var_name), ft_strlen(current->value));
		current = current->next;
	}
	free(var_name);
	return (0);
}
