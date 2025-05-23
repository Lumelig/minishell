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
	t_token *token;
    char *input;
	char *buf = NULL;
	char *cwd;
	(void)argc;
    (void)argv;
	(void)env;

    while (1)
    {
		cwd = getcwd(buf, BUFSIZ);
        printf("%s$", cwd);
		
        input = readline("\033[1;32 \033[0m");
		if (empty_input(input))
			continue ;

        if (!input)
        {
            printf("exit\n");
            break;
        }

        if (*input)
            add_history(input);
		print_history();
       
		token = tokenize(input);
		while (token)
		{
			printf("%s \n", token->value);
			token = token->next;
		}

		if(!ft_strncmp("exit", input, 4))
		{
			free(input);
			return (0);
		}
        free(input);
    }

    return 0;
}