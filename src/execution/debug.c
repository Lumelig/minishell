#include "minishell.h"

void	create_cmd(int cmd_type, char **args, t_cmd_list *cmd_list)
{
	int			argc;
	t_cmd_node	*new_node;

	argc = 0;
	while (args[argc])
		argc++;
	new_node = malloc(sizeof(t_cmd_node));
	if (!new_node)
		return ; // handle error as you want
	new_node->cmd_type = cmd_type;
	new_node->files = NULL;
	new_node->next = NULL;
	new_node->cmd = malloc(sizeof(char *) * (argc + 1));
	if (!new_node->cmd)
	{
		free(new_node);
		return ;
	}
	for (int i = 0; i < argc; i++)
	{
		new_node->cmd[i] = strdup(args[i]);
		if (!new_node->cmd[i])
		{
			while (--i >= 0)
				free(new_node->cmd[i]);
			free(new_node->cmd);
			free(new_node);
			return ;
		}
	}
	new_node->cmd[argc] = NULL;
	// Add node to the cmd_list
	if (!cmd_list->head)
		cmd_list->head = new_node;
	else
		cmd_list->tail->next = new_node;
	cmd_list->tail = new_node;
	cmd_list->size++;
}

void	debug_test(t_cmd_list *cmd_list, t_env *env)
{
	char *echo1[] = {"echo", "hello", "world", NULL};
	char *echo2[] = {"echo", "-n", "no", "newline", NULL};
	char *echo3[] = {"echo", NULL};
	char *cd1[] = {"cd", "/", NULL};
	char *cd2[] = {"cd", "..", NULL};
	char *cd3[] = {"cd", "doesnotexist", NULL};
	char *pwd1[] = {"pwd", NULL};
	char *export1[] = {"export", "FOO=bar", NULL};
	char *export2[] = {"export", "A=1", "B=2", NULL};
	char *export3[] = {"export", "INVALID-NAME=value", NULL};
	char *unset1[] = {"unset", "FOO", NULL};
	char *unset2[] = {"unset", "A", "B", NULL};
	char *unset3[] = {"unset", "INVALID-NAME", NULL};
	char *env1[] = {"env", NULL};
	char *exit1[] = {"exit", NULL};
	char *exit2[] = {"exit", "42", NULL};
	char *exit3[] = {"exit", "invalid", NULL};

	(void)env;
	create_cmd(1, pwd1, cmd_list);
	create_cmd(2, env1, cmd_list);
	create_cmd(3, echo1, cmd_list);
	create_cmd(3, echo2, cmd_list);
	create_cmd(3, echo3, cmd_list);
	create_cmd(4, exit1, cmd_list);
	create_cmd(4, exit2, cmd_list);
	create_cmd(4, exit3, cmd_list);
	create_cmd(5, export1, cmd_list);
	create_cmd(5, export2, cmd_list);
	create_cmd(5, export3, cmd_list);
	create_cmd(6, cd1, cmd_list);
	create_cmd(6, cd2, cmd_list);
	create_cmd(6, cd3, cmd_list);
	create_cmd(7, unset1, cmd_list);
	create_cmd(7, unset2, cmd_list);
	create_cmd(7, unset3, cmd_list);
}