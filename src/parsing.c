#include "minishell.h"



// Helper function to clean up tokens on error
static t_token *cleanup_tokens(t_token *head) {
    t_token *current = head;
    t_token *next;
    
    while (current) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    
    return NULL;
}

// Enhanced add_token function with error checking
static int add_token(t_token **head, t_token_type type, const char *value) {
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return 0;  // Memory allocation failed
    
    new_token->type = type;
    new_token->value = ft_strdup(value);
    if (!new_token->value) {
        free(new_token);
        return 0;  // String duplication failed
    }
    
    new_token->next = NULL;
    
    // Add to end of list to preserve order
    if (!*head) {
        *head = new_token;
    } else {
        t_token *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
    
    return 1;  // Success
}
t_token *tokenize(char *line) {
    if (!line)
        return NULL;
    
    int i = 0;
    t_token *head = NULL;
    
    while (line[i]) {
        // Skip whitespace
        if (ft_isspace(line[i])) {
            i++;
            continue;
        }
        
        // Handle operators and redirections
        if (line[i] == '|') {
            if (!add_token(&head, TOKEN_PIPE, "|"))
                return cleanup_tokens(head);
            i++;
        } 
        else if (line[i] == '>') {
            if (line[i + 1] == '>') {
                if (!add_token(&head, TOKEN_REDIR_APPEND, ">>"))
                    return cleanup_tokens(head);
                i += 2;
            } else {
                if (!add_token(&head, TOKEN_REDIR_OUT, ">"))
                    return cleanup_tokens(head);
                i++;
            }
        } 
        else if (line[i] == '<') {
            if (line[i + 1] == '<') {
                if (!add_token(&head, TOKEN_HEREDOC, "<<"))
                    return cleanup_tokens(head);
                i += 2;
            } else {
                if (!add_token(&head, TOKEN_REDIR_IN, "<"))
                    return cleanup_tokens(head);
                i++;
            }
        } 
        else {
            // Handle quoted strings and words
            int start = i;
            char quote_char = 0;
            
            // Check if this is a quoted string
            if (line[i] == '"' || line[i] == '\'') {
                quote_char = line[i];
                i++; // Skip opening quote
                start = i; // Start after the quote
                
                // Find closing quote
                while (line[i] && line[i] != quote_char) {
                    i++;
                }
                
                if (line[i] == quote_char) {
                    // Found closing quote
                    if (i > start) {  // Only create token if we have content
                        char *word = substr(line, start, i - start);// change substr
                        if (!word || !add_token(&head, TOKEN_WORD, word)) {
                            free(word);
                            return cleanup_tokens(head);
                        }
                        free(word);
                    } else {
                        // Empty quoted string ""
                        if (!add_token(&head, TOKEN_WORD, "")) {
                            return cleanup_tokens(head);
                        }
                    }
                    i++; // Skip closing quote
                } else {
                    // Unclosed quote - treat as regular word from original start
                    i = start - 1; // Go back to include the quote
                    while (line[i] && !ft_isspace(line[i]) && 
                           line[i] != '|' && line[i] != '<' && line[i] != '>') {
                        i++;
                    }
                    
                    char *word = ft_substr(line, start - 1, i - (start - 1));
                    if (!word || !add_token(&head, TOKEN_WORD, word)) {
                        free(word);
                        return cleanup_tokens(head);
                    }
                    free(word);
                }
            } else {
                // Regular unquoted word
                while (line[i] && !ft_isspace(line[i]) && 
                       line[i] != '|' && line[i] != '<' && line[i] != '>') {
                    i++;
                }
                
                if (i > start) {  // Only create token if we have content
                    char *word = ft_substr(line, start, i - start);
                    if (!word || !add_token(&head, TOKEN_WORD, word)) {
                        free(word);
                        return cleanup_tokens(head);
                    }
                    free(word);
                }
            }
        }
    }
    
    return head;
}