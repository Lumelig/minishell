#include "minishell.h"

static int	calculate_expanded_size(char *original, t_envlist *envlist,
		t_env *env)
{
	int	len;
	int	result_size;
	int	i;
	int	var_end;
	int	skip;

	len = ft_strlen(original);
	result_size = 0;
	i = 0;
	while (i < len)
	{
		if (original[i] == '$' && i + 1 < len)
		{
			skip = get_special_var_skip(original, i);
			if (skip > 0)
			{
				result_size += calculate_special_var_size(original, i, env);
				i += skip;
			}
			else if (ft_isalpha(original[i + 1]) || original[i + 1] == '_'
				|| original[i + 1] == '{')
			{
				result_size += calculate_var_size(original, i, envlist, env);
				get_var_length(original, i + 1, &var_end);
				i = var_end;
			}
			else
			{
				result_size++;
				i++;
			}
		}
		else
		{
			result_size++;
			i++;
		}
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
				copy_special_var(result, &j, original, &i, env);
			else if (ft_isalpha(original[i + 1]) || original[i + 1] == '_'
				|| original[i + 1] == '{')
				copy_variable(result, &j, original, &i, envlist);
			else
				result[j++] = original[i++];
		}
		else
			result[j++] = original[i++];
	}
	result[j] = '\0';
}

char	*expand_token_value(char *original, t_envlist *envlist, t_env *env)
{
	char	*result;
	int		result_size;

	if (!original || !ft_strchr(original, '$'))
		return (ft_strdup(original));
	result_size = calculate_expanded_size(original, envlist, env);
	result = malloc(result_size);
	if (!result)
		return (NULL);
	process_expansion(result, original, envlist, env);
	return (result);
}

void	expand_tokens(t_token *token, t_envlist *envlist, t_env *env)
{
	t_token	*current;
	char	*expanded;

	current = token;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_token_value(current->value, envlist, env);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}