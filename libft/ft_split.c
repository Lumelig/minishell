/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:36:49 by jpflegha          #+#    #+#             */
/*   Updated: 2025/09/20 16:27:33 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// for counting how many index is needed(using in_words as bool)
static int	count_words(const char *str, char delim)
{
	int	count;
	int	in_words;

	count = 0;
	in_words = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == delim)
		{
			in_words = 0;
		}
		else if (!in_words)
		{
			in_words = 1;
			count++;
		}
		str++;
	}
	return (count);
}
// to allcate for each word enough momory and copy the word

static char	*allocate_word(const char *start, const char *end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(end - start + 1);
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[i] = *start;
		start++;
		i++;
	}
	word[i] = '\0';
	return (word);
}
// free every index induvidual by index

static void	free_split(char **split, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free(split[i]);
		i++;
	}
	free(split);
}
// 2. Loop while it finds our words
// 1. count_words() for the **result (first index)

char	**ft_split(const char *s, char split)
{
	int			words;
	char		**result;
	int			i;
	const char	*start;

	i = 0;
	words = count_words(s, split);
	result = malloc((words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (*s)
	{
		if (*s != split)
		{
			start = s;
			while (*s && *s != split)
				s++;
			result[i] = allocate_word(start, s);
			if (!result[i++])
				return (free_split(result, i - 1), NULL);
		}
		if (*s != '\0')
			s++;
	}
	return (result[i] = NULL, result);
}

// int main()
// {
// 	char c = ' ';
//     char *s = "olol                     ";
// 	char *error = "Wenn du das siehst ists falsch";
// 	char **test = ft_split(s, c);
// 	while(*test)
// 	{
// 		printf("%s\n", *test);
// 		test++;
// 	}
// }
