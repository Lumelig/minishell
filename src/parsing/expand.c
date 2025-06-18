
#include "minishell.h"

static char	*ft_getenv_value(char *var_name)
{
	char	*env_val;

	env_val = getenv(var_name);
	if (env_val)
		return (ft_strdup(env_val));
	return (ft_strdup(""));
}

static char	*expand_special_vars(char *src, char **dst, t_env env)
{
	char	*result;

	if (*src == '?')
	{
		result = ft_itoa(env.exit_status);
		ft_strlcpy(*dst, result, ft_strlen(result) + 1);
		*dst += ft_strlen(result);
		free(result);
		return (src + 1);
	}
	else if (*src == '$')
	{
		result = ft_itoa(getpid());
		ft_strlcpy(*dst, result, ft_strlen(result) + 1);
		*dst += ft_strlen(result);
		free(result);
		return (src + 1);
	}
	**dst = '$';
	(*dst)++;
	return (src);
}

static char	*expand_braced_var(char *src, char **dst)
{
	char	*var_start;
	char	var_name[256];
	char	*env_val;
	int		var_len;

	src++;
	var_start = src;
	while (*src && *src != '}')
		src++;
	if (*src == '}')
	{
		var_len = src - var_start;
		if (var_len < 255)
		{
			ft_strlcpy(var_name, var_start, var_len + 1);
			env_val = ft_getenv_value(var_name);
			if (env_val)
			{
				ft_strlcpy(*dst, env_val, ft_strlen(env_val) + 1);
				*dst += ft_strlen(env_val);
				free(env_val);
			}
		}
		return (src + 1);
	}
	**dst = '$';
	(*dst)++;
	**dst = '{';
	(*dst)++;
	return (var_start);
}


static char	*expand_simple_var(char *src, char **dst)
{
	char	*var_start;
	char	var_name[256];
	char	*env_val;
	int		var_len;

	var_start = src;
	while (*src && (ft_isalnum(*src) || *src == '_'))
		src++;
	if (src > var_start)
	{
		var_len = src - var_start;
		if (var_len < 255)
		{
			ft_strlcpy(var_name, var_start, var_len + 1);
			env_val = ft_getenv_value(var_name);
			if (env_val)
			{
				ft_strlcpy(*dst, env_val, ft_strlen(env_val) + 1);
				*dst += ft_strlen(env_val);
				free(env_val);
			}
		}
		return (src);
	}
	**dst = '$';
	(*dst)++;
	return (src);
}


char	*expand_variables(char *str, t_env env)
{
	char	*result;
	char	*src;
	char	*dst;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) * 4 + 1);
	if (!result)
		return (NULL);
	src = str;
	dst = result;
	while (*src)
	{
		if (*src == '$')
		{
			src++;
			if (*src == '{')
				src = expand_braced_var(src, &dst);
			else if (*src == '?' || *src == '$')
				src = expand_special_vars(src, &dst, env);
			else
				src = expand_simple_var(src, &dst);
		}
		else
			*dst++ = *src++;
	}
	*dst = '\0';
	return (result);
}

static char	*get_home_directory(void)
{
	char	*home;

	home = getenv("HOME");
	if (home)
		return (ft_strdup(home));
	return (ft_strdup("/"));
}

static char	*expand_home_tilde(char *str)
{
	char	*home_dir;
	char	*result;
	size_t	total_len;

	home_dir = get_home_directory();
	if (!home_dir)
		return (ft_strdup(str));
	if (str[1] == '\0')
	{
		result = ft_strdup(home_dir);
	}
	else
	{
		total_len = ft_strlen(home_dir) + ft_strlen(str + 1) + 1;
		result = malloc(total_len);
		if (result)
		{
			ft_strlcpy(result, home_dir, total_len);
			ft_strlcat(result, str + 1, total_len);
		}
	}
	free(home_dir);
	return (result);
}


char	*expand_tilde(char *str)
{
	if (!str || str[0] != '~')
		return (ft_strdup(str));
	if (str[1] == '\0' || str[1] == '/')
		return (expand_home_tilde(str));
	return (ft_strdup(str));
}

static int	has_wildcards(char *str)
{
	while (*str)
	{
		if (*str == '*' || *str == '?' || *str == '[')
			return (1);
		str++;
	}
	return (0);
}

static int	match_pattern(char *pattern, char *str)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (1);
		while (*str)
		{
			if (match_pattern(pattern, str))
				return (1);
			str++;
		}
		return (0);
	}
	if (!*str)
		return (0);
	if (*pattern == '?' || *pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

static t_token	*create_wildcard_tokens(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*first;
	t_token			*last;
	t_token			*new_token;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	first = NULL;
	last = NULL;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue;
		if (match_pattern(pattern, entry->d_name))
		{
			new_token = malloc(sizeof(t_token));
			if (!new_token)
				break;
			new_token->type = TOKEN_WORD;
			new_token->value = ft_strdup(entry->d_name);
			new_token->next = NULL;
			if (!first)
				first = new_token;
			else
				last->next = new_token;
			last = new_token;
		}
	}
	closedir(dir);
	return (first);
}

t_token	*expand_wildcards(t_token *token)
{
	t_token	*expanded;

	if (!token || token->type != TOKEN_WORD || !has_wildcards(token->value))
		return (token);
	expanded = create_wildcard_tokens(token->value);
	if (expanded)
	{
		while (expanded->next)
			expanded = expanded->next;
		expanded->next = token->next;
		free(token->value);
		free(token);
		return (expanded);
	}
	return (token);
}

t_token	*expand_tokens(t_token *tokens, t_env env)
{
	t_token	*current;
	t_token	*head;
	t_token	*tail;
	char	*expanded_vars;
	char	*expanded_tilde;

	if (!tokens)
		return (NULL);
	current = tokens;
	head = NULL;
	tail = NULL;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded_vars = expand_variables(current->value, env);
			if (expanded_vars)
			{
				free(current->value);
				current->value = expanded_vars;
			}
			expanded_tilde = expand_tilde(current->value);
			if (expanded_tilde)
			{
				free(current->value);
				current->value = expanded_tilde;
			}
			current = expand_wildcards(current);
		}
		if (!head)
			head = current;
		else
			tail->next = current;
		while (current && current->next)
			current = current->next;
		tail = current;
		if (current)
			current = current->next;
	}
	return (head);
}

static t_token	*split_token_by_spaces(t_token *token)
{
	char	*str;
	char	*start;
	t_token	*first;
	t_token	*last;
	t_token	*new_token;
	int		len;

	str = token->value;
	first = NULL;
	last = NULL;
	while (*str)
	{
		while (*str == ' ' || *str == '\t' || *str == '\n')
			str++;
		if (!*str)
			break;
		start = str;
		while (*str && *str != ' ' && *str != '\t' && *str != '\n')
			str++;
		len = str - start;
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			break;
		new_token->type = TOKEN_WORD;
		new_token->value = malloc(len + 1);
		if (!new_token->value)
		{
			free(new_token);
			break;
		}
		ft_strlcpy(new_token->value, start, len + 1);
		new_token->next = NULL;
		if (!first)
			first = new_token;
		else
			last->next = new_token;
		last = new_token;
	}
	return (first);
}


	int	has_spaces(const char *str)
{
	while (*str)
	{
		if (ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}


t_token	*split_expanded_words(t_token *tokens)
{
	t_token	*current;
	t_token	*next;
	t_token	*head;
	t_token	*tail;
	t_token	*split_tokens;

	if (!tokens)
		return (NULL);
	current = tokens;
	head = NULL;
	tail = NULL;
	while (current)
	{
		next = current->next;
		current->next = NULL;
		if (current->type == TOKEN_WORD && has_spaces(current->value))
		{
			split_tokens = split_token_by_spaces(current);
			free(current->value);
			free(current);
			if (split_tokens)
			{
				if (!head)
					head = split_tokens;
				else
					tail->next = split_tokens;
				while (split_tokens->next)
					split_tokens = split_tokens->next;
				tail = split_tokens;
			}
		}
		else
		{
			if (!head)
				head = current;
			else
				tail->next = current;
			tail = current;
		}
		current = next;
	}
	return (head);
}

t_token	*expand_and_split_tokens(t_token *tokens, t_env env)
{
	t_token	*expanded;

	expanded = expand_tokens(tokens, env);
	return (split_expanded_words(expanded));
}