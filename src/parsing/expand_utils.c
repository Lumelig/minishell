/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:37:59 by jenne             #+#    #+#             */
/*   Updated: 2025/09/25 17:52:37 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cpy_str(char *original, char *result, int j, int i)
{
	char	*extracted;
	char	*tmp;

	extracted = ft_substr(original, j, i - (j - 1));
	tmp = ft_strjoin(result, extracted);
	free(extracted);
	free(result);
	return (tmp);
}

static int	handle_braced_var(char *str, int start, int *end_pos)
{
	int	i;

	i = start + 1;
	if ((str[i] == '$' || str[i] == '?') && str[i + 1] == '}')
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
		return (*end_pos = i, -1);
	return (*end_pos = i + 1, i - start - 1);
}

static int	get_var_length(char *original, int start, int *end_pos)
{
	int	i;

	i = start;
	if (original[start] == '{')
		return (handle_braced_var(original, start, end_pos));
	while (original[i] && (ft_isalnum(original[i]) || original[i] == '_'))
		i++;
	*end_pos = i;
	return (i - start);
}

static char	*get_var_value(char *str, int start, int len, t_env *my_env)
{
	char		*var_name;
	t_envlist	*current;

	var_name = ft_substr(str, start, len);
	current = my_env->head;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
		{
			free(var_name);
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

char	*copy_variable(char *old_result, char *original, int *i, t_env *my_env)
{
	int		var_start;
	int		var_end;
	int		var_len;
	char	*var;
	char	*result;

	var_start = *i + 1;
	if (original[var_start] == '{')
		var_start++;
	var_len = get_var_length(original, var_start, &var_end);
	if (var_len <= 0)
	{
		*i = var_end;
		return (old_result);
	}
	var = get_var_value(original, var_start, var_len, my_env);
	if (!var)
	{
		*i = var_end;
		return (old_result);
	}
	result = ft_strjoin(old_result, var);
	free(old_result);
	*i = var_end;
	return (result);
}
