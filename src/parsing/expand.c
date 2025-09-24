/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:23:20 by jenne             #+#    #+#             */
/*   Updated: 2025/09/24 12:40:30 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_quoting(char *original, char **join, char **expand, int *i)
{
	t_quote quote_state;
	int		start;
	int		start2;

	start2 = *i;
	quote_state = QUOTE_NONE;
	*join = NULL;  // Initialize join to NULL
	*expand = NULL;  // Initialize expand to NULL
	
	while (original[*i])
	{
		if (original[*i] == '"')
		{
			if (quote_state == QUOTE_DOUBLE)
			{
				quote_state = QUOTE_NONE;
			}
			else if (quote_state == QUOTE_NONE)
			{
				quote_state = QUOTE_DOUBLE;
			}
		}
		else if (original[*i] == '\'')
		{
			if (quote_state == QUOTE_NONE)
			{
				start = *i;  
				quote_state = QUOTE_SINGLE;	
			}
			else if (quote_state == QUOTE_SINGLE)
			{
				*expand = ft_substr(original, start2, start - start2);
				*join = ft_substr(original, start + 1, *i - start - 1);  // Content inside quotes
				(*i)++;  // Move past the closing quote
				return ;
			}
		}
		(*i)++;
	}
	
	// If no single quote pair found, expand the entire remaining string
	*expand = ft_substr(original, start2, *i - start2);
	*join = NULL;
}

static int	handle_variable_size(char *original, int *i, t_env *env,
		t_envlist *envlist)
{
	int	skip;
	int	var_end;
	int	size;

	skip = get_special_var_skip(original, *i);
	if (skip > 0)
	{
		size = calculate_special_var_size(original, *i, env);
		*i += skip - 1;
		return (size);
	}
	else if (ft_isalpha(original[*i + 1]) || original[*i + 1] == '_'
		|| original[*i + 1] == '{')
	{
		size = calculate_var_size(original, *i, envlist, env);
		get_var_length(original, *i + 1, &var_end);
		*i = var_end - 1;
		return (size);
	}
	return (1);  // Return 1 for literal '$' character
}

int	calculate_expanded_size(char *original, t_env *env, t_envlist *envlist)
{
	int	i;
	int	len;
	int	result_size;

	i = 0;
	len = ft_strlen(original);
	result_size = 0;
	while (i < len)
	{
		if (original[i] == '$')
			result_size += handle_variable_size(original, &i, env, envlist);
		else
			result_size++;
		i++;
	}
	return (result_size + 1);
}

static void	process_expansion(char *result, char *original, t_envlist *envlist,
		t_env *env)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = ft_strlen(original);
	while (i < len)
	{
		if (original[i] == '$' && i + 1 < len)
		{
			if (is_special_expansion(original, i))
				j += copy_special_var(result + j, original, &i, env);
			else if (ft_isalpha(original[i + 1]) || original[i + 1] == '_'
				|| original[i + 1] == '{')
				j += copy_variable(result + j, original, &i, envlist);
			else
				result[j++] = original[i++];
		}
		else
			result[j++] = original[i++];
	}
	result[j] = '\0';
}

char	*expand_string(char *original, t_envlist *envlist, t_env *env)
{
	char	*result;
	char	*expand;
	char	*fin_result;
	char	*join;
	char	*temp;
	int		result_size;
	int 	i;

	if (!original)
		return (NULL);
	if (!ft_strchr(original, '$'))
		return (ft_strdup(original));
		
	i = 0;
	fin_result = ft_strdup("");  // Initialize with empty string
	if (!fin_result)
		return (NULL);
		
	while (original[i])
	{
		check_quoting(original, &join, &expand, &i);
		
		if (!expand)  // Error in check_quoting
		{
			free(fin_result);
			return (NULL);
		}
		
		result_size = calculate_expanded_size(expand, env, envlist);
		result = malloc(result_size);
		if (!result)
		{
			free(expand);
			free(join);
			free(fin_result);
			return (NULL);
		}
		
		process_expansion(result, expand, envlist, env);
		
		// Join the expanded result
		temp = ft_strjoin(fin_result, result);
		free(fin_result);
		free(result);
		fin_result = temp;
		
		if (!fin_result)
		{
			free(expand);
			free(join);
			return (NULL);
		}
		
		// Join the literal content (from inside single quotes)
		if (join)
		{
			temp = ft_strjoin(fin_result, join);
			free(fin_result);
			free(join);
			fin_result = temp;
			if (!fin_result)
			{
				free(expand);
				return (NULL);
			}
		}
		
		free(expand);
	}
	
	return (fin_result);
}
