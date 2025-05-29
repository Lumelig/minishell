#include "minishell.h"

int check_quotes_balanced_enhanced(char *line) 
{
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    int i = 0;
    
    while (line[i]) {
        // Handle escape sequences
        if (line[i] == '\\' && line[i + 1]) {
            i += 2; // Skip escaped character
            continue;
        }
        
        // Only toggle single quotes if we're NOT inside double quotes
        if (line[i] == '\'' && !in_double_quotes) {
            in_single_quotes = !in_single_quotes;
        } 
        // Only toggle double quotes if we're NOT inside single quotes
        else if (line[i] == '"' && !in_single_quotes) {
            in_double_quotes = !in_double_quotes;
        }
        i++;
    }
    
    if (in_single_quotes)
        return 1;  // Unclosed single quote
    if (in_double_quotes)
        return 2;  // Unclosed double quote
    return 0;      // All quotes balanced
}

// Get the appropriate prompt for continuation
char *get_continuation_prompt(int quote_type) {
    if (quote_type == 1)
        return "quote> ";  // Single quote continuation
    else if (quote_type == 2)
        return "dquote> "; // Double quote continuation
    return "minishell$ ";  // Default prompt
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
        
        // Only toggle single quotes if we're NOT inside double quotes
        if (line[i] == '\'' && !state.in_double_quote) {
            state.in_single_quote = !state.in_single_quote;
        } 
        // Only toggle double quotes if we're NOT inside single quotes
        else if (line[i] == '"' && !state.in_single_quote) {
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
    
    line = readline("");
    if (!line)
        return NULL;
    
    // Check if quotes are balanced
    quote_status = check_quotes_balanced_enhanced(line);  // Fixed function call
    
    if (quote_status == 0) {
        return line;
    }
    
    // Quotes are unbalanced, need continuation
    complete_input = ft_strdup(line);
    free(line);
    
    while (quote_status != 0) {
        line = readline(get_continuation_prompt(quote_status));
        
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
        
        // Fixed: proper string concatenation with newline
        sprintf(temp, "%s\n%s", complete_input, line);
        free(complete_input);
        free(line);
        complete_input = temp;
        
        // Check if quotes are now balanced
        quote_status = check_quotes_balanced_enhanced(complete_input);
    }
    
    return (complete_input);
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