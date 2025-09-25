/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:37:59 by jenne             #+#    #+#             */
/*   Updated: 2025/09/25 16:47:11 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cpy_str(char *original, char *result, int j, int i)
{
	char	*extracted;
	char	*tmp;

	extracted = ft_substr(original, j, i - (j - 1));
	// printf("999: %s\n", extracted);
	tmp = ft_strjoin(result, extracted);
	free(extracted);
	free(result);
	return (tmp);
}

int	is_special_expansion(char *str, int i)
{
	if (str[i + 1] == '$' || str[i + 1] == '?' || str[i + 1] == '0')
		return (1);
	if (str[i + 1] == '{' && str[i + 2] && (str[i + 2] == '$' || str[i
			+ 2] == '?' || str[i + 2] == '0') && str[i + 3] == '}')
		return (1);
	return (0);
}

char	*copy_special_var(char *old_result, char *original, int *i,
		t_env *my_env)
{
	char	*var;
	int		check_pos;
	char	*result;

	// /bin/echo ''$?''"42"
	check_pos = *i + 1;
	var = NULL;
	result = old_result;
	if (original[check_pos] == '{')
		check_pos++;
	if (original[check_pos] == '$')
		var = ft_itoa(my_env->pid);
	else if (original[check_pos] == '?')
		var = ft_itoa(*exit_code());
	else if (original[check_pos] == '0')
		var = ft_strdup("minishell");
	if (var)
	{
		result = ft_strjoin(old_result, var);
		free(var);
		free(old_result);
	}
	*i += get_special_var_skip(original, *i);
	return (result);
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
	// free(var_name);
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
