/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:58:30 by jpflegha          #+#    #+#             */
/*   Updated: 2024/10/12 15:38:40 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	char		*dest_p;
	const char	*src_p;
	size_t		i;

	if (!dest && !src)
		return (NULL);
	dest_p = (char *)dest;
	src_p = (char *)src;
	i = 0;
	if (dest_p > src_p)
	{
		while (len-- > 0)
		{
			dest_p[len] = src_p[len];
		}
	}
	else
	{
		while (i < len)
		{
			dest_p[i] = src_p[i];
			i++;
		}
	}
	return (dest);
}
