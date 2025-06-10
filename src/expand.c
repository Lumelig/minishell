# include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glob.h>
#include <pwd.h>
#include <sys/types.h>

//Need to be cleaner

// Function to expand environment variables in a string
char *expand_variables(char *str) {
    if (!str) return NULL;
    
    char *result = malloc(strlen(str) * 4 + 1); // Allocate extra space for expansion
    if (!result) return NULL;
    
    char *src = str;
    char *dst = result;
    
    while (*src) {
        if (*src == '$') {
            src++; // Skip $
            
            if (*src == '{') {
                // Handle ${VAR} format
                src++; // Skip {
                char *var_start = src;
                while (*src && *src != '}') 
					src++;

                if (*src == '}') {
                    char var_name[256];
                    int var_len = src - var_start;
                    if (var_len < 256) {
                        strncpy(var_name, var_start, var_len);
                        var_name[var_len] = '\0';
                        
                        char *env_val = getenv(var_name);
                        if (env_val) {
                            strcpy(dst, env_val);
                            dst += ft_strlen(env_val);
                        }
                    }
                    src++; // Skip }
                } else {
                    // Malformed ${, treat as literal
                    *dst++ = '$';
                    *dst++ = '{';
                }
            } else if (*src == '?') {
                // Handle $? (exit status)
                // You'll need to implement exit status tracking
                char exit_code[12];
                sprintf(exit_code, "%d", 0); // Replace with actual exit status
                strcpy(dst, exit_code);
                dst += ft_strlen(exit_code);
                src++;
            } else if (*src == '$') {
                // Handle $$ (process ID)
                char pid_str[12];
                sprintf(pid_str, "%d", getpid());
                strcpy(dst, pid_str);
                dst += ft_strlen(pid_str);
                src++;
            } else {
                // Handle $VAR format (alphanumeric and underscore)
                char *var_start = src;
                while (*src && (ft_isalnum(*src) || *src == '_')) src++;
                
                if (src > var_start) {
                    char var_name[256];
                    int var_len = src - var_start;
                    if (var_len < 256) {
                        strncpy(var_name, var_start, var_len);
                        var_name[var_len] = '\0';
                        
                        char *env_val = getenv(var_name);
                        if (env_val) {
                            strcpy(dst, env_val);
                            dst += ft_strlen(env_val);
                        }
                    }
                } else {
                    // Just a $ with no valid variable name
                    *dst++ = '$';
                }
            }
        } else {
            *dst++ = *src++;
        }
    }
    
    *dst = '\0';
    
    // Reallocate to actual size
    char *final_result = ft_strdup(result);
    free(result);
    return final_result;
}

// Function to expand tilde (~)
char *expand_tilde(char *str) {
    if (!str || str[0] != '~') {
        return ft_strdup(str);
    }
    
    char *result;
    char *home_dir = NULL;
    
    if (str[1] == '\0' || str[1] == '/') {
        // ~/... format
        home_dir = getenv("HOME");
        if (!home_dir) {
			//change getpwuid
            struct passwd *pw = getpwuid(getuid());
            home_dir = pw ? pw->pw_dir : "/";
        }
        
        if (str[1] == '\0') {
            result = ft_strdup(home_dir);
        } else {
            result = malloc(ft_strlen(home_dir) + ft_strlen(str + 1) + 1);
            if (result) {
                strcpy(result, home_dir);
                strcat(result, str + 1);
            }
        }
    } else {
        // ~username/... format
        char *slash = ft_strchr(str + 1, '/');
        char username[256];
        int username_len;
        
        if (slash) {
            username_len = slash - (str + 1);
        } else {
            username_len = ft_strlen(str + 1);
        }
        
        if (username_len < 256) {
            strncpy(username, str + 1, username_len);
            username[username_len] = '\0';
            //change getpwam
            struct passwd *pw = getpwnam(username);
            if (pw) {
                if (slash) {
                    result = malloc(ft_strlen(pw->pw_dir) + ft_strlen(slash) + 1);
                    if (result) {
                        strcpy(result, pw->pw_dir);
                        strcat(result, slash);
                    }
                } else {
                    result = ft_strdup(pw->pw_dir);
                }
            } else {
                // User not found, return original
                result = ft_strdup(str);
            }
        } else {
            result = ft_strdup(str);
        }
    }
    
    return result;
}

// Function to expand wildcards using glob
t_token *expand_wildcards(t_token *token) {
    if (!token || token->type != TOKEN_WORD || 
        (!ft_strchr(token->value, '*') && !ft_strchr(token->value, '?') && !ft_strchr(token->value, '['))) {
        return token;
    }
    
    glob_t glob_result;
    int flags = GLOB_TILDE | GLOB_BRACE;
    
	//chang glob
    if (glob(token->value, flags, NULL, &glob_result) == 0) {
        if (glob_result.gl_pathc > 0) {
            // Create new tokens for each match
            t_token *first_new = NULL;
            t_token *last_new = NULL;
            
            for (size_t i = 0; i < glob_result.gl_pathc; i++) {
                t_token *new_token = malloc(sizeof(t_token));
                if (!new_token) {
                    globfree(&glob_result);
                    return token; // Return original on failure
                }
                
                new_token->type = TOKEN_WORD;
                new_token->value = ft_strdup(glob_result.gl_pathv[i]);
                new_token->next = NULL;
                
                if (!first_new) {
                    first_new = new_token;
                    last_new = new_token;
                } else {
                    last_new->next = new_token;
                    last_new = new_token;
                }
            }
            
            // Link the last new token to the original token's next
            if (last_new) {
                last_new->next = token->next;
            }
            
            // Free the original token
            free(token->value);
            free(token);
            
            globfree(&glob_result);
            return first_new;
        }
    }
    
    globfree(&glob_result);
    return token; // Return original if no matches or error
}

// Main expansion function
t_token *expand_tokens(t_token *tokens) {
    if (!tokens) 
		return NULL;
    
    t_token *current = tokens;
    t_token *head = NULL;
    t_token *tail = NULL;
    
    while (current) {
        t_token *next = current->next;
        current->next = NULL; // Disconnect for processing
        
        if (current->type == TOKEN_WORD) {
            // Step 1: Variable expansion
            char *expanded_vars = expand_variables(current->value);
            if (expanded_vars) {
                free(current->value);
                current->value = expanded_vars;
            }
            
            // Step 2: Tilde expansion
            char *expanded_tilde = expand_tilde(current->value);
            if (expanded_tilde) {
                free(current->value);
                current->value = expanded_tilde;
            }
            
            // Step 3: Wildcard expansion (can return multiple tokens)
            t_token *expanded_wildcards = expand_wildcards(current);
            
            // Add expanded tokens to result list
            if (!head) {
                head = expanded_wildcards;
                tail = expanded_wildcards;
            } else {
                tail->next = expanded_wildcards;
            }
            
            // Find the new tail
            while (tail && tail->next) {
                tail = tail->next;
            }
        } else {
            // Non-word tokens pass through unchanged
            if (!head) {
                head = current;
                tail = current;
            } else {
                tail->next = current;
                tail = current;
            }
        }
        
        current = next;
    }
    
    return head;
}

// Helper function to split expanded variables by spaces if needed
// This handles cases where a variable expands to multiple words
t_token *split_expanded_words(t_token *tokens) {
    if (!tokens) return NULL;
    
    t_token *current = tokens;
    t_token *head = NULL;
    t_token *tail = NULL;
    
    while (current) {
        t_token *next = current->next;
        current->next = NULL;
        
        if (current->type == TOKEN_WORD && ft_strchr(current->value, ' ')) {
            // Split this token by spaces
            char *str = ft_strdup(current->value);
			//strtok
            char *token_str = strtok(str, " \t\n");
            
            while (token_str) {
                t_token *new_token = malloc(sizeof(t_token));
                if (!new_token) break;
                
                new_token->type = TOKEN_WORD;
                new_token->value = ft_strdup(token_str);
                new_token->next = NULL;
                
                if (!head) {
                    head = new_token;
                    tail = new_token;
                } else {
                    tail->next = new_token;
                    tail = new_token;
                }
                
                token_str = strtok(NULL, " \t\n");
            }
            
            free(str);
            free(current->value);
            free(current);
        } else {
            // Add token as-is
            if (!head) {
                head = current;
                tail = current;
            } else {
                tail->next = current;
                tail = current;
            }
        }
        
        current = next;
    }
    
    return head;
}

// Complete expansion function with word splitting
t_token *expand_and_split_tokens(t_token *tokens) {
    t_token *expanded = expand_tokens(tokens);
    return split_expanded_words(expanded);
}