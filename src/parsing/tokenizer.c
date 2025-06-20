/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by student         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strjoin_char(char const *s1, char c);

static t_token	*cleanup_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	return (NULL);
}

static int	add_token(t_token **head, t_token_type type, const char *value)
{
	t_token	*new_token;
	t_token	*current;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (0);
	new_token->type = type;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (0);
	}
	new_token->next = NULL;
	if (!*head)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*   String building helpers                                                 */
/*                                                                            */
/* ************************************************************************** */

static char	*handle_escape_char(char c)
{
	char	*result;

	result = malloc(2);
	if (!result)
		return (NULL);
	if (c == 'n')
		result[0] = '\n';
	else if (c == 't')
		result[0] = '\t';
	else if (c == 'r')
		result[0] = '\r';
	else if (c == 'b')
		result[0] = '\b';
	else if (c == 'f')
		result[0] = '\f';
	else if (c == 'v')
		result[0] = '\v';
	else if (c == '\\' || c == '\'' || c == '"' || c == '$')
		result[0] = c;
	else
		result[0] = c;
	result[1] = '\0';
	return (result);
}

static char	*ft_strjoin_char(char const *s1, char c)
{
	char	*result;
	size_t	len;
	size_t	i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = s1[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*   Quote handling functions                                                */
/*                                                                            */
/* ************************************************************************** */

static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	handle_quoted_content(char *line, int *i, char **result, char quote)
{
	char	*temp;
	char	*escaped;

	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '"' && line[*i + 1])
		{
			(*i)++;
			escaped = handle_escape_char(line[*i]);
			if (!escaped)
				return (0);
			temp = *result;
			*result = ft_strjoin(*result, escaped);
			free(temp);
			free(escaped);
		}
		else
		{
			temp = *result;
			*result = ft_strjoin_char(*result, line[*i]);
			free(temp);
		}
		if (!*result)
			return (0);
		(*i)++;
	}
	if (line[*i] == quote)
		(*i)++;
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*   Word extraction function                                                 */
/*                             \n  und ander sachen handeln                                               */
/* ************************************************************************** */

static int	extract_word(char *line, int *i, char **word)
{
	char	*temp;
	char	quote_char;

	*word = ft_strdup("");
	if (!*word)
		return (0);
	while (line[*i] && !ft_isspace(line[*i]) && !is_operator_char(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			quote_char = line[*i];  // Store the quote character
			(*i)++;                 // Skip the opening quote
			if (!handle_quoted_content(line, i, word, quote_char))
			{
				free(*word);
				return (0);
			}
			// Note: handle_quoted_content already advances past the closing quote
		}
		else if (line[*i] == '\\' && line[*i + 1])
		{
			(*i)++;  // Skip the backslash
			temp = *word;
			*word = ft_strjoin_char(*word, line[*i]);
			free(temp);
			if (!*word)
				return (0);
			(*i)++;  // Move past the escaped character
		}
		else
		{
			temp = *word;
			*word = ft_strjoin_char(*word, line[*i]);
			free(temp);
			if (!*word)
				return (0);
			(*i)++;  // Move to next character
		}
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*   Operator handling                                                        */
/*                                                                            */
/* ************************************************************************** */

static int	handle_operator(char *line, int *i, t_token **head)
{
	if (line[*i] == '|')
	{
		if (!add_token(head, TOKEN_PIPE, "|"))
			return (0);
		(*i)++;
	}
	else if (line[*i] == '>')
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
	}
	else if (line[*i] == '<')
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
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*   Main tokenizer function                                                  */
/*                                                                            */
/* ************************************************************************** */

t_token	*tokenize(char *line)
{
	t_token	*head;
	int		i;
	char	*word;
	int		ret;

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
		if (!extract_word(line, &i, &word))
			return (cleanup_tokens(head));
		if (ft_strlen(word) > 0)
		{
			ret = add_token(&head, TOKEN_WORD, word);
			free(word);
			if (!ret)
				return (cleanup_tokens(head));
		}
		else
			free(word);
	}
	return (head);
}