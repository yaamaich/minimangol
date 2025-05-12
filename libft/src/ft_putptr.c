/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 07:13:20 by yaamaich          #+#    #+#             */
/*   Updated: 2025/04/09 04:53:25 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_putnbrptr(unsigned long n)
{
	int		count;
	char	*symbols1;

	count = 0;
	symbols1 = "0123456789abcdef";
	if (n < 16)
		return (ft_putchar(symbols1[n]));
	else
	{
		count = ft_putnbrptr(n / 16);
		return (count + ft_putnbrptr(n % 16));
	}
}

int	ft_putptr(void *ptr)
{
	int	len;
	int	aux;

	len = 0;
	aux = 0;
	ft_putstr("0x");
	len += 2;
	aux = ft_putnbrptr((unsigned long)ptr);
	len += aux;
	return (len);
}
