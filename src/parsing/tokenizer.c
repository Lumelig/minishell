
#include "minishell.h"

int	handle_pipe_operator(int *i, t_token **head)
{
	if (!add_token(head, TOKEN_PIPE, "|"))
		return (0);
	(*i)++;
	return (1);
}

int	handle_output_redirect(char *line, int *i, t_token **head)
{
	if (line[*i + 1] == '>')
	{
		if (!add_token(head, TOKEN_REDIR_APPEND, ">>"))
			return (0);
		(*i) += 2;
	}
	else
	{
		if (!add_token(head, TOKEN_REDIR_OUT, ">"))
			return (0);
		(*i)++;
	}
	return (1);
}

int	handle_input_redirect(char *line, int *i, t_token **head)
{
	if (line[*i + 1] == '<')
	{
		if (!add_token(head, TOKEN_HEREDOC, "<<"))
			return (0);
		(*i) += 2;
	}
	else
	{
		if (!add_token(head, TOKEN_REDIR_IN, "<"))
			return (0);
		(*i)++;
	}
	return (1);
}

int	handle_operator(char *line, int *i, t_token **head)
{
	if (line[*i] == '|')
		return (handle_pipe_operator(i, head));
	else if (line[*i] == '>')
		return (handle_output_redirect(line, i, head));
	else if (line[*i] == '<')
		return (handle_input_redirect(line, i, head));
	return (1);
}
t_token	*tokenize(char *line)
{
	t_token	*head;
	int		i;

	if (!line)
		return (NULL);
	i = 0;
	head = NULL;
	while (line[i])
	{
		if (ft_isspace(line[i]))
		{
			i++;
			continue ;
		}
		if (is_operator_char(line[i]))
		{
			if (!handle_operator(line, &i, &head))
				return (cleanup_tokens(head));
			continue ;
		}
		if (!process_word_token(line, &i, &head))
			return (cleanup_tokens(head));
	}
	if (!add_token(&head, TOKEN_EOF, ""))
		return (cleanup_tokens(head));
	return (head);
}
