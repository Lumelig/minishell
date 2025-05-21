#include "minishell.h"
bool	empty_input(char *input)
{
	int	i;

	i = 0;
	while (input[i] && ft_is_space(input[i]))
		i++;
	if (i == (int)ft_strlen(input))
	{
		free(input);
		return (true);
	}
	return (false);
}

void print_history(void)
{
    HIST_ENTRY **the_list;
    int i = 0;

    the_list = history_list();
    if (the_list)
    {
        while (the_list[i])
        {
            printf("%d: %s\n", i + history_base, the_list[i]->line);
            i++;
        }
    }
}

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
		if (empty_input(input))
			continue ;

        // Exit on EOF (Ctrl+D) or NULL input
        if (!input)
        {
            printf("exit\n");
            break;
        }

        // If user typed something, add to history
        if (*input)
            add_history(input);
		print_history();
        // Simple echo of the command
        printf("You typed: %s\n", input);
		user = ft_atoi(input);
        // Free the input buffer
        free(input);
    }

    return 0;
}