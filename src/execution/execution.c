#include "minishell.h"

// static void	execution(t_token *token)
// {
// int for sdtin and stdout
// execution loop
// std function
// reset redir
// close files
// }

static void	create_cmd(int cmd_type, char **args, t_cmd_list *cmd_list)
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

static void	debug_test(t_cmd_list *cmd_list, t_env *env)
{
	char	*echo1[] = {"echo", "hello", "world", NULL};
	char	*echo2[] = {"echo", "-n", "no", "newline", NULL};
	char	*echo3[] = {"echo", NULL};
	char	*cd1[] = {"cd", "/", NULL};
	char	*cd2[] = {"cd", "..", NULL};
	char	*cd3[] = {"cd", "doesnotexist", NULL};
	char	*pwd1[] = {"pwd", NULL};
	char	*pwd2[] = {"pwd", NULL};
	char	*pwd3[] = {"pwd", NULL};
	char	*export1[] = {"export", "FOO=bar", NULL};
	char	*export2[] = {"export", "A=1", "B=2", NULL};
	char	*export3[] = {"export", "INVALID-NAME=value", NULL};
	char	*unset1[] = {"unset", "FOO", NULL};
	char	*unset2[] = {"unset", "A", "B", NULL};
	char	*unset3[] = {"unset", "INVALID-NAME", NULL};
	char	*env1[] = {"env", NULL};
	char	*env2[] = {"env", NULL};
	char	*env3[] = {"env", NULL};
	char	*exit1[] = {"exit", NULL};
	char	*exit2[] = {"exit", "42", NULL};
	char	*exit3[] = {"exit", "invalid", NULL};

	(void)env;
	// Echo
	// Cd
	// Pwd
	// Export
	// Unset
	// Env
	// Exit
	// Fill cmd_list via create_cmd
	create_cmd(2, echo1, cmd_list);
	create_cmd(2, echo2, cmd_list);
	create_cmd(2, echo3, cmd_list);
	create_cmd(1, cd1, cmd_list);
	create_cmd(1, cd2, cmd_list);
	create_cmd(1, cd3, cmd_list);
	create_cmd(7, pwd1, cmd_list);
	create_cmd(7, pwd2, cmd_list);
	create_cmd(7, pwd3, cmd_list);
	create_cmd(5, export1, cmd_list);
	create_cmd(5, export2, cmd_list);
	create_cmd(5, export3, cmd_list);
	create_cmd(6, unset1, cmd_list);
	create_cmd(6, unset2, cmd_list);
	create_cmd(6, unset3, cmd_list);
	create_cmd(4, env1, cmd_list);
	create_cmd(4, env2, cmd_list);
	create_cmd(4, env3, cmd_list);
	create_cmd(3, exit1, cmd_list);
	create_cmd(3, exit2, cmd_list);
	create_cmd(3, exit3, cmd_list);
}

static int	builtin_check(t_cmd_node *cmd)
{
	printf("debug02: builtin_check %s\n", cmd->cmd[0]);
	if (!cmd || !cmd->cmd[0] || !cmd->cmd)
		return (0);
	if (strcmp(cmd->cmd[0], "cd") == 0) // only relative or absolute path
		return (1);
	else if (strcmp(cmd->cmd[0], "echo") == 0) // echo with -n
		return (2);
	else if (strcmp(cmd->cmd[0], "exit") == 0) // rest without options
		return (3);
	else if (strcmp(cmd->cmd[0], "env") == 0)
		return (4);
	else if (strcmp(cmd->cmd[0], "export") == 0)
		return (5);
	else if (strcmp(cmd->cmd[0], "unset") == 0)
		return (6);
	else if (strcmp(cmd->cmd[0], "pwd") == 0)
		return (7);
	else
		return (0);
}

static void	run_builtin(int id, t_cmd_node *curr, t_env *env)
{
	// standalone easy to implement:
	// pwd
	// env
	// echo
	// exit
	// export needed for cd
	// cd relies on pwd and export
	// unset   standalone but needed complementary to export
	(void)curr;
	(void)env;
	printf("debug03: run builtin\n");
	if (id == 1) // cd
		printf("cd\n");
	else if (id == 2) // echo
		printf("echo\n");
	else if (id == 3) // exit
		printf("exit\n");
	else if (id == 4) // env
		printf("env\n");
	else if (id == 5) // export
		printf("export\n");
	else if (id == 6) // unset
		printf("unset\n");
	else if (id == 7) // pwd
		printf("pwd\n");
}

int	executor(t_cmd_list *cmd_list, t_env *env)
// start of the entire execution chain
{
	pid_t pid;
	int status;

	debug_test(cmd_list, env);
	if (!cmd_list || !cmd_list->head)
		return (printf("bad cmd_list or head\n"), 1);
	t_cmd_node *curr = cmd_list->head;

	printf("debug 00: entry\n");
	printf("debug 01: %s\n", curr->cmd[0]);
	if (!curr->cmd[0])
		return (printf("debug 88: empty curr\n"), free(curr->cmd), 1);
	while (curr)
	{
		int id = builtin_check(curr);
		if (id <= 7 && id >= 1)
			run_builtin(id, curr, env);
		else // execve // TODO: put it into a function
		{
			// TODO: function to get path
			printf("debug 99\n");
			continue ;
			pid = fork();
			if (pid == 0)
			{
				// execve(path, curr, env);
				perror("execve failed");
				exit(1);
			}
			else if (pid < 0)
				perror("fork failed");
			else
				waitpid(pid, &status, 0);
		}
		curr = curr->next;
	}
	return (0);
	/*
Decisions executor makes:
	Nothing to execute? Return cleanly.
	One command only, and it's a builtin that must affect the parent?
		No fork() → Call builtin directly
	Anything else (multiple commands or external command)?
		Pass to the main execution loop:
			Set up pipes
			Fork each command
			Setup redirections
			Wait for all children
			Collect status
	*/
}

/*
Execution	logic = loop over command nodes, and for each:

	Check if it's a builtin (in parent or child?) in parent duh. we didn't fork yet
	Set up redirections (<, >, >>, <<)
	Set up pipes if needed
	Fork (unless parent-only builtin)
	In child: handle redirs + execve or builtin
	In parent: track child PID, wait later
	After loop: wait for all child processes
*/

/*
get_env() function? don't need to pass env around
*/

/*
Warning for Signals:
Be careful. This global variable cannot provide any other
information or data access than the number of a received signal.
Therefore, using "norm" type structures in the global scope is
forbidden.
*/