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

const char *redir_type_to_string(int redir_type)
{
    switch (redir_type)
    {
        case TOKEN_REDIR_IN:
            return "<";
        case TOKEN_REDIR_OUT:
            return ">";
        case TOKEN_REDIR_APPEND:
            return ">>";
        case TOKEN_HEREDOC:
            return "<<";
        default:
            return "UNKNOWN";
    }
}
void print_file_list(t_file_list *file_list)
{
    t_file_node *current;
    int i;

    if (!file_list)
    {
        printf("  Files: NULL\n");
        return;
    }
    
    printf("  Files (size: %zd):\n", file_list->size);
    
    if (file_list->size == 0)
    {
        printf("    (no files)\n");
        return;
    }
    
    current = file_list->head;
    i = 0;
    while (current)
    {
        printf("    [%d] %s %s\n", 
               i, 
               redir_type_to_string(current->redir_type),
               current->filename);
        current = current->next;
        i++;
    }
}

// Print a single command node
void print_cmd_node(t_cmd_node *cmd_node, int cmd_index)
{
    int i;

    if (!cmd_node)
    {
        printf("Command [%d]: NULL\n", cmd_index);
        return;
    }
    
    printf("Command [%d]:\n", cmd_index);
    printf("  Type: %d\n", cmd_node->cmd_type);
    
    // Print command arguments
    if (cmd_node->cmd)
    {
        printf("  Args: ");
        i = 0;
        while (cmd_node->cmd[i])
        {
            printf("'%s'", cmd_node->cmd[i]);
            if (cmd_node->cmd[i + 1])
                printf(" ");
            i++;
        }
        printf("\n");
    }
    else
    {
        printf("  Args: NULL\n");
    }
    
    // Print file list
    print_file_list(cmd_node->files);
    printf("\n");
}

// Print the entire command list
void print_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_node *current;
    int i;

    if (!cmd_list)
    {
        printf("Command List: NULL\n");
        return;
    }
    
    printf("=== COMMAND LIST ===\n");
    printf("Total commands: %zd\n\n", cmd_list->size);
    
    if (cmd_list->size == 0)
    {
        printf("(empty command list)\n");
        return;
    }
    
    current = cmd_list->head;
    i = 0;
    while (current)
    {
        print_cmd_node(current, i);
        current = current->next;
        i++;
    }
    printf("=== END COMMAND LIST ===\n");
}
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
		printf("%s%c%s\n", current->key, '=', current->value);
		current = current->next;
	}
}

static void	cleanup_and_exit(t_token *token, char *input)
{
	free_tokens(token);
	free(input);
}

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
	t_token		*token;
	t_cmd_list	*cmd_list;
	char		*input;

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
		token = tokenize(input);
		//print_tokens(token);
		cmd_list = parsing(my_env, token);
		//print_cmd_list(cmd_list);
		if (*exit_code() == 0)
			executor(cmd_list, my_env);
		cleanup_and_exit(token, input);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_env	my_env;
	int		is_interactive;

	init_environment(&my_env, env, argv, argc);
	// if (argc > 1) TODO: to consider for later
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
