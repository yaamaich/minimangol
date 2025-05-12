/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 01:16:27 by yaamaich          #+#    #+#             */
/*   Updated: 2025/04/09 04:57:03 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_format(char specifier, va_list ap)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count = ft_putchar(va_arg(ap, int));
	else if (specifier == 's')
		count = ft_putstr(va_arg(ap, char *));
	else if (specifier == 'd' || specifier == 'i')
		count = ft_putnbr_max((long)va_arg(ap, int), 10, specifier);
	else if (specifier == 'u')
		count = ft_putunsnbr((long)va_arg(ap, int));
	else if (specifier == 'x' || specifier == 'X')
		count = ft_putnbr_max((long)va_arg(ap, unsigned int), 16, specifier);
	else if (specifier == 'p')
		count = ft_putptr(va_arg(ap, void *));
	else
		count += write(1, &specifier, 1);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		count;

	va_start(ap, format);
	count = 0;
	if (write(1, 0, 0) == -1)
	{
		return (-1);
	}
	while (*format)
	{
		if (*format == '%' && *format + 1)
		{
			format++;
			if (!*format)
				return (count);
			count += print_format(*format, ap);
		}
		else
			count += write(1, format, 1);
		format++;
	}
	va_end(ap);
	return (count);
}
