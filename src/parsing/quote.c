#include "minishell.h"

int check_quotes_balanced_enhanced(char *line) 
{
    int in_single = 0;
    int in_double = 0;
    int i = 0;
    
    while (line[i]) {
        if (in_single) {
            // Inside single quotes: NOTHING can be escaped, only look for closing single quote
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
    
    line = readline("minishell$ ");  // Use proper default prompt
    if (!line)
        return NULL;
    
    // Check if quotes are balanced
    quote_status = check_quotes_balanced_enhanced(line);
    
    if (quote_status == 0) {
        // Quotes are balanced, add to history and return
        if (*line)  // Only add non-empty lines to history
            add_history(line);
        return line;
    }
    
    // Quotes are unbalanced, need continuation
    complete_input = ft_strdup(line);
    if (!complete_input) {
        free(line);
        return NULL;
    }
    free(line);
    
    while (quote_status != 0) {
        line = readline(get_continuation_prompt(quote_status));
        
        if (!line) {
            // User pressed Ctrl+D, return what we have so far
            break;
        }
        
        // Create new string with newline separator
        size_t complete_len = strlen(complete_input);
        size_t line_len = strlen(line);
        temp = malloc(complete_len + line_len + 2); // +2 for \n and \0
        
        if (!temp) {
            free(complete_input);
            free(line);
            return NULL;
        }
        
        // Copy complete_input, add newline, then add new line
        strcpy(temp, complete_input);
        temp[complete_len] = '\n';
        strcpy(temp + complete_len + 1, line);
        
        // Free old strings and update
        free(complete_input);
        free(line);
        complete_input = temp;
        
        // Check if quotes are now balanced
        quote_status = check_quotes_balanced_enhanced(complete_input);
    }
    
    // Add complete input to history
    if (complete_input && *complete_input)
        add_history(complete_input);
    
    return complete_input;
}
