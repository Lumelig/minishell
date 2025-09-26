/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 01:23:20 by jenne             #+#    #+#             */
/*   Updated: 2025/09/26 14:35:44 by jpflegha         ###   ########.fr       */
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

static void	dquote_handler(char *original, char **result, int *i,
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

static void	squote_handler(char *original, char **result, int *i,
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

static void	expander_loop(char *original, char **result, t_env *my_env)
{
	t_quote	quote_state;
	int		i[2];

	i[0] = 0;
	i[1] = 0;
	quote_state = QUOTE_NONE;
	while (original[i[0]])
	{
		if (original[i[0]] == '"')
			dquote_handler(original, result, i, &quote_state);
		else if (original[i[0]] == '\'')
			squote_handler(original, result, i, &quote_state);
		if ((quote_state == QUOTE_NONE || quote_state == QUOTE_DOUBLE)
			&& original[i[0]] == '$')
		{
			handle_dollar_copy(original, result, i, &quote_state);
			if (handle_dollar_expand(original, result, i, my_env))
				continue ;
		}
		else if (quote_state == QUOTE_NONE && original[i[0] + 1] == '\0')
			if (original[i[0]] != '\'' && original[i[0]] != '"')
				*result = cpy_str(original, *result, i[1] + 1, i[0]);
		i[0]++;
	}
	return ;
}

char	*expand_string(char *original, t_env *my_env)
{
	char	*result;

	result = NULL;
	if (!original || !ft_strchr(original, '$'))
		return (original);
	expander_loop(original, &result, my_env);
	if (!result)
		return (original);
	free(original);
	return (result);
}
