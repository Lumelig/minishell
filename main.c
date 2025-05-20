#include "minishell.h"

int main(void)
{
    char *input;

    while (1)
    {
        // Display prompt and read user input
        input = readline("minishell> ");

        // Exit on EOF (Ctrl+D) or NULL input
        if (!input)
        {
            printf("exit\n");
            break;
        }

        // If user typed something, add to history
        if (*input)
            add_history(input);

        // Simple echo of the command
        printf("You typed: %s\n", input);

        // Free the input buffer
        free(input);
    }

    return 0;
}