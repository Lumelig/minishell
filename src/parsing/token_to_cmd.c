#include "minishell.h"

// Command type definitions (add these to your header)
#define CMD_SIMPLE    0  // Simple command
#define CMD_PIPE      1  // Command in a pipeline

// Redirection type definitions (add these to your header)
#define REDIR_IN      0  // <
#define REDIR_OUT     1  // >
#define REDIR_APPEND  2  // >>
#define REDIR_HEREDOC 3  // <<

void cleanup_cmd_node(t_cmd_node *node);
void cleanup_cmd_list(t_cmd_list *list);
void print_cmd_list(t_cmd_list *list);

// Helper function to create a new file node
t_file_node *create_file_node(char *filename, int redir_type)
{
    t_file_node *node = malloc(sizeof(t_file_node));
    if (!node)
        return NULL;
    
    node->filename = ft_strdup(filename);
    if (!node->filename)
    {
        free(node);
        return NULL;
    }
    
    node->redir_type = redir_type;
    node->next = NULL;
    return node;
}

// Helper function to add file node to file list
bool add_file_to_list(t_file_list *list, char *filename, int redir_type)
{
    t_file_node *new_node = create_file_node(filename, redir_type);
    if (!new_node)
        return false;
    
    if (!list->head)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return true;
}

// Helper function to create a new file list
t_file_list *create_file_list(void)
{
    t_file_list *list = malloc(sizeof(t_file_list));
    if (!list)
        return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// Helper function to count words before next pipe or end
int count_words_until_pipe(t_token *start)
{
    int count = 0;
    t_token *current = start;
    
    while (current && current->type != TOKEN_PIPE)
    {
        if (current->type == TOKEN_WORD)
            count++;
        current = current->next;
    }
    return count;
}

// Helper function to create command array from tokens
char **create_cmd_array(t_token **current_token, int word_count)
{
    char **cmd_array = malloc(sizeof(char *) * (word_count + 1));
    if (!cmd_array)
        return NULL;
    
    int i = 0;
    t_token *token = *current_token;
    
    while (token && token->type != TOKEN_PIPE && i < word_count)
    {
        if (token->type == TOKEN_WORD)
        {
            cmd_array[i] = ft_strdup(token->value);
            if (!cmd_array[i])
            {
                // Cleanup on failure
                while (--i >= 0)
                    free(cmd_array[i]);
                free(cmd_array);
                return NULL;
            }
            i++;
        }
        token = token->next;
    }
    cmd_array[i] = NULL;
    *current_token = token;
    return cmd_array;
}

// Convert redirection token type to redir_type int
int token_type_to_redir_type(t_token_type type)
{
    switch (type)
    {
        case TOKEN_REDIR_IN:
            return REDIR_IN;
        case TOKEN_REDIR_OUT:
            return REDIR_OUT;
        case TOKEN_REDIR_APPEND:
            return REDIR_APPEND;
        case TOKEN_HEREDOC:
            return REDIR_HEREDOC;
        default:
            return -1; // Error
    }
}

// Parse a single command (until pipe or end)
t_cmd_node *parse_single_command(t_token **current_token, bool is_pipe)
{
    t_cmd_node *cmd_node = malloc(sizeof(t_cmd_node));
    if (!cmd_node)
        return NULL;
    
    // Initialize command node
    cmd_node->cmd_type = is_pipe ? CMD_PIPE : CMD_SIMPLE;
    cmd_node->cmd = NULL;
    cmd_node->files = create_file_list();
    cmd_node->next = NULL;
    
    if (!cmd_node->files)
    {
        free(cmd_node);
        return NULL;
    }
    
    t_token *token = *current_token;
    int word_count = 0;
    
    // First pass: count words and handle redirections
    t_token *temp_token = token;
    while (temp_token && temp_token->type != TOKEN_PIPE)
    {
        if (temp_token->type == TOKEN_WORD)
        {
            word_count++;
        }
        else if (temp_token->type >= TOKEN_REDIR_IN && temp_token->type <= TOKEN_HEREDOC)
        {
            // Skip redirection operator and its target
            temp_token = temp_token->next;
            if (temp_token && temp_token->type == TOKEN_WORD)
            {
                // Don't count redirection target as command word
                temp_token = temp_token->next;
                continue;
            }
        }
        temp_token = temp_token->next;
    }
    
    // Allocate command array
    if (word_count > 0)
    {
        cmd_node->cmd = malloc(sizeof(char *) * (word_count + 1));
        if (!cmd_node->cmd)
        {
            free(cmd_node->files);
            free(cmd_node);
            return NULL;
        }
    }
    
    // Second pass: fill command array and file list
    int cmd_index = 0;
    while (token && token->type != TOKEN_PIPE)
    {
        if (token->type == TOKEN_WORD)
        {
            if (cmd_node->cmd)
            {
                cmd_node->cmd[cmd_index] = ft_strdup(token->value);
                if (!cmd_node->cmd[cmd_index])
                {
                    // Cleanup on failure
                    while (--cmd_index >= 0)
                        free(cmd_node->cmd[cmd_index]);
                    free(cmd_node->cmd);
                    free(cmd_node->files);
                    free(cmd_node);
                    return NULL;
                }
                cmd_index++;
            }
            token = token->next;
        }
        else if (token->type >= TOKEN_REDIR_IN && token->type <= TOKEN_HEREDOC)
        {
            // Handle redirection
            int redir_type = token_type_to_redir_type(token->type);
            token = token->next; // Move to filename
            
            if (!token || token->type != TOKEN_WORD)
            {
                // Syntax error: redirection without filename
                if (cmd_node->cmd)
                {
                    while (--cmd_index >= 0)
                        free(cmd_node->cmd[cmd_index]);
                    free(cmd_node->cmd);
                }
                free(cmd_node->files);
                free(cmd_node);
                return NULL;
            }
            
            if (!add_file_to_list(cmd_node->files, token->value, redir_type))
            {
                // Memory allocation failed
                if (cmd_node->cmd)
                {
                    while (--cmd_index >= 0)
                        free(cmd_node->cmd[cmd_index]);
                    free(cmd_node->cmd);
                }
                free(cmd_node->files);
                free(cmd_node);
                return NULL;
            }
            token = token->next;
        }
        else
        {
            token = token->next;
        }
    }
    
    // Null-terminate command array
    if (cmd_node->cmd)
        cmd_node->cmd[cmd_index] = NULL;
    
    *current_token = token;
    return cmd_node;
}

// Add command node to command list
bool add_cmd_to_list(t_cmd_list *list, t_cmd_node *cmd_node)
{
    if (!list || !cmd_node)
        return false;
    
    if (!list->head)
    {
        list->head = cmd_node;
        list->tail = cmd_node;
    }
    else
    {
        list->tail->next = cmd_node;
        list->tail = cmd_node;
    }
    list->size++;
    return true;
}

// Create a new command list
t_cmd_list *create_cmd_list(void)
{
    t_cmd_list *list = malloc(sizeof(t_cmd_list));
    if (!list)
        return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// Main parser function: convert tokens to command structure
t_cmd_list *parse_tokens(t_token *tokens)
{
    if (!tokens)
        return NULL;
    
    t_cmd_list *cmd_list = create_cmd_list();
    if (!cmd_list)
        return NULL;
    
    t_token *current_token = tokens;
    bool is_pipe = false;
    
    while (current_token)
    {
        t_cmd_node *cmd_node = parse_single_command(&current_token, is_pipe);
        if (!cmd_node)
        {
            // Cleanup and return NULL on error
            cleanup_cmd_list(cmd_list);
            return NULL;
        }
        
        if (!add_cmd_to_list(cmd_list, cmd_node))
        {
            // Cleanup on failure
            cleanup_cmd_node(cmd_node);
            cleanup_cmd_list(cmd_list);
            return NULL;
        }
        
        // Check if we hit a pipe
        if (current_token && current_token->type == TOKEN_PIPE)
        {
            current_token = current_token->next; // Skip pipe token
            is_pipe = true;
        }
        else
        {
            is_pipe = false;
        }
    }
	//print_cmd_list(cmd_list);
    return cmd_list;
}

// Cleanup functions
void cleanup_file_node(t_file_node *node)
{
    if (!node)
        return;
    free(node->filename);
    free(node);
}

void cleanup_file_list(t_file_list *list)
{
    if (!list)
        return;
    
    t_file_node *current = list->head;
    while (current)
    {
        t_file_node *next = current->next;
        cleanup_file_node(current);
        current = next;
    }
    free(list);
}

void cleanup_cmd_node(t_cmd_node *node)
{
    if (!node)
        return;
    
    if (node->cmd)
    {
        for (int i = 0; node->cmd[i]; i++)
            free(node->cmd[i]);
        free(node->cmd);
    }
    
    cleanup_file_list(node->files);
    free(node);
}

void cleanup_cmd_list(t_cmd_list *list)
{
    if (!list)
        return;
    
    t_cmd_node *current = list->head;
    while (current)
    {
        t_cmd_node *next = current->next;
        cleanup_cmd_node(current);
        current = next;
    }
    free(list);

}

// Debug function to print command structure
void print_cmd_list(t_cmd_list *list)
{
    if (!list)
    {
        printf("Command list is NULL\n");
        return;
    }
    
    printf("Command list (%zd commands):\n", list->size);
    
    t_cmd_node *cmd = list->head;
    int cmd_num = 0;
    
    while (cmd)
    {
        printf("Command %d (type: %s):\n", cmd_num++, 
               cmd->cmd_type == CMD_SIMPLE ? "SIMPLE" : "PIPE");
        
        // Print command
        if (cmd->cmd)
        {
            printf("  Command: ");
            for (int i = 0; cmd->cmd[i]; i++)
                printf("'%s' ", cmd->cmd[i]);
            printf("\n");
        }
        else
        {
            printf("  Command: (none)\n");
        }
        
        // Print files/redirections
        if (cmd->files && cmd->files->size > 0)
        {
            printf("  Redirections (%zd):\n", cmd->files->size);
            t_file_node *file = cmd->files->head;
            while (file)
            {
                char *type_str;
                switch (file->redir_type)
                {
                    case REDIR_IN: type_str = "<"; break;
                    case REDIR_OUT: type_str = ">"; break;
                    case REDIR_APPEND: type_str = ">>"; break;
                    case REDIR_HEREDOC: type_str = "<<"; break;
                    default: type_str = "?"; break;
                }
                printf("    %s %s\n", type_str, file->filename);
                file = file->next;
            }
        }
        else
        {
            printf("  Redirections: (none)\n");
        }
        
        cmd = cmd->next;
        if (cmd)
            printf("  | (pipe to next command)\n");
        printf("\n");
    }
}