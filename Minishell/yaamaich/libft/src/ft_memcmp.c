/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:18:41 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/19 16:36:08 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*i;
	unsigned char	*l;

	i = (unsigned char *)s1;
	l = (unsigned char *)s2;
	if (n == 0)
	{
		return (0);
	}
	while (*i == *l && --n > 0)
	{
		i++;
		l++;
	}
	return (*i - *l);
}
