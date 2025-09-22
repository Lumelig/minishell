/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:43:50 by jpflegha          #+#    #+#             */
/*   Updated: 2025/06/19 21:27:05 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	cc;

	cc = 0;
	while (s1[cc] != '\0' && s2[cc] != '\0')
	{
		if (s1[cc] != s2[cc])
			return (s1[cc] - s2[cc]);
		cc++;
	}
	return (s1[cc] - s2[cc]);
}
