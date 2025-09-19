/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:18:14 by jpflegha          #+#    #+#             */
/*   Updated: 2025/09/19 12:07:14 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sigint_received;

bool	empty_input(char *input)
{
	int	i;

	g_sigint_received = 0;
	if (!input)
		return (true);
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (input[i] == '\0');
}

static void	cleanup(t_token *token, char *input)
{
	t_token	*temp;

	while (token)
	{
		temp = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = temp;
	}
	free(input);
}

// static char	*get_input(int is_interactive)
// {
// 	char	*input;

// 	if (is_interactive)
// 	{
// 		input = get_complete_input();
// 	}
// 	else
// 	{
// 		input = get_next_line(STDIN_FILENO);
// 		if (input && input[ft_strlen(input) - 1] == '\n')
// 			input[ft_strlen(input) - 1] = '\0';
// 	}
// 	return (input);
// }

static void	shell_loop(t_env *my_env, int is_interactive)
{
	t_token		*token;
	t_cmd_list	*cmd_list;
	char		*input;

	while (1)
	{
		// input = get_input(is_interactive);
		input = get_complete_input();
		if (!input && g_sigint_received != 2)
		{
			if (is_interactive)
				printf("exit\n");
			break ;
		}
		if (empty_input(input))
		{
			free(input);
			continue ;
		}
		token = tokenize(input);
		cmd_list = parsing(my_env, token);
		cleanup(token, input);
		if (*exit_code() == 0)
			executor(cmd_list, my_env);
		clean_cmd_list(cmd_list);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_env	my_env;
	int		is_interactive;

	init_environment(&my_env, env, argv, argc);
	is_interactive = isatty(STDIN_FILENO);
	if (is_interactive)
		setup_signal_handlers();
	shell_loop(&my_env, is_interactive);
	free_environment(&my_env);
	rl_clear_history();
	return (0);
}
