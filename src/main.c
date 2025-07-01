/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by student         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	empty_input(char *input)
{
	int	i;

	if (!input)
		return (true);
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (input[i] == '\0');
}

void	print_history(void)
{
	HIST_ENTRY	**the_list;
	int			i;

	i = 0;
	the_list = history_list();
	if (the_list)
	{
		while (the_list[i])
		{
			printf("%d: %s\n", i + history_base, the_list[i]->line);
			i++;
		}
	}
}

void	free_tokens(t_token *token)
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
}

void	print_env_list(t_envlist *head)
{
	t_envlist	*current;

	current = head;
	while (current)
	{
		printf("%s%c%s\n", current->key, current->delimiter, current->value);
		current = current->next;
	}
}

static void	cleanup_and_exit(t_token *token, char *input, t_env *my_env)
{
	free_tokens(token);
	free(input);
	free_environment(my_env);
	rl_clear_history();
}

static int	check_exit_command(t_token *token)
{
	if (token && !ft_strncmp("exit", token->value, 5)
		&& ft_strlen(token->value) == 4)
		return (1);
	return (0);
}

// static void	process_tokens(t_token *token)
// {
// 	t_token	*current;

// 	current = token;
// 	while (current)
// 	{
// 		printf("Token: '%s', Type: %d\n", current->value, current->type);
// 		current = current->next;
// 	}
// }

static char	*get_input(int is_interactive)
{
	char	*input;

	if (is_interactive)
	{
		input = get_complete_input();
	}
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (input && input[ft_strlen(input) - 1] == '\n')
			input[ft_strlen(input) - 1] = '\0';
	}
	return (input);
}

static void	shell_loop(t_env *my_env, int is_interactive)
{
	t_token *token;
	t_cmd_list *cmd_list;
	char	*input;

	while (1)
	{
		input = get_input(is_interactive);
		if (!input)
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
		if (is_interactive)//debug
			print_history();
		token = tokenize(input);
		if (check_exit_command(token))
		{
			cleanup_and_exit(token, input, my_env);
			exit(0);
		}
		cmd_list = parsing(my_env, token);
		(void)cmd_list;
		free_tokens(token);
		free(input);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_env	my_env;
	int		is_interactive;

	init_environment(&my_env, env, argv, argc);
	// if (argc > 1)
	// {
	// 	if (strcmp(argv[1], "-c") == 0 && argc > 2)
	// 	{
	// 		execute_command_string(&my_env, argv[2]);
	// 		free_environment(&my_env);
	// 		return (my_env.last_exit_status);
	// 	}
	// 	else
	// 	{
	// 		execute_script_file(&my_env, argv[1]);
	// 		free_environment(&my_env);
	// 		return (my_env.last_exit_status);
	// 	}
	// }
	is_interactive = isatty(STDIN_FILENO);
	if (is_interactive)
		setup_signal_handlers();
	shell_loop(&my_env, is_interactive);
	free_environment(&my_env);
	rl_clear_history();
	return (0);
}