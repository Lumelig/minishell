/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxrmax <mring@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:18:14 by jpflegha          #+#    #+#             */
/*   Updated: 2025/09/27 14:21:54 by maxrmax          ###   ########.fr       */
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

static char	*get_input(int is_interactive)
{
	char	*input;
	char	*trimmed;

	if (is_interactive)
	{
		input = get_complete_input();
	}
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (!input)
			return (NULL);
		trimmed = ft_strtrim(input, "\n");
		free(input);
		input = trimmed;
	}
	return (input);
}

static void	shell_loop(t_env *my_env, int is_interactive)
{
	t_token		*token;
	t_cmd_list	*cmd_list;
	char		*input;

	// int i = 0;
	while (1)
	{
		input = get_input(is_interactive);
		if (!input && g_sigint_received != 2)
			break ;
		if (empty_input(input))
		{
			free(input);
			continue ;
		}
		// debug
		// printf("00 debug input string: %s\n", input);
		// expands if necessary
		input = expand_string(input, my_env);
		// printf("10 debug input after expand: %s\n", input);
		// tokenize removes quotes
		// if quote removal handled properly its good
		token = tokenize(input);
		// debug
		// t_token *debug = token;
		// while (debug)
		// {
		// 	if (debug->value)
		// 	{
		// 		if (debug->value[0] == '\0')
		// 			printf("20 debug tokens: (NULL)\n");
		// 		printf("20 debug tokens: ^%s^\n", debug->value);
		// 	}
		// 	debug = debug->next;
		// }
		// quotes are already removed when moving into parsing
		cmd_list = parsing(token);
		// debug
		// parsing() is expanding.
		// t_cmd_node *debug_list = cmd_list->head;
		// while (debug_list)
		// {
		// i = 0;
		// while (debug_list->cmd[i])
		// 	{
		// 		printf("30 debug list: ^%s^\n", debug_list->cmd[i]);
		// 		i++;
		// 	}
		// 	debug_list = debug_list->next;
		// }
		//
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
	return (*exit_code()); // need to find a fix for this
}
