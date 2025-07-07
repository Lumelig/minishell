#include "minishell.h"

// Function to convert redirection type to string for printing
const char *redir_type_to_string(int redir_type)
{
    switch (redir_type)
    {
        case TOKEN_REDIR_IN:
            return "<";
        case TOKEN_REDIR_OUT:
            return ">";
        case TOKEN_REDIR_APPEND:
            return ">>";
        case TOKEN_HEREDOC:
            return "<<";
        default:
            return "UNKNOWN";
    }
}

// Print a single file list
void print_file_list(t_file_list *file_list)
{
    t_file_node *current;
    int i;

    if (!file_list)
    {
        printf("  Files: NULL\n");
        return;
    }
    
    printf("  Files (size: %zd):\n", file_list->size);
    
    if (file_list->size == 0)
    {
        printf("    (no files)\n");
        return;
    }
    
    current = file_list->head;
    i = 0;
    while (current)
    {
        printf("    [%d] %s %s\n", 
               i, 
               redir_type_to_string(current->redir_type),
               current->filename);
        current = current->next;
        i++;
    }
}

// Print a single command node
void print_cmd_node(t_cmd_node *cmd_node, int cmd_index)
{
    int i;

    if (!cmd_node)
    {
        printf("Command [%d]: NULL\n", cmd_index);
        return;
    }
    
    printf("Command [%d]:\n", cmd_index);
    printf("  Type: %d\n", cmd_node->cmd_type);
    
    // Print command arguments
    if (cmd_node->cmd)
    {
        printf("  Args: ");
        i = 0;
        while (cmd_node->cmd[i])
        {
            printf("'%s'", cmd_node->cmd[i]);
            if (cmd_node->cmd[i + 1])
                printf(" ");
            i++;
        }
        printf("\n");
    }
    else
    {
        printf("  Args: NULL\n");
    }
    
    // Print file list
    print_file_list(cmd_node->files);
    printf("\n");
}

// Print the entire command list
void print_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_node *current;
    int i;

    if (!cmd_list)
    {
        printf("Command List: NULL\n");
        return;
    }
    
    printf("=== COMMAND LIST ===\n");
    printf("Total commands: %zd\n\n", cmd_list->size);
    
    if (cmd_list->size == 0)
    {
        printf("(empty command list)\n");
        return;
    }
    
    current = cmd_list->head;
    i = 0;
    while (current)
    {
        print_cmd_node(current, i);
        current = current->next;
        i++;
    }
    printf("=== END COMMAND LIST ===\n");
}

// Bonus: Print just the file list (standalone)
void print_standalone_file_list(t_file_list *file_list)
{
    if (!file_list)
    {
        printf("File List: NULL\n");
        return;
    }
    
    printf("=== FILE LIST ===\n");
    print_file_list(file_list);
    printf("=== END FILE LIST ===\n");
}