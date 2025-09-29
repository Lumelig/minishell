/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:23:20 by jenne             #+#    #+#             */
/*   Updated: 2025/09/29 13:21:18 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	quote_handler(char *original, char **result, int *i,
		t_quote *quote_state)
{
	if (((*quote_state == QUOTE_DOUBLE && original[i[0]] == '"')
			|| (*quote_state == QUOTE_SINGLE && original[i[0]] == '\''))
		&& (i[0] == 0 || original[i[0] - 1] != '\\'))
	{
		*quote_state = QUOTE_NONE;
		if (!*result)
			*result = ft_substr(original, 0, i[0]);
		else
			*result = cpy_str(original, *result, i[1], i[0]);
		i[1] = i[0];
	}
	else if (*quote_state == QUOTE_NONE)
	{
		if (original[i[0]] == '\'')
			*quote_state = QUOTE_SINGLE;
		else if (original[i[0]] == '"')
			*quote_state = QUOTE_DOUBLE;
		if (!*result)
			*result = ft_substr(original, 0, i[0]);
		else
			*result = cpy_str(original, *result, i[1], i[0]);
		i[1] = i[0];
	}
}

static void	expander_loop(char *original, char **result, t_env *my_env)
{
	t_quote	quote_state;
	int		i[2];

	i[0] = 0;
	i[1] = 0;
	quote_state = QUOTE_NONE;
	while (original[i[0]])
	{
		if (original[i[0]] == '"' || original[i[0]] == '\'')
			quote_handler(original, result, i, &quote_state);
		if ((quote_state == QUOTE_NONE || quote_state == QUOTE_DOUBLE)
			&& original[i[0]] == '$')
		{
			handle_dollar_copy(original, result, i, &quote_state);
			if (handle_dollar_expand(original, result, i, my_env))
				continue ;
		}
		else if (quote_state == QUOTE_NONE && original[i[0] + 1] == '\0')
			*result = cpy_str(original, *result, i[1], i[0] + 1);
		i[0]++;
	}
	return ;
}

char	*expand_string(char *original, t_env *my_env)
{
	char	*result;

	result = NULL;
	if (!original || !ft_strchr(original, '$'))
		return (original);
	expander_loop(original, &result, my_env);
	if (!result)
		return (original);
	free(original);
	return (result);
}
