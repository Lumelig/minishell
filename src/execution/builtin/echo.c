/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:45:11 by mring             #+#    #+#             */
/*   Updated: 2025/09/19 16:27:56 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_flags(t_cmd_node *curr, int *i)
{
	int	j;
	int	newline_flag;

	newline_flag = 1;
	while (curr->cmd[*i] && curr->cmd[*i][0] == '-')
	{
		j = 1;
		while (curr->cmd[*i][j] == 'n')
			j++;
		if (curr->cmd[*i][j] != '\0' || curr->cmd[*i][1] == '\0')
			break ;
		else
		{
			(*i)++;
			newline_flag = 0;
		}
	}
	return (newline_flag);
}

void	echo_builtin(t_cmd_node *curr)
{
	int	i;
	int	newline_flag;

	i = 1;
	newline_flag = process_flags(curr, &i);
	while (curr->cmd[i])
	{
		printf("%s", curr->cmd[i]);
		if (curr->cmd[i + 1])
			printf(" ");
		i++;
	}
	if (newline_flag == 1)
		printf("\n");
}
