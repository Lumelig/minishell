#include "minishell.h"

bool empty_input(char *input)
{
    int i;

    if (!input)
        return (true);
    
    i = 0;
    while (input[i] && ft_isspace(input[i]))
        i++;
    
    // Don't free input here - let caller handle it
    return (input[i] == '\0');
}

//delet later
void print_history(void)
{
    HIST_ENTRY **the_list;
    int i = 0;

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
void free_tokens(t_token *token)
{
    t_token *temp;
    
    while (token)
    {
        temp = token->next;
        free(token->value);
        free(token);
        token = temp;
    }
}

// void	print_env_list(t_envlist *head)
// {
// 	t_envlist *current = head;

// 	while (current)
// 	{
// 		printf("%s%s\n", current->key, current->value ? current->value : "");
// 		current = current->next;
// 	}
// }

int main(int argc, char **argv, char **env)
{
    t_token *token;
    char *input;
    char *cwd;
	t_env	my_env;

    // Initialize readline history
    using_history();
	init_environment(&my_env, env, argv, argc);
	//print_env_list(my_env.head);
	//setup_signal_handlers()
    while (1)
    {
        // Get current working directory for prompt
        cwd = getcwd(NULL, 0);
        if (cwd)
        {
            printf("%s ", cwd);
            free(cwd);
        }
        else
        {
            printf("minihell$ ");
        }

        // Get input (this handles quote continuation)
        input = get_complete_input();
        
        // Check if user pressed Ctrl+D
        if (!input)
        {
            printf("exit\n");
            break;
        }

        // Check if input is empty (only whitespace)
        if (empty_input(input))
        {
            free(input);
            continue;
        }

        // Add to history if not empty
        add_history(input);

        // Debug: print history (remove this in production)
        print_history();

        // Tokenize input
        token = tokenize(input);
		
		token = expand_and_split_tokens(token);
        
        // Debug: print tokens (remove this in production)
        t_token *current = token;
        while (current)
        {
            printf("Token: '%s'\n", current->value);
            current = current->next;
        }

        // Check for exit command
        if (token && !ft_strncmp("exit", token->value, 5) && 
            ft_strlen(token->value) == 4)
        {
            free_tokens(token);
            free(input);
            break;
        }

        // Here you would execute the command
        // execute_command(token);

        // Clean up
        free_tokens(token);
        free(input);
    }

    // Clean up history
    clear_history();
    return (0);
}

// Alternative version with better prompt handling
// int main_alternative(int argc, char **argv, char **env)
// {
//     t_token *token;
//     char *input;
//     char prompt[1024];
//     (void)argc;
//     (void)argv;
//     (void)env;

//     using_history();

//     while (1)
//     {
//         // Create a nice prompt with current directory
//         char *cwd = getcwd(NULL, 0);
//         if (cwd)
//         {
//             snprintf(prompt, sizeof(prompt), "\033[1;32m%s\033[0m$ ", basename(cwd));
//             free(cwd);
//         }
//         else
//         {
//             strcpy(prompt, "minishell$ ");
//         }

//         // Use readline with custom prompt
//         input = readline(prompt);
        
//         if (!input)
//         {
//             printf("exit\n");
//             break;
//         }

//         if (empty_input(input))
//         {
//             free(input);
//             continue;
//         }

//         // Handle quote continuation
//         int quote_status = check_quotes_balanced(input);
//         char *complete_input = input;
        
//         while (quote_status != 0)
//         {
//             char *cont_prompt = get_continuation_prompt(quote_status);
//             char *continuation = readline(cont_prompt);
            
//             if (!continuation)
//                 break;
                
//             char *temp = malloc(strlen(complete_input) + strlen(continuation) + 2);
//             sprintf(temp, "%s\n%s", complete_input, continuation);
            
//             if (complete_input != input)
//                 free(complete_input);
//             free(continuation);
//             complete_input = temp;
            
//             quote_status = check_quotes_balanced(complete_input);
//         }

//         add_history(complete_input);
        
//         token = tokenize(complete_input);
        
//         if (token && !strcmp(token->value, "exit"))
//         {
//             free_tokens(token);
//             if (complete_input != input)
//                 free(complete_input);
//             free(input);
//             break;
//         }

//         // Execute command here
//         // execute_command(token);

//         free_tokens(token);
//         if (complete_input != input)
//             free(complete_input);
//         free(input);
//     }

//     clear_history();
//     return (0);
// }