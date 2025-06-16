#include "minishell.h"

void init_empty_env(t_env *env, char **argv, int argc)
{
    (void)argv;
    (void)argc;
    env->size = 0;
    env->shlvl = 1;
    env->head = NULL;
    env->tail = NULL;
    env->pid = getpid();
    env->exit_status = 0;
}

void set_environment(t_env *my_env, char **key, char **value, char *env)
{
    char *sep;
    char *val_raw;
    int shlvl;

    sep = ft_strchr(env, '=');
    if (!sep)
    {
        *key = ft_strdup(env);
        *value = ft_strdup(""); // Empty value instead of NULL
        return;
    }

    *key = ft_substr(env, 0, sep - env);
    val_raw = sep + 1;

    if (!ft_strcmp(*key, "SHLVL"))
    {
        shlvl = ft_atoi(val_raw) + 1;
        my_env->shlvl = shlvl;
        *value = ft_itoa(shlvl); // Don't add '=' here - it's handled in add_env_var
    }
    else
    {
        *value = ft_strdup(val_raw); // Just duplicate the value, no '='
    }
}

bool add_env_var(t_env *env, char *key, char *value)
{
    t_envlist *new_node;

    if (!key) // Safety check
        return false;

    new_node = (t_envlist *)malloc(sizeof(t_envlist));
    if (!new_node)
        return false;
    
    new_node->key = key;
    new_node->delimiter = '=';
    new_node->value = value; // Can be NULL for variables without values
    new_node->next = NULL;
    
    if (!env->head)
    {
        env->head = new_node;
        env->tail = new_node;
    }
    else
    {
        env->tail->next = new_node;
        env->tail = new_node;
    }
    env->size++;
    return true;
}

// Helper function to find an environment variable
t_envlist *find_env_var(t_env *env, const char *key)
{
    t_envlist *current = env->head;
    
    while (current)
    {
        if (!ft_strcmp(current->key, key))
            return current;
        current = current->next;
    }
    return NULL;
}

// Helper function to update or add an environment variable
bool update_or_add_env_var(t_env *env, char *key, char *value)
{
    t_envlist *existing = find_env_var(env, key);
    
    if (existing)
    {
        // Update existing variable
        free(existing->value);
        existing->value = value;
        free(key); // We don't need the new key since we're updating
        return true;
    }
    else
    {
        // Add new variable
        return add_env_var(env, key, value);
    }
}

// Fixed initialization function
bool init_environment(t_env *my_env, char **env, char **argv, int argc)
{
    int i;
    char *key;
    char *value;

    init_empty_env(my_env, argv, argc);
    
    if (!env || !env[0])
    {
        // No environment provided, create minimal environment
        if (!add_env_var(my_env, ft_strdup("PWD"), ft_strdup(getcwd(NULL, 0))))
            return false;
        if (!add_env_var(my_env, ft_strdup("SHLVL"), ft_strdup("1")))
            return false;
        if (!add_env_var(my_env, ft_strdup("_"), ft_strdup("/usr/bin/minishell")))
            return false;
        return true;
    }

    i = 0;
    while (env[i])
    {
        // Skip OLDPWD as bash does
        if (!ft_strncmp(env[i], "OLDPWD=", 7))
        {
            i++;
            continue;
        }
        
        key = NULL;
        value = NULL;
        set_environment(my_env, &key, &value, env[i]);
        
        if (!key)
        {
            i++;
            continue;
        }
        
        if (!add_env_var(my_env, key, value))
        {
            free(key);
            free(value);
            return false;
        }
        i++;
    }

    // Ensure SHLVL exists
    if (!find_env_var(my_env, "SHLVL"))
    {
        if (!add_env_var(my_env, ft_strdup("SHLVL"), ft_strdup("1")))
            return false;
    }

    // Set PWD if not present
    if (!find_env_var(my_env, "PWD"))
    {
        char *cwd = getcwd(NULL, 0);
        if (cwd)
        {
            if (!add_env_var(my_env, ft_strdup("PWD"), cwd))
            {
                free(cwd);
                return false;
            }
        }
    }

    return true;
}

// Function to convert your env list back to char** format (useful for execve)
char **env_to_array(t_env *env)
{
    char **env_array;
    t_envlist *current;
    int i;

    if (!env || env->size == 0)
        return NULL;

    env_array = malloc(sizeof(char *) * (env->size + 1));
    if (!env_array)
        return NULL;

    current = env->head;
    i = 0;
    while (current && i < env->size)
    {
        size_t key_len = strlen(current->key);
        size_t val_len = current->value ? strlen(current->value) : 0;
        
        env_array[i] = malloc(key_len + val_len + 2); // +2 for '=' and '\0'
        if (!env_array[i])
        {
            // Cleanup on failure
            while (--i >= 0)
                free(env_array[i]);
            free(env_array);
            return NULL;
        }
        
        if (current->value)
            sprintf(env_array[i], "%s=%s", current->key, current->value);
        else
            sprintf(env_array[i], "%s=", current->key);
        
        current = current->next;
        i++;
    }
    env_array[i] = NULL;
    
    return env_array;
}

// Function to get environment variable value
char *get_env_value(t_env *env, const char *key)
{
    t_envlist *var = find_env_var(env, key);
    return var ? var->value : NULL;
}

// Function to clean up environment list
void cleanup_env(t_env *env)
{
    t_envlist *current = env->head;
    t_envlist *next;
    
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    
    env->head = NULL;
    env->tail = NULL;
    env->size = 0;
}

