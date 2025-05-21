#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
    char *input;
	(void)argc;
    (void)argv;
	int user;

	user = 0;
    while (1)
    {
        printf("%s\n", env[user]);
        // Display prompt and read user input
        input = readline("\033[1;32mminihell> \033[0m");


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
		user = ft_atoi(input);
        // Free the input buffer
        free(input);
    }

    return 0;
}