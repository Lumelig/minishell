#include "minishell.h"

// Function to check if quotes are balanced in a line
int check_quotes_balanced(char *line) {
    int single_quotes = 0;
    int double_quotes = 0;
    int i = 0;
    
    while (line[i]) {
        if (line[i] == '\'' && double_quotes == 0) {
            single_quotes = !single_quotes;
        } else if (line[i] == '"' && single_quotes == 0) {
            double_quotes = !double_quotes;
        }
        i++;
    }
    
    // Return the type of unclosed quote, or 0 if balanced
    if (single_quotes)
        return 1; // Single quote unclosed
    if (double_quotes)
        return 2; // Double quote unclosed
    return 0; // Balanced
}

// Get the appropriate prompt for continuation
char *get_continuation_prompt(int quote_type) {
    if (quote_type == 1)
        return "quote> ";  // Single quote continuation
    else if (quote_type == 2)
        return "dquote> "; // Double quote continuation
    return "minishell$ ";  // Default prompt
}

// Main function to handle multi-line input with quote continuation


// Enhanced quote checker that handles escape sequences
int check_quotes_balanced_enhanced(char *line) 
{
    int single_quotes = 0;
    int double_quotes = 0;
    int i = 0;
    
    while (line[i]) {
        // Handle escape sequences
        if (line[i] == '\\' && line[i + 1]) {
            i += 2; // Skip escaped character
            continue;
        }
        
        if (line[i] == '\'' && double_quotes == 0) {
            single_quotes = !single_quotes;
        } else if (line[i] == '"' && single_quotes == 0) {
            double_quotes = !double_quotes;
        }
        i++;
    }
    
    if (single_quotes)
        return 1;
    if (double_quotes)
        return 2;
    return 0;
}

t_quote_state check_line_completion(char *line) 
{
    t_quote_state state = {0, 0, 0};
    int i = 0;
    
    while (line[i]) {
        if (line[i] == '\\' && line[i + 1]) {
            i += 2;
            continue;
        }
        
        if (line[i] == '\'' && !state.in_double_quote) {
            state.in_single_quote = !state.in_single_quote;
        } else if (line[i] == '"' && !state.in_single_quote) {
            state.in_double_quote = !state.in_double_quote;
        }
        i++;
    }
    
    state.needs_continuation = state.in_single_quote || state.in_double_quote;
    return state;
}

char *get_complete_input(void) {
    char *line = NULL;
    char *complete_input = NULL;
    char *temp = NULL;
    int quote_status;
    
    // Get initial input
    line = readline("minishell$ ");
    if (!line)
        return NULL;
    
    // Check if quotes are balanced
    quote_status = check_quotes_balanced(line);
    
    if (quote_status == 0) {

        return line;
    }
    
    // Quotes are unbalanced, need continuation
    complete_input = ft_strdup(line);
    free(line);
    
    while (quote_status != 0) {
        char *continuation_prompt = get_continuation_prompt(quote_status);
        line = readline(continuation_prompt);
        
        if (!line) {
            // User pressed Ctrl+D, return what we have
            break;
        }
        
        // Append newline and new input
        temp = malloc(strlen(complete_input) + strlen(line) + 2);
        if (!temp) {
            free(complete_input);
            free(line);
            return NULL;
        }
        
        sprintf(temp, "%s\n%s", complete_input, line);
        free(complete_input);
        free(line);
        complete_input = temp;
        
        // Check if quotes are now balanced
        quote_status = check_quotes_balanced(complete_input);
    }
    
    // Add to history if we have content
    if (complete_input && strlen(complete_input) > 0)
        add_history(complete_input);
    
    return complete_input;
}

// Main shell loop implementation
// void minishell_loop(void) {
//     char *input;
    
//     while (1) {
//         input = get_complete_input();
        
//         if (!input) {
//             printf("exit\n");
//             break;
//         }
        
//         if (strlen(input) == 0) {
//             free(input);
//             continue;
//         }
        
//         // Process the complete input
//         printf("Processing: %s\n", input);
        
//         // Here you would call your tokenizer and parser
//         // t_token *tokens = tokenize(input);
//         // execute_command(tokens);
        
//         free(input);
//     }
// }