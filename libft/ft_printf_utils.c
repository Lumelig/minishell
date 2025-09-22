/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:36:45 by jpflegha          #+#    #+#             */
/*   Updated: 2025/06/21 14:21:00 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_char(char c)
{
	return (write(1, &c, 1));
}

int	print_str(char *str)
{
	int	i;
	int	ret;

	i = 0;
	if (!str)
		return (write(1, "(null)", 6));
	while (str[i])
	{
		ret = write(1, &str[i], 1);
		if (ret == -1)
			return (-1);
		i++;
	}
	return (i);
}

int	print_digit(long n, int base)
{
	int		count;
	char	*symbols;

	symbols = "0123456789abcdef";
	count = 0;
	if (n < 0)
	{
		count += write(1, "-", 1);
		return (count + print_digit(-n, base));
	}
	if (n < base)
		return (count + print_char(symbols[n]));
	count += print_digit(n / base, base);
	return (count + print_char(symbols[n % base]));
}
