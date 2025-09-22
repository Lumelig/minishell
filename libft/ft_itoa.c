/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:46:56 by jpflegha          #+#    #+#             */
/*   Updated: 2025/09/20 16:39:05 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

short	digit(int n)
{
	short	i;

	i = 0;
	if (n < 0)
	{
		n *= -1;
		i++;
	}
	else if (n == 0)
		return (++i);
	while (n)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*itoa_write(int n, short len, char *num)
{
	long	nbr;

	num[len] = '\0';
	if (n == 0)
		return (num[0] = '0', num);
	else if (n < 0)
	{
		nbr = (long)n * -1;
		num[0] = '-';
	}
	else
		nbr = n;
	while (--len >= 0)
	{
		if (n < 0 && num[len] == '-')
			break ;
		num[len] = (nbr % 10) + '0';
		nbr = nbr / 10;
	}
	return (num);
}

char	*ft_itoa(int n)
{
	short	len;
	char	*num;

	len = digit(n);
	if (len > 10)
		NULL;
	num = malloc((len + 1) * sizeof(char));
	if (!num)
		return (NULL);
	num = itoa_write(n, len, num);
	return (num);
}
// int main()
// {
// 	int i = 42;
// 	char *out = ft_itoa(i);
// 	printf("%d\n", (int)out[0]);
// 	if (out[0] == '4')
// 		printf("WE AHVE A FOUR!\n");
// 	if (ft_strncmp("42", out, 3))
// 		printf("somethings wrong\n");
// 	printf("num is: %s\n", out);
// 	free(out);
// 	return (0);
// }