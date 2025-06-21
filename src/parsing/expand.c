#include "minishell.h"

// Calculate the required buffer size for expansion
static int calculate_expanded_size(char *original, t_envlist *envlist, t_env *env)
{
    int len = ft_strlen(original);
    int result_size = 0;
    int i = 0;
    
    while (i < len)
    {
        if (original[i] == '$')
        {
            if (i + 1 < len && original[i + 1] == '$')
            {
                // $$ expands to PID
                char *pid_str = ft_itoa(env->pid);
                result_size += ft_strlen(pid_str);
                free(pid_str);
                i += 2;
            }
            else if (i + 1 < len && original[i + 1] == '?')
            {
                // $? expands to exit status
                char *status_str = ft_itoa(env->exit_status);
                result_size += ft_strlen(status_str);
                free(status_str);
                i += 2;
            }
            else if (i + 1 < len && (ft_isalpha(original[i + 1]) || original[i + 1] == '_'))
            {
                // $VARIABLE
                int var_start = i + 1;
                int var_end = var_start;
                
                while (var_end < len && (ft_isalnum(original[var_end]) || original[var_end] == '_'))
                    var_end++;
                
                char *var_name = ft_substr(original, var_start, var_end - var_start);
                t_envlist *current = envlist;
                char *var_value = NULL;
                
                while (current)
                {
                    if (ft_strcmp(current->key, var_name) == 0)
                    {
                        var_value = current->value;
                        break;
                    }
                    current = current->next;
                }
                
                if (var_value)
                    result_size += ft_strlen(var_value);
                // If not found, adds 0 (empty expansion)
                
                free(var_name);
                i = var_end;
            }
            else
            {
                // Regular $ character
                result_size++;
                i++;
            }
        }
        else
        {
            result_size++;
            i++;
        }
    }
    
    return result_size + 1; // +1 for null terminator
}

char *expand_token_value(char *original, t_envlist *envlist, t_env *env)
{
    int len;
    int i = 0, j = 0;
    char *tmp;
    int var_start;
    int var_end;
    t_envlist *current;
    char *var_value;
    char *result;
    int result_size;

    if (!original || !ft_strchr(original, '$'))
        return (ft_strdup(original)); // No $ found, return copy

    len = ft_strlen(original);
    
    // Calculate the actual required size
    result_size = calculate_expanded_size(original, envlist, env);
    result = malloc(result_size);
    
    if (!result)
        return (NULL);

    while (i < len)
    {
        if (original[i] == '$')
        {
            if (i + 1 < len && original[i + 1] == '$')
            {
                // Handle $$
                tmp = ft_itoa(env->pid);
                ft_strcpy(result + j, tmp);
                j += ft_strlen(tmp);
                free(tmp);
                i += 2;
            }
            else if (i + 1 < len && original[i + 1] == '?')
            {
                // Handle $?
                tmp = ft_itoa(env->exit_status);
                ft_strcpy(result + j, tmp);
                j += ft_strlen(tmp);
                free(tmp);
                i += 2;
            }
            else if (i + 1 < len && (ft_isalpha(original[i + 1]) || original[i + 1] == '_'))
            {
                // Handle $VARIABLE
                var_start = i + 1;
                var_end = var_start;
                
                // Find end of variable name
                while (var_end < len && (ft_isalnum(original[var_end]) || original[var_end] == '_'))
                    var_end++;
                
                // Extract variable name
                tmp = ft_substr(original, var_start, var_end - var_start);
                
                // Find in environment
                current = envlist;
                var_value = NULL;
                while (current)
                {
                    if (ft_strcmp(current->key, tmp) == 0)
                    {
                        var_value = current->value;
                        break;
                    }
                    current = current->next;
                }
                
                // Copy value or empty string if not found
                if (var_value)
                {
                    ft_strcpy(result + j, var_value);
                    j += ft_strlen(var_value);
                }
                // If not found, add nothing (empty expansion)
                
                free(tmp);
                i = var_end;
            }
            else
            {
                // Just a regular $ character
                result[j++] = original[i++];
            }
        }
        else
        {
            result[j++] = original[i++];
        }
    }
    
    result[j] = '\0';
    return (result);
}

void expand_tokens(t_token *token, t_envlist *envlist, t_env *env)
{
    t_token *current;
    char *expanded;

    current = token;
    while (current)
    {
        if (current->type == TOKEN_WORD)
        {
            expanded = expand_token_value(current->value, envlist, env);
            if (expanded)
            {
                free(current->value);
                current->value = expanded;
                
                // Additional check: if expansion resulted in empty string,
                // you might want to handle this case specially
                if (ft_strlen(expanded) == 0)
                {
                    // Handle empty expansion - maybe mark token for removal
                    // or change its type, depending on your shell's behavior
                }
            }
        }
        current = current->next;
    }
}