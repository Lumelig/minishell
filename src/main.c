/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:18:14 by jpflegha          #+#    #+#             */
/*   Updated: 2025/09/29 14:15:56 by mring            ###   ########.fr       */
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

static void	shell_loop(t_env *my_env, int is_interactive)
{
	t_token		*token;
	t_cmd_list	*cmd_list;
	char		*input;

	while (is_interactive)
	{
		input = get_complete_input();
		if (!input && g_sigint_received != 2)
			break ;
		if (empty_input(input))
		{
			free(input);
			continue ;
		}
		input = expand_string(input, my_env);
		token = tokenize(input);
		cmd_list = parsing(token);
		cleanup(token, input);
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
	return (*exit_code());
}
