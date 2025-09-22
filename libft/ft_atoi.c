/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:28:08 by jpflegha          #+#    #+#             */
/*   Updated: 2025/09/20 16:38:51 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	max_min(int sign)
{
	if (sign == 1)
		return (INT_MAX);
	else
		return (INT_MIN);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r'
		|| *str == '\f' || *str == '\v')
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (result > (INT_MAX - (*str - '0')) / 10)
			max_min(sign);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
