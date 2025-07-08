#include "minishell.h"

// standalone easy to implement:
// pwd
// env
// echo
// exit
// export needed for cd
// cd relies on pwd and export
// unset   standalone but needed complementary to export

int	builtin_check(t_cmd_node *cmd)
{
	printf("debug02: builtin_check %s\n", cmd->cmd[0]);
	if (!cmd || !cmd->cmd[0] || !cmd->cmd)
		return (0);
	if (strcmp(cmd->cmd[0], "pwd") == 0)
		return (1);
	else if (strcmp(cmd->cmd[0], "env") == 0)
		return (2);
	else if (strcmp(cmd->cmd[0], "echo") == 0) // echo with -n
		return (3);
	else if (strcmp(cmd->cmd[0], "exit") == 0) // rest without options
		return (4);
	else if (strcmp(cmd->cmd[0], "export") == 0)
		return (5);
	else if (strcmp(cmd->cmd[0], "cd") == 0) // only relative or absolute path
		return (6);
	else if (strcmp(cmd->cmd[0], "unset") == 0)
		return (7);
	else
		return (0);
}

void	run_builtin(int id, t_cmd_node *curr, t_env *env)
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
	if (id == 1)
		pwd_builtin(); // done
	else if (id == 2)
		env_builtin(env); // done
	else if (id == 3)
		echo_builtin(); //
	else if (id == 4)
		exit_builtin();
	else if (id == 5)
		export_builtin();
	else if (id == 6)
		cd_builtin();
	else if (id == 7)
		unset_builtin();
}
