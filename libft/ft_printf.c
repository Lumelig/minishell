/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:29:52 by jpflegha          #+#    #+#             */
/*   Updated: 2025/06/21 14:31:41 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_digit_upper(unsigned long n, int base)
{
	int		count;
	char	*symbols;

	symbols = "0123456789ABCDEF";
	count = 0;
	if (n < (unsigned long)base)
		return (print_char(symbols[n]));
	count += print_digit_upper(n / base, base);
	return (count + print_char(symbols[n % base]));
}

int	print_pointer(unsigned long ptr)
{
	int	count;

	count = 0;
	if (!ptr)
		return (write(1, "(nil)", 5));
	count += write(1, "0x", 2);
	count += print_digit(ptr, 16);
	return (count);
}

int	print_unsigned(unsigned int n)
{
	int		count;
	char	*symbols;

	symbols = "0123456789";
	count = 0;
	if (n < 10)
		return (print_char(symbols[n]));
	count += print_unsigned(n / 10);
	return (count + print_char(symbols[n % 10]));
}

int	print_format(char c, va_list ap)
{
	int	count;

	count = 0;
	if (c == 'c')
		count = print_char(va_arg(ap, int));
	else if (c == 's')
		count = print_str(va_arg(ap, char *));
	else if (c == 'd' || c == 'i')
		count = print_digit((long)(va_arg(ap, int)), 10);
	else if (c == 'u')
		count = print_unsigned(va_arg(ap, unsigned int));
	else if (c == 'p')
		count = print_pointer((unsigned long)va_arg(ap, void *));
	else if (c == 'x')
		count = print_digit((long)(va_arg(ap, unsigned int)), 16);
	else if (c == 'X')
		count = print_digit_upper(va_arg(ap, unsigned int), 16);
	else if (c == '%')
		count = print_char('%');
	else
		count = print_char(c);
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	va_start(ap, str);
	while (str[i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			ret += print_format(str[++i], ap);
			if (ret == -1)
				return (va_end(ap), -1);
		}
		else
		{
			ret += write(1, &str[i], 1);
			if (ret == -1)
				return (va_end(ap), -1);
		}
		i++;
	}
	return (va_end(ap), ret);
}
