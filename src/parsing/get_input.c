/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 10:00:00 by user              #+#    #+#             */
/*   Updated: 2025/08/29 00:25:10 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_continuation_line(char *complete_input, int quote_status)
{
	char	*line;
	char	*temp;
	char	*input_temp;

	line = readline(get_continuation_prompt(quote_status));
	if (!line)
		return (complete_input);
	input_temp = ft_strjoin_char(complete_input, '\n');
	temp = ft_strjoin(input_temp, line);
	free(input_temp);
	free(complete_input);
	free(line);
	if (!temp)
		return (NULL);
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

void	custom_add_history(char *input)
{
	char	**input_arry;
	int		i;

	i = 0;
	input_arry = ft_split(input, '\n');
	if (!input_arry)
		return ;
	while (input_arry[i])
	{
		add_history(input_arry[i]);
		i++;
	}
	i = 0;
	while (input_arry[i])
		free(input_arry[i++]);
	free(input_arry);
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
			custom_add_history(line);
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
