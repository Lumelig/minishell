/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:44:34 by jpflegha          #+#    #+#             */
/*   Updated: 2024/10/12 18:05:46 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	cc;
	char			*cs;

	cs = (char *)s;
	cc = (char)c;
	while (*cs)
	{
		if (*cs == cc)
			return (cs);
		cs++;
	}
	if (cc == '\0')
		return (cs);
	return (NULL);
}
