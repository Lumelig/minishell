#include "minishell.h"

static int	handle_escape_sequence(char *line, int *i)
{
	if (line[*i] == '\\' && line[*i + 1])
	{
		if (line[*i + 1] == '$' || line[*i + 1] == '"' || line[*i + 1] == '\\'
			|| line[*i + 1] == '\n')
		{
			*i += 2;
			return (1);
		}
	}
	return (0);
}

static int	process_quotes(char *line, int *i, int *in_single, int *in_double)
{
	if (*in_single && line[*i] == '\'')
		*in_single = 0;
	else if (*in_double && line[*i] == '"')
		*in_double = 0;
	else if (*in_double && handle_escape_sequence(line, i))
		return (1);
	else if (!*in_single && !*in_double)
	{
		if (line[*i] == '\\' && line[*i + 1])
			*i += 2;
		else if (line[*i] == '\'')
			*in_single = 1;
		else if (line[*i] == '"')
			*in_double = 1;
	}
	return (0);
}

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
		if (process_quotes(line, &i, &in_single, &in_double))
			continue ;
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
