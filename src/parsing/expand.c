/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:23:20 by jenne             #+#    #+#             */
/*   Updated: 2025/09/25 17:18:15 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
"" rules:
world splitting (IFS)
pathname expansion (globbing) (*, ?, ...)
\ only escapes ", $, `, \, \n
~ does not expand
"" is an empty word

no quotes:
\ escapes everything

minishell scope:
pathname expansions are not necessary to be handled
escapes technically not either but we will manage that
we don't handle tilde yet but will probably add it
need to add handling to turn '' and "" into empty words instead of only removing them
*/
// echo "\\\$USER"Test$USER
// echo j\\\iUSERiTestiUSER
// echo "\\\$USER"Test$EXPANDED
// echo "\\\\$EXPANDED"Test$EXPANDED

/*
echo '"'$USER'"'
"realm"
/bin/echo $"HOM"E$USER
HOMErealm

/bin/echo "'$USER'"
'realm'
/bin/echo "''$USER''"
''realm''
/bin/echo '"'"$USER"'"'
"realm"
/bin/echo "exit_code ->$? user ->$USER home -> $HOME"
exit_code ->0 user ->realm home -> /home/realm



/bin/echo $"42$"
4242$ | 42$
/bin/echo "$ "
<$> | <$ >
/bin/echo \$USER

/bin/echo \\\$USER

/bin/echo \\\\\\\\\$USER

/bin/echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\$USER \$PATH \\$PWD

/bin/echo '$USER' "$USER" "text  ' text"

/bin/echo text"'$USER'" ' $USER '

/bin/echo "text"   "$USER"    "$USER"

/bin/echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''

/bin/echo "text" "text$USER" ... "$USER"

/bin/echo '' ""

/bin/echo $USER$TESTNOTFOUND$HOME$

/bin/echo $USER$TESTNOTFOUND$HOME$WTF$PWD

echo '' -n

echo "" "" "" -n -n -n -n

cat << $USER
why
not
$USER

cat << "$USER"
why
not
$USER

cat << "$US"E"R"
because
we
love
bash
$USER

export T=n
echo "-"$T$T

export T=nnnnnnnn
echo "-""$T"nnnnnnnnnnnnn -nnnnnnnn"$T" '-'"$T"

export T=nnnnnnnn
echo "-""$T"nnnnnnnnnnnnn -nnnnnnnn"$T" '-''$T'

export T=nnnnnnnn
echo "-"'$T'nnnnnnnnnnnnn -nnnnnnnn'$T' '-'"$T "

export T=nnnnnnnn
echo "-"'$T'nnnnnnnnnnnnn -nnnnnnnn$T '-''$T '

export T=e E=c S=h L=o
$T$E$S$L

export T="|"
echo segfault $T grep segfault

export T='|'
echo segfault $T grep segfault

export T=">"
echo segfault $T grep segfault

export T='<'
echo segfault $T grep segfault

export T="<<"
echo segfault $T grep segfault

export T='<<'
echo segfault $T grep segfault

export T="echo segfault | grep segfault"
$T

export T='|'
echo $T echo lala $T echo $T echo ?

export T="<<"
$T.

'''''''''''''''' echo ok

echo "$"$'$'$"$"$"$"$'$'

echo $/ $/

echo "$/ $/"

echo $/"$/"


*/

void	dquote_handler(char *original, char **result, int *i,
		t_quote *quote_state)
{
	if (*quote_state == QUOTE_DOUBLE && (i[0] == 0 || original[i[0]
			- 1] != '\\'))
	{
		*quote_state = QUOTE_NONE;
		if (!*result)
			*result = ft_substr(original, 0, i[0]);
		else
			*result = cpy_str(original, *result, i[1], i[0]);
		i[1] = i[0];
	}
	else if (*quote_state == QUOTE_NONE)
	{
		*quote_state = QUOTE_DOUBLE;
		i[1] = i[0];
		if (!*result)
			*result = ft_substr(original, 0, i[0]);
	}
}

void	squote_handler(char *original, char **result, int *i,
		t_quote *quote_state)
{
	if (*quote_state == QUOTE_SINGLE && (i[0] == 0 || original[i[0]
			- 1] != '\\'))
	{
		*quote_state = QUOTE_NONE;
		if (!*result)
			*result = ft_substr(original, 0, i[0]);
		else
			*result = cpy_str(original, *result, i[1], i[0]);
		i[1] = i[0];
	}
	else if (*quote_state == QUOTE_NONE)
	{
		*quote_state = QUOTE_SINGLE;
		i[1] = i[0];
		if (!*result)
			*result = ft_substr(original, 0, i[0]);
	}
}

// bool is_backslash(char *original)
// {

// 	return (false);
// }

bool	handle_dollar(char *original, char **result, int *i, t_env *my_env)
{
	if (!result)
		result = ft_substr(original, 0, i[0]);
	else if (quote_state == QUOTE_DOUBLE)
		result = cpy_str(original, result, i[1], i[0] - 1);
	else
		result = cpy_str(original, result, i[1] + 1, i[0] - 1);
	if (is_special_expansion(original, i[0]))
	{
		result = copy_special_var(result, original, &i[0], my_env);
		i[1] = i[0];
		return (1);
	}
	else if ((ft_isalpha(original[i[0] + 1]) || original[i[0] + 1] == '_'
			|| original[i[0] + 1] == '{'))
	{
		result = copy_variable(result, original, &i[0], my_env);
		i[1] = i[0];
		return (1);
	}
	return (0);
}

static char	*expander_loop(char *original, t_env *my_env)
{
	t_quote	quote_state;
	char	*result;
	int		i[2];

	i[0] = 0;
	i[1] = 0;
	result = NULL;
	quote_state = QUOTE_NONE;
	while (original[i[0]])
	{
		if (original[i[0]] == '"')
			dquote_handler(original, &result, i, &quote_state);
		else if (original[i[0]] == '\'')
			squote_handler(original, &result, i, &quote_state);
		//
		if ((quote_state == QUOTE_NONE || quote_state == QUOTE_DOUBLE)
			&& original[i[0]] == '$')
		{
			if (handle_dollar(original, &result, i, my_env))
				continue ;
		}
		else if (quote_state == QUOTE_NONE && original[i[0] + 1] == '\0')
		{
			if (original[i[0]] != '\'' && original[i[0]] != '"')
				result = cpy_str(original, result, i[1], i[0]);
		}
		i[0]++;
	}
	return (result);
}

// static char	*expander_loop(char *original, t_env *my_env)
// {
// 	t_quote	quote_state;
// 	char	*result;
// 	int		i[2];

// 	i[0] = 0;
// 	i[1] = 0;
// 	result = NULL;
// 	quote_state = QUOTE_NONE;
// 	while (original[i[0]])
// 	{
// 		if (original[i[0]] == '"')
// 		{
// 			// printf("%d:entering dquote: %c\n", i, original[i[0]]);
// 			if (quote_state == QUOTE_DOUBLE && (i[0] == 0 || original[i[0]
// 					- 1] != '\\'))
// 			{
// 				// printf("%d:inside double\n", i[0]);
// 				quote_state = QUOTE_NONE;
// 				if (!result)
// 				{
// 					result = ft_substr(original, 0, i[0]);
// 					// printf("%d: !result %s - %c - i: %d j: %d\n", i, result,
// 					// original[i[0]], i, j);
// 				}
// 				else
// 				{
// 					// if the expandible doesn't exist, i need i[0] - 1
// 					result = cpy_str(original, result, i[1], i[0]);
// 					// printf("%d: cpy_str %s - %c - i: %d j: %d\n", i, result,
// 					// original[i[0]], i, j);
// 				}
// 				// printf("i[1] = i\n");
// 				i[1] = i[0];
// 			}
// 			else if (quote_state == QUOTE_NONE)
// 			{
// 				quote_state = QUOTE_DOUBLE;
// 				// printf("%d:j before: %d after: %d\n", i, i[1], i[0]);
// 				i[1] = i[0];
// 				if (!result)
// 				{
// 					result = ft_substr(original, 0, i[0]);
// 					// printf("%d:no result: substr: dquote %s\n", i, result);
// 				}
// 			}
// 			// else if (quote_state == QUOTE_SINGLE && original[i[0]
// 			// - 1] == '\'')
// 			// {
// 			// 	// printf("%d:double inside single\n", i[0]);
// 			// 	result = cpy_str(original, result, i[1], i[0]);
// 			// }
// 		}
// 		// single quote flag setting
// 		else if (original[i[0]] == '\'')
// 		{
// 			// printf("%d:entering squote: %c\n", i, original[i[0]]);
// 			if (quote_state == QUOTE_SINGLE && (i[0] == 0 || original[i[0]
// 					- 1] != '\\'))
// 			{
// 				// printf("%d:inside single\n", i[0]);
// 				quote_state = QUOTE_NONE;
// 				if (!result)
// 				{
// 					result = ft_substr(original, 0, i[0]);
// 					// printf("%d: !result %s - %c - %d - %d\n", i, result,
// 					// original[i[0]], i, j);
// 				}
// 				else
// 				{
// 					result = cpy_str(original, result, i[1], i[0]);
// 					// printf("%d: cpy_str %s - %c - %d - %d\n", i, result,
// 					// original[i[0]], i, j);
// 				}
// 				// printf("i[1] = i\n");
// 				i[1] = i[0];
// 			}
// 			else if (quote_state == QUOTE_NONE)
// 			{
// 				quote_state = QUOTE_SINGLE;
// 				// printf("%d:j before: %d after: %d\n", i, i[1], i[0]);
// 				i[1] = i[0];
// 				if (!result)
// 				{
// 					result = ft_substr(original, 0, i[0]);
// 					// printf("%d:no result: substr: squote: %s\n", i, result);
// 				}
// 			}
// 			// else if (quote_state == QUOTE_DOUBLE && original[i[0]
// 			// - 1] == '"')
// 			// {
// 			// 	// printf("%d:single inside double\n", i[0]);
// 			// 	result = cpy_str(original, result, i[1], i[0]);
// 			// }
// 		}
// 		// if (result)
// 		// printf("%d:after check: result: %s - state: %d\n", i, result,
// 		// quote_state);
// 		if ((quote_state == QUOTE_NONE || quote_state == QUOTE_DOUBLE)
// 			&& original[i[0]] == '$')
// 		{
// 			// printf("%d:$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n",
// 			// i[0]);
// 			// printf("%d: %s - %c\n", i, result, original[i[0]]);
// 			if (!result)
// 			{
// 				result = ft_substr(original, 0, i[0]);
// 				// printf("%d:no result: substr: %s\n", i, result);
// 			}
// 			else if (quote_state == QUOTE_DOUBLE)
// 				// copies $ if not i-1
// 				result = cpy_str(original, result, i[1], i[0] - 1);
// 			else
// 				result = cpy_str(original, result, i[1] + 1, i[0] - 1);
// 			// if i[0] + 1 == $, ?, 0 // " in future?
// 			if (is_special_expansion(original, i[0]))
// 			{
// 				result = copy_special_var(result, original, &i[0], my_env);
// 				// printf("%d:result after special var: %s - %c - %d\n",
// 				// i, result, original[i[0]], i[0]);
// 				i[1] = i[0];
// 				continue ;
// 			}
// 			else if ((ft_isalpha(original[i[0] + 1]) || original[i[0]
// 					+ 1] == '_' || original[i[0] + 1] == '{'))
// 			{
// 				result = copy_variable(result, original, &i[0], my_env);
// 				// printf("%d:result after var: %s - %c - %d\n", i,
// 				// result, original[i[0]], i[0]);
// 				i[1] = i[0];
// 				continue ;
// 			}
// 		}
// 		else if (quote_state == QUOTE_NONE && original[i[0] + 1] == '\0')
// 		{
// 			// i[0] = j;
// 			// printf("%d - %c\n", i, original[i[0]]);
// 			if (original[i[0]] != '\'' && original[i[0]] != '"')
// 			{
// 				result = cpy_str(original, result, i[1], i[0]);
// 				// printf("%d:result after none_quote: %s - %c - %d\n",
// 				// i, result, original[i[0]], i[0]);
// 			}
// 		}
// 		// if (result)
// 		// printf("%d:loop end result: %s - %c\n", i, result,
// 		// original[i[0]]);
// 		i[0]++;
// 	}
// 	// printf("final result: %s\n", result);
// 	return (result);
// }

// echo "\\\$USER"Test$USER
char	*expand_string(char *original, t_env *my_env)
{
	char	*result;

	if (!original || !ft_strchr(original, '$'))
		return (original);
	result = expander_loop(original, my_env);
	if (!result)
		return (original);
	free(original);
	return (result);
}
