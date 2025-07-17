/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenne <jenne@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:18:25 by jenne             #+#    #+#             */
/*   Updated: 2025/07/16 16:18:28 by jenne            ###   ########.fr       */
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

int	extract_word(char *line, int *i, char **word)
{
	*word = ft_strdup("");
	if (!*word)
		return (0);
	while (line[*i] && !ft_isspace(line[*i]) && !is_operator_char(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			if (!handle_quote_in_word(line, i, word))
			{
				free(*word);
				return (0);
			}
		}
		else if (line[*i] == '\\' && line[*i + 1])
		{
			if (!handle_escape_in_word(line, i, word))
				return (0);
		}
		else
		{
			if (!handle_regular_char_in_word(line, i, word))
				return (0);
		}
	}
	return (1);
}

int	process_word_token(char *line, int *i, t_token **head)
{
	char	*word;
	int		ret;

	if (!extract_word(line, i, &word))
		return (0);
	if (ft_strlen(word) > 0)
	{
		ret = add_token(head, TOKEN_WORD, word);
		if (!ret)
		{
			free(word);
			return (0);
		}
	}
	free(word);
	return (1);
}
