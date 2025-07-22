#include "minishell.h"

static bool	add_node(char *cmd, t_env *ms_env);
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
not for export though, but in special syntax cases:

export if=foo        # allowed
export else=bar      # allowed
export do=stuff      # allowed

if true; then echo yes; fi  # "if" acts as a keyword here, special syntax


Examples
- **Valid Names**: `myVar`, `_myVar`, `var123`, `my_variable`
- **Invalid Names**: `1var`, `my-var`, `my variable`, `@var`, `if`
*/

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

void	export_builtin(t_cmd_node *curr, t_env *ms_env)
{
	int	i;

	i = 1;
	if (!curr->cmd[1])
	{
		printf("export only\n");
		return ;
	}
	printf("tail: %s=%s\n", ms_env->tail->key, ms_env->tail->value);
	printf("curr: %s\n", curr->cmd[1]);
	while (curr->cmd[i])
	{
		if (add_node(curr->cmd[i], ms_env) == 0)
		{
			printf("error adding node for export");
			return ;
		}
		i++;
	}
	printf("tail: %s=%s\n", ms_env->tail->key, ms_env->tail->value);
	printf("TODO: export parsing, overwriting\n");
}

// static void	ex_update_env_var(t_env *, char *key, char *value)
// {
// 	t_envlist	*existing;

// 	existing = find_env_var(env, key);
// 	if (existing)
// 	{
// 		free(existing->value);
// 		existing->value = value;
// 		free(key);
// 		return (true);
// 	}
// 	else
// 		return (add_env_var(env, key, value));
// }

// static t_envlist	*ex_find_env_var(t_env *env, const char *key)
// {
// 	t_envlist	*current;

// 	current = env->head;
// 	while (current)
// 	{
// 		if (!ft_strcmp(current->key, key))
// 			return (current);
// 		current = current->next;
// 	}
// 	return (NULL);
// }

// static char	*ex_get_env_value(t_env *env, const char *key)
// {
// 	t_envlist	*var;

// 	var = find_env_var(env, key);
// 	if (var)
// 		return (var->value);
// 	else
// 		return (NULL);
// }

static bool	add_node(char *cmd, t_env *ms_env)
{
	t_envlist	*new_node;
	char		*sep;

	// parsing
	// if not ok i++ and continue
	new_node = (t_envlist *)malloc(sizeof(t_envlist));
	if (!new_node)
		return (false);
	sep = ft_strchr(cmd, '=');
	new_node->key = ft_substr(cmd, 0, sep - cmd);
	new_node->value = ft_strdup(sep + 1);
	if (!ms_env->head)
	{
		ms_env->head = new_node;
		ms_env->tail = new_node;
	}
	else
	{
		ms_env->tail->next = new_node;
		ms_env->tail = new_node;
	}
	ms_env->size++;
	return (true);
}
