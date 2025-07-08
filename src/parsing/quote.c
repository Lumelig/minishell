#include "minishell.h"

static int	handle_escape_sequence(char *line, int *i)
{
	if (line[*i] == '\\' && line[*i + 1])
	{
		if (line[*i + 1] == '$' || line[*i + 1] == '"' 
			|| line[*i + 1] == '\\' || line[*i + 1] == '\n')
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


static int	handle_escape_in_double(char *line, int *i)
{
	if (line[*i] == '\\' && line[*i + 1])
	{
		if (line[*i + 1] == '$' || line[*i + 1] == '"' 
			|| line[*i + 1] == '\\' || line[*i + 1] == '\n')
		{
			*i += 2;
			return (1);
		}
	}
	return (0);
}

static void	process_quote_state(char *line, int *i, t_quote_state *state)
{
	if (state->in_single && line[*i] == '\'')
		state->in_single = 0;
	else if (state->in_double && line[*i] == '"')
		state->in_double = 0;
	else if (state->in_double && handle_escape_in_double(line, i))
		return ;
	else if (!state->in_single && !state->in_double)
	{
		if (line[*i] == '\\' && line[*i + 1])
			*i += 2;
		else if (line[*i] == '\'')
			state->in_single = 1;
		else if (line[*i] == '"')
			state->in_double = 1;
	}
}

t_quote_state	check_line_completion(char *line)
{
	t_quote_state	state;
	int				i;

	state.in_single = 0;
	state.in_double = 0;
	state.continuation = 0;
	i = 0;
	while (line[i])
	{
		process_quote_state(line, &i, &state);
		i++;
	}
	state.continuation = state.in_single || state.in_double;
	return (state);
}

