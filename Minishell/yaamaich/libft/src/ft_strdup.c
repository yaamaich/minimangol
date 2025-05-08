/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 03:24:08 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/12 19:26:47 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	size_t	i;

	i = ft_strlen(s1);
	ptr = malloc(i + 1);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s1, i);
	ptr[i] = '\0';
	return (ptr);
}
