/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:44:56 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/17 04:23:39 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *to, const void *from, size_t c)
{
	size_t			i;
	unsigned char	*dest;
	unsigned char	*src;

	i = 0;
	dest = (unsigned char *)to;
	src = (unsigned char *)from;
	if ((src == NULL && dest == NULL))
		return (NULL);
	while (i < c)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}
