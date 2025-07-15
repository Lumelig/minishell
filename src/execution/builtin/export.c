#include "minishell.h"

/*
need to parse the export variable variable_name=variable

In Bash,
	variable names must follow specific rules regarding their syntax. Here are the key points:

Allowed Variable Names
1. **Start with a Letter or Underscore**: Variable names must begin with a letter (a-z,
	A-Z) or an underscore (_).
2. **Followed by Letters, Digits, or Underscores**: After the first character,
	variable names can include letters, digits (0-9), and underscores.

Forbidden Variable Names
1. **Cannot Start with a Digit**: Names like `1variable` are invalid.
2. **No Special Characters**: Characters such as `-`, `@`, `#`, `$`, `%`, `&`,
	etc., are not allowed. For example,
	`invalid-name` is invalid because it contains a hyphen.
3. **No Spaces**: Variable names cannot contain spaces. For example,
	`my variable` is invalid.
4. **Reserved Keywords**: Avoid using reserved keywords in Bash (like `if`,
	`then`, `else`, `for`, `while`, etc.) as variable names,
	as they may lead to unexpected behavior.

Examples
- **Valid Names**: `myVar`, `_myVar`, `var123`, `my_variable`
- **Invalid Names**: `1var`, `my-var`, `my variable`, `@var`, `if`
*/

void	export_builtin(t_cmd_node *curr, t_env *ms_env)
{
	int	i;

	/*
	export test=lala lala lala -> test=lala
	export test=lala lala=lala -> test=lala ; lala=lala
	*/
	/*
	env is initialized by taking a single line and allocating it
	then it gets put into the s_env struct and s_envlist
	s_env has head and tail to s_envlist, s_envlist is has the variables
	each variable is malloced
	so we allocate for export new env elements and put them on the end.
	(bash sorts them with a hash table but i don't need to implement that)
	so i will do:
	take the variable i need to export (from the cmd node)
	*/
	/*
	need to write all exports:
	env list + all self exported variables (how?)
	*/
	i = 0;
	while (curr->cmd[i])
	{
		printf("env: %c\n", ms_env->head[i].delimiter);
		printf("cmd: %s\n", curr->cmd[i++]);
	}
	(void)curr;
	(void)ms_env;
	printf("TODO: export\n");
}
