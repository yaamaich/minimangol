/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 01:11:25 by yaamaich          #+#    #+#             */
/*   Updated: 2025/04/09 04:57:49 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(char *str)
{
	unsigned int	i;
	int				count;

	count = 0;
	i = 0;
	if (!str)
		return (write(1, "(null)", 6));
	while (str[i])
	{
		ft_putchar(str[i]);
		i++;
		count++;
	}
	return (count);
}
