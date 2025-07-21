/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:37:59 by jenne             #+#    #+#             */
/*   Updated: 2025/07/18 01:22:14 by jenne            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_expansion(char *str, int i)
{
	if (str[i + 1] == '$' || str[i + 1] == '?' || str[i + 1] == '0')
		return (1);
	if (str[i + 1] == '{' && str[i + 2] && (str[i + 2] == '$'
			|| str[i + 2] == '?' || str[i + 2] == '0') && str[i + 3] == '}')
		return (1);
	return (0);
}

int	copy_special_var(char *result, char *str, int *i, t_env *env)
{
	char	*tmp;
	int		check_pos;
	int		chars_written;

	check_pos = *i + 1;
	chars_written = 0;
	tmp = NULL;
	if (str[check_pos] == '{')
		check_pos++;
	if (str[check_pos] == '$')
		tmp = ft_itoa(env->pid);
	else if (str[check_pos] == '?')
		tmp = ft_itoa(env->exit_status);
	else if (str[check_pos] == '0')
		tmp = ft_strdup("Minishell");
	if (tmp)
	{
		ft_strcpy(result, tmp);
		chars_written = ft_strlen(tmp);
		free(tmp);
	}
	*i += get_special_var_skip(str, *i);
	return (chars_written);
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

int	copy_variable(char *result, char *str, int *i, t_envlist *envlist)
{
	int		var_start;
	int		var_end;
	int		var_len;
	char	*var_value;
	int		chars_written;

	chars_written = 0;
	var_start = *i + 1;
	if (str[var_start] == '{')
		var_start++;
	var_len = get_var_length(str, *i + 1, &var_end);
	if (var_len <= 0)
	{
		result[0] = str[*i];
		*i += 1;
		return (1);
	}
	var_value = get_var_value(str, var_start, var_len, envlist);
	if (var_value)
	{
		ft_strcpy(result, var_value);
		chars_written = ft_strlen(var_value);
	}
	*i = var_end;
	return (chars_written);
}
