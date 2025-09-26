/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:19:58 by jenne             #+#    #+#             */
/*   Updated: 2025/09/26 14:35:07 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_expansion(char *str, int i)
{
	if (str[i + 1] == '$' || str[i + 1] == '?' || str[i + 1] == '0')
		return (1);
	if (str[i + 1] == '{' && str[i + 2] && (str[i + 2] == '$' || str[i
			+ 2] == '?' || str[i + 2] == '0') && str[i + 3] == '}')
		return (1);
	return (0);
}

int	get_special_var_skip(char *str, int i)
{
	if (str[i + 1] == '$' || str[i + 1] == '?' || str[i + 1] == '0')
		return (2);
	if (str[i + 1] == '{' && str[i + 2] && (str[i + 2] == '$' || str[i
			+ 2] == '?' || str[i + 2] == '0') && str[i + 3] == '}')
		return (4);
	return (0);
}

char	*copy_special_var(char *old_result, char *original, int *i,
		t_env *my_env)
{
	char	*var;
	int		check_pos;
	char	*result;

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

void	handle_dollar_copy(char *original, char **result, int *i,
		t_quote *quote_state)
{
	if (!*result)
		*result = ft_substr(original, 0, i[0]);
	else if (*quote_state == QUOTE_DOUBLE)
		*result = cpy_str(original, *result, i[1], i[0] - 1);
	else
		*result = cpy_str(original, *result, i[1] + 1, i[0] - 1);
}

bool	handle_dollar_expand(char *original, char **result, int *i,
		t_env *my_env)
{
	if (is_special_expansion(original, i[0]))
	{
		*result = copy_special_var(*result, original, &i[0], my_env);
		i[1] = i[0];
		return (1);
	}
	else if ((ft_isalpha(original[i[0] + 1]) || original[i[0] + 1] == '_'
			|| original[i[0] + 1] == '{'))
	{
		*result = copy_variable(*result, original, &i[0], my_env);
		i[1] = i[0];
		return (1);
	}
	return (0);
}
