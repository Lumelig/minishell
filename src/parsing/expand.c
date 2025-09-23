/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:23:20 by jenne             #+#    #+#             */
/*   Updated: 2025/09/23 19:28:19 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	handle_variable_size(char *original, int *i, t_env *env,
// 		t_envlist *envlist)
// {
// 	int	skip;
// 	int	var_end;
// 	int	size;

// 	skip = get_special_var_skip(original, *i);
// 	if (skip > 0)
// 	{
// 		size = calculate_special_var_size(original, *i, env);
// 		*i += skip - 1;
// 		return (size);
// 	}
// 	else if (ft_isalpha(original[*i + 1]) || original[*i + 1] == '_'
// 		|| original[*i + 1] == '{')
// 	{
// 		size = calculate_var_size(original, *i, envlist, env);
// 		get_var_length(original, *i + 1, &var_end);
// 		*i = var_end - 1;
// 		return (size);
// 	}
// 	return (0);
// }

// int	calculate_expanded_size(char *original, t_env *env, t_envlist *envlist)
// {
// 	int	i;
// 	int	len;
// 	int	result_size;

// 	i = 0;
// 	len = ft_strlen(original);
// 	result_size = 0;
// 	while (i < len)
// 	{
// 		if (original[i] == '$')
// 			result_size += handle_variable_size(original, &i, env, envlist);
// 		else
// 			result_size++;
// 		i++;
// 	}
// 	return (result_size + 1);
// }

// static void	process_expansion(char *result, char *original,
// t_envlist *envlist,
// 		t_env *env)
// {
// 	int	i;
// 	int	j;
// 	int	len;

// 	i = 0;
// 	j = 0;
// 	len = ft_strlen(original);
// 	while (i < len)
// 	{
// 		// if a $ is encountered && i + 1 < length of original
// 		if (original[i] == '$' && i + 1 < len)
// 		{
// 			// if its a special expansion $? $0 $$
// 			if (is_special_expansion(original, i))
// 				// copies the expansion of specials into result
// 				j += copy_special_var(result + j, original, &i, env);
// 			// else if i + 1 is alpha or _ or {
// 			else if (ft_isalpha(original[i + 1]) || original[i + 1] == '_'
// 				|| original[i + 1] == '{')
// 				// get length of the variable
// 				j += copy_variable(result + j, original, &i, envlist);
// 			else
// 				// copy char into result
// 				result[j++] = original[i++];
// 		}
// 		else
// 			// copy char into result
// 			result[j++] = original[i++];
// 	}
// 	result[j] = '\0';
// }
//
//
//
//
//
/*
"" rules:
world splitting (IFS)
pathname expansion (globbing) (*, ?, ...)
\ only escapes ", $, `, \, \n
~ does not expand
"" is an empty word

no quotes:
\ escapes everything

minishell scope:
pathname expansions are not necessary to be handled
escapes technically not either but we will manage that
we don't handle tilde yet but will probably add it
need to add handling to turn '' and "" into empty words instead of only removing them
*/
// echo "\\\$USER"Test$USER
// echo j\\\iUSERiTestiUSER
// echo "\\\$USER"Test$EXPANDED
// echo "\\\\$EXPANDED"Test$EXPANDED
static char	*expander_loop(char *original, t_envlist *envlist, t_env *env)
{
	t_quote	quote_state;
	char	*result;
	char	*var;
	int		i;
	int		j;

	i = 0;
	j = 0; // quote start position
	var = NULL;
	result = NULL;
	quote_state = QUOTE_NONE;
	while (original[i])
	{
		// double quote flag setting
		if (original[i] == '"')
		{
			// need to save starting quote position
			if (quote_state == QUOTE_DOUBLE && (i == 0 || original[i
					- 1] != '\\'))
			{
				quote_state = QUOTE_NONE;
				if (!result)
					result = ft_substr(original, 0, i);
				else
					result = ft_strjoin(result, var);
			}
			else if (quote_state == QUOTE_NONE)
			{
				quote_state = QUOTE_DOUBLE;
				j = i;
			}
		}
		// single quote flag setting
		else if (original[i] == '\'')
		{
			if (quote_state == QUOTE_SINGLE && (i == 0 || original[i
					- 1] != '\\'))
			{
				// need to save starting quote position
				// can do strjoin only due nature of ''
				quote_state = QUOTE_NONE;
				result = ft_strjoin(result, var);
			}
			else if (quote_state == QUOTE_NONE)
			{
				quote_state = QUOTE_SINGLE;
				j = i;
			}
		}
		if ((quote_state == QUOTE_NONE || quote_state == QUOTE_DOUBLE)
			&& original[i] == '$')
		{
			if (!result)
				result = ft_substr(original, 0, i);
			// if i + 1 == $, ?, 0
			if (is_special_expansion(original, i))
			{
				var = copy_special_var(result, original, &i, env);
				result = ft_strjoin(result, var);
			}
			else if (ft_isalpha(original[i + 1]) || original[i + 1] == '_'
				|| original[i + 1] == '{')
			{
				var = copy_variable(result + j, original, &i, envlist);
				result = ft_strjoin(result, var);
			}
		}
		i++;
	}
	return (result);
}

char	*expand_string(char *original, t_envlist *envlist, t_env *env)
{
	char	*result;
	int		result_size;

	if (!original || !ft_strchr(original, '$'))
		return (original);
	else
		result = expander_loop(original, envlist, env);
	// process_expansion(result, original, envlist, env);
	return (result);
}
