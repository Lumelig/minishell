#include "minishell.h"

int	check_quotes_balanced_enhanced(char *line)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (line[i])
	{
		if (in_single)
		{
			if (line[i] == '\'')
			{
				in_single = 0;
			}
		}
		else if (in_double)
		{
			if (line[i] == '\\' && line[i + 1])
			{
				if (line[i + 1] == '$' || line[i + 1] == '"' || line[i
					+ 1] == '\\' || line[i + 1] == '\n')
				{
					continue ;
				}
			}
			else if (line[i] == '"')
				in_double = 0;
		}
		else
		{
			if (line[i] == '\\' && line[i + 1])
			{
				i += 2;
				continue ;
			}
			else if (line[i] == '\'')
				in_single = 1;
			else if (line[i] == '"')
				in_double = 1;
		}
		i++;
	}
	if (in_single)
		return (1);
	else if (in_double)
		return (2);
	else
		return (0);
}

// Get the appropriate prompt for continuation
char	*get_continuation_prompt(int quote_type)
{
	if (quote_type == 1)
		return ("quote> ");
	else if (quote_type == 2)
		return ("dquote> ");
	else
		return ("minishell$ ");
}

t_quote_state	check_line_completion(char *line)
{
	t_quote_state	state = {0, 0, 0};;
	int				i;

	i = 0;
	while (line[i])
	{
		if (state.in_single)
		{
			if (line[i] == '\'')
				state.in_single = 0;
		}
		else if (state.in_double)
		{
			if (line[i] == '\\' && line[i + 1])
			{
				if (line[i + 1] == '$' || line[i + 1] == '"' || line[i
					+ 1] == '\\' || line[i + 1] == '\n')
				{
					i += 2;
					continue ;
				}
			}
			else if (line[i] == '"')
				state.in_double = 0;
		}
		else
		{
			if (line[i] == '\\' && line[i + 1])
			{
				i += 2;
				continue ;
			}
			else if (line[i] == '\'')
				state.in_single = 1;
			else if (line[i] == '"')
				state.in_double = 1;
		}
		i++;
	}
	state.continuation = state.in_single || state.in_double;
	return (state);
}

char	*get_complete_input(void)
{
	char	*line;
	char	*complete_input;
	char	*temp;
	int		quote_status;
	size_t	complete_len;
	size_t	line_len;

	line = NULL;
	complete_input = NULL;
	temp = NULL;
	line = readline("minishell$ ");
	if (!line)
		return (NULL);
	quote_status = check_quotes_balanced_enhanced(line);
	if (quote_status == 0)
	{
		if (*line)
			add_history(line);
		return (line);
	}
	complete_input = ft_strdup(line);
	if (!complete_input)
	{
		free(line);
		return (NULL);
	}
	free(line);
	while (quote_status != 0)
	{
		line = readline(get_continuation_prompt(quote_status));
		if (!line)
			break ;
		complete_len = strlen(complete_input);
		line_len = strlen(line);
		temp = malloc(complete_len + line_len + 2);
		if (!temp)
		{
			free(complete_input);
			free(line);
			return (NULL);
		}
		ft_strcpy(temp, complete_input);
		temp[complete_len] = '\n';
		ft_strcpy(temp + complete_len + 1, line);
		free(complete_input);
		free(line);
		complete_input = temp;
		quote_status = check_quotes_balanced_enhanced(complete_input);
	}
	if (complete_input && *complete_input)
		add_history(complete_input);
	return (complete_input);
}
