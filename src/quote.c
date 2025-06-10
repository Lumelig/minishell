#include "minishell.h"

int check_quotes_balanced_enhanced(char *line) 
{
    int in_single = 0;
    int in_double = 0;
    int i = 0;
    
    while (line[i]) {
        if (in_single) {
            // Inside single quotes: NOTHING can be escaped, only look for closing quote
            if (line[i] == '\'') {
                in_single = 0;
            }
        } else if (in_double) {
            // Inside double quotes: handle specific escapes
            if (line[i] == '\\' && line[i + 1]) {
                // In double quotes, only these can be escaped: $ " \ newline
                if (line[i + 1] == '$' || line[i + 1] == '"' || 
                    line[i + 1] == '\\' || line[i + 1] == '\n') {
                    i += 2; // Skip the escaped character
                    continue;
                }
                // Other characters: backslash is literal, don't skip
            } else if (line[i] == '"') {
                in_double = 0;
            }
        } else {
            // Outside all quotes
            if (line[i] == '\\' && line[i + 1]) {
                i += 2; // Skip escaped character
                continue;
            } else if (line[i] == '\'') {
                in_single = 1;
            } else if (line[i] == '"') {
                in_double = 1;
            }
        }
        i++;
    }
    
    if (in_single)
        return 1;  // Unclosed single quote
    else if (in_double)
        return 2;  // Unclosed double quote
    else
        return 0;  // All quotes balanced
}

// Get the appropriate prompt for continuation
char *get_continuation_prompt(int quote_type) {
    if (quote_type == 1)
        return "quote> ";     // Single quote continuation (like bash)
    else if (quote_type == 2)
        return "dquote> ";    // Double quote continuation (like bash)
    else
        return "minishell$ "; // Default prompt
}

t_quote_state check_line_completion(char *line) 
{
    t_quote_state state = {0, 0, 0};
    int i = 0;
    
    while (line[i]) {
        if (state.in_single) {
            // Inside single quotes: NOTHING can be escaped
            if (line[i] == '\'') {
                state.in_single = 0;
            }
        } else if (state.in_double) {
            // Inside double quotes: handle specific escapes
            if (line[i] == '\\' && line[i + 1]) {
                // In double quotes, only these can be escaped: $ " \ newline
                if (line[i + 1] == '$' || line[i + 1] == '"' || 
                    line[i + 1] == '\\' || line[i + 1] == '\n') {
                    i += 2; // Skip the escaped character
                    continue;
                }
            } else if (line[i] == '"') {
                state.in_double = 0;
            }
        } else {
            // Outside all quotes
            if (line[i] == '\\' && line[i + 1]) {
                i += 2; // Skip escaped character
                continue;
            } else if (line[i] == '\'') {
                state.in_single = 1;
            } else if (line[i] == '"') {
                state.in_double = 1;
            }
        }
        i++;
    }
    
    state.continuation = state.in_single || state.in_double;
    return state;
}

char *get_complete_input(void) {
    char *line = NULL;
    char *complete_input = NULL;
    char *temp = NULL;
    int quote_status;
    
    line = readline(">");  // Add default prompt
    if (!line)
        return NULL;
    
    // Check if quotes are balanced
    quote_status = check_quotes_balanced_enhanced(line);
    
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
        
        // Proper string concatenation with newline
		temp = ft_strjoin(complete_input, line);
        //sprintf(temp, "%s\n%s", complete_input, line);
        //free(complete_input);
        free(line);
        //complete_input = temp;
        
        // Check if quotes are now balanced
        quote_status = check_quotes_balanced_enhanced(complete_input);
    }
    
    return temp;
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