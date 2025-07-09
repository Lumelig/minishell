/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 10:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/08 11:00:54 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*allocate_and_join(char *complete_input, char *line)
{
	char	*temp;
	size_t	complete_len;
	size_t	line_len;

	complete_len = strlen(complete_input);
	line_len = strlen(line);
	temp = malloc(complete_len + line_len + 2);
	if (!temp)
		return (NULL);
	ft_strcpy(temp, complete_input);
	temp[complete_len] = '\n';
	ft_strcpy(temp + complete_len + 1, line);
	return (temp);
}

static char	*handle_continuation_line(char *complete_input, int quote_status)
{
	char	*line;
	char	*temp;

	line = readline(get_continuation_prompt(quote_status));
	if (!line)
		return (complete_input);
	temp = allocate_and_join(complete_input, line);
	if (!temp)
	{
		free(complete_input);
		free(line);
		return (NULL);
	}
	free(complete_input);
	free(line);
	return (temp);
}

static char	*process_multiline_input(char *complete_input)
{
	int		quote_status;
	char	*temp;

	quote_status = check_quotes_balanced_enhanced(complete_input);
	while (quote_status != 0)
	{
		temp = handle_continuation_line(complete_input, quote_status);
		if (!temp)
			return (NULL);
		complete_input = temp;
		if (!complete_input)
			break ;
		quote_status = check_quotes_balanced_enhanced(complete_input);
	}
	return (complete_input);
}

char	*get_complete_input(void)
{
	char	*line;
	char	*complete_input;
	int		quote_status;

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
	free(line);
	if (!complete_input)
		return (NULL);
	complete_input = process_multiline_input(complete_input);
	if (complete_input && *complete_input)
		add_history(complete_input);
	return (complete_input);
}
