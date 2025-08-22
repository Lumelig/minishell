#include "minishell.h"

// Function to convert redirection type to string for printing
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

// Print a single file list
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

// Bonus: Print just the file list (standalone)
void print_standalone_file_list(t_file_list *file_list)
{
    if (!file_list)
    {
        printf("File List: NULL\n");
        return;
    }
    
    printf("=== FILE LIST ===\n");
    print_file_list(file_list);
    printf("=== END FILE LIST ===\n");
}

static char	**get_input(int is_interactive)
{
	char	*input;
	char 	**input_arry;

	if (is_interactive)
	{
		//
		input = get_complete_input();
		input_arry = ft_split(input, '\n');
		free(input);
	}
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (input && input[ft_strlen(input) - 1] == '\n')
			input[ft_strlen(input) - 1] = '\0';
		input_arry = malloc(sizeof(char *) * 2);
		input_arry[0] = input;
		input_arry[1] = NULL;
	}
	return (input_arry);
}

static void	shell_loop(t_env *my_env, int is_interactive)
{
	t_token		*token;
	t_cmd_list	*cmd_list;
	char		**input;
	int			i;

	while (1)
	{
		input = get_input(is_interactive);
		if (!input)
		{
			if (is_interactive)
				printf("exit\n");
			break;
		}

		i = 0;
		while (input[i])
		{
			if (empty_input(input[i]))
			{
				i++;
				continue;
			}

			token = tokenize(input[i]);
			add_history(input[i]);
			cmd_list = parsing(my_env, token);

			if (*exit_code() == 0)
				executor(cmd_list, my_env);

			cleanup_and_exit(token, NULL);  // free token, keep input for now
			i++;
		}

		// free the entire input array
		i = 0;
		while (input[i])
		{
			free(input[i]);
			i++;
		}
		free(input);
	}
}
static const char	*get_token_name(t_token_type type)
{
	static const char *token_names[] = {
		"WORD",
		"PIPE",
		"REDIR_IN",
		"REDIR_OUT", 
		"REDIR_APPEND",
		"HEREDOC",
		"END_CMD",
		"EOF",
		"UNKNOWN"
	};
	
	// Assuming your token types are sequential starting from 0
	if (type >= 0 && type < (sizeof(token_names) / sizeof(token_names[0]) - 1))
		return (token_names[type]);
	return (token_names[sizeof(token_names) / sizeof(token_names[0]) - 1]); // "UNKNOWN"
}

void	print_tokens(t_token *head)
{
	t_token	*current;
	int		index;

	if (!head)
	{
		printf("No tokens found.\n");
		return ;
	}
	
	printf("=== TOKENS ===\n");
	current = head;
	index = 0;
	
	while (current)
	{
		printf("[%d] Type: %-12s | Value: '%s'\n", 
			index, 
			get_token_name(current->type), 
			current->value ? current->value : "(null)");
		current = current->next;
		index++;
	}
	printf("==============\n");
}