
#include "minishell.h"

bool	empty_input(char *input)
{
	int	i;

	if (!input)
		return (true);
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	// Don't free input here - let caller handle it
	return (input[i] == '\0');
}

// delet later
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

// Function to free token list
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

int	main(int argc, char **argv, char **env)
{
	t_token	*token;
	char	*input;
	t_env	my_env;
	//t_token	*current;

	init_environment(&my_env, env, argv, argc);
	//print_env_list(my_env.head);
	setup_signal_handlers();
	while (1)
	{
		input = get_complete_input();
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (empty_input(input))
		{
			free(input);
			continue ;
		}
		print_history();
		token = tokenize(input);
		token = expand_and_split_tokens(token, my_env);
		parse_tokens(token);
		//current = token;
		// while (current)
		// {
		// 	printf("Token: '%s'\n", current->value);
		// 	current = current->next;
		// }
		if (token && !ft_strncmp("exit", token->value, 5)
			&& ft_strlen(token->value) == 4)
		{
			free_tokens(token);
			free(input);
			break ;
		}
		free_tokens(token);
		free(input);
	}
	free_tokens(token);
	free(input);
	free_environment(&my_env);
	rl_clear_history();
	return (0);
}
