/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:18:25 by jenne             #+#    #+#             */
/*   Updated: 2025/08/29 14:53:35 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_escape_in_word(char *line, int *i, char **word)
{
	char	*temp;

	(*i)++;
	temp = *word;
	*word = ft_strjoin_char(*word, line[*i]);
	free(temp);
	if (!*word)
		return (0);
	(*i)++;
	return (1);
}

int	handle_regular_char_in_word(char *line, int *i, char **word)
{
	char	*temp;

	temp = *word;
	*word = ft_strjoin_char(*word, line[*i]);
	free(temp);
	if (!*word)
		return (0);
	(*i)++;
	return (1);
}

static int	handle_word_char(char *line, int *i, char **word, t_quote *quote)
{
	if (line[*i] == '"' || line[*i] == '\'')
	{
		if (*quote == QUOTE_NONE)
		{
			if (line[*i] == '\'')
				*quote = QUOTE_SINGLE;
			else
				*quote = QUOTE_DOUBLE;
		}
		else if (*quote != QUOTE_NONE && ((line[*i] == '\''
					&& *quote == QUOTE_DOUBLE) || (line[*i] == '"'
					&& *quote == QUOTE_SINGLE)))
			*quote = QUOTE_MIXED;
		return (handle_quote_in_word(line, i, word));
	}
	else if (line[*i] == '\\' && line[*i + 1])
		return (handle_escape_in_word(line, i, word));
	else
		return (handle_regular_char_in_word(line, i, word));
}

int	extract_word(char *line, int *i, char **word, t_quote *quote)
{
	*word = ft_strdup("");
	*quote = QUOTE_NONE;
	if (!*word)
		return (0);
	while (line[*i] && !ft_isspace(line[*i]) && !is_operator_char(line[*i]))
	{
		if (!handle_word_char(line, i, word, quote))
		{
			free(*word);
			return (0);
		}
	}
	return (1);
}

int	process_word_token(char *line, int *i, t_token **head)
{
	char	*word;
	int		ret;
	t_quote	quote;

	if (!extract_word(line, i, &word, &quote))
		return (0);
	if (ft_strlen(word) > 0)
	{
		ret = add_token(head, TOKEN_WORD, word, quote);
		if (!ret)
		{
			free(word);
			return (0);
		}
	}
	free(word);
	return (1);
	free(word);
	return (1);
}
