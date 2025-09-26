/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 13:49:21 by jpflegha          #+#    #+#             */
/*   Updated: 2024/10/13 20:17:01 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*tmp2;
	char	*tmp;
	size_t	i;

	i = 0;
	tmp = malloc(count * size);
	if (!tmp)
		return (NULL);
	tmp2 = tmp;
	while (i < count * size)
	{
		tmp[i] = 0;
		i++;
	}
	return (tmp2);
}

// void	*ft_calloc(size_t count, size_t size)
// {
// 	void	*tmp2;
// 	char	*tmp;
// 	size_t	i;

// 	i = 0;
// 	tmp = malloc(count * size);
// 	if (!tmp)
// 		return (NULL);
// 	tmp2 = tmp;
// 	while (i < count * size)
// 		tmp[i++] = 0;
// 	return (tmp);
// }