/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 00:11:08 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/17 03:28:42 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str1, const char *str2, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!*str2)
		return ((char *)str1);
	if (str1 == NULL && len == 0)
		return (NULL);
	while (str1[i] && i < len)
	{
		if (str1[i] == str2[0])
		{
			j = 0;
			while (str2[j] && str1[i + j] && (i + j) < len && 
				str1[i + j] == str2[j])
				j++;
			if (!str2[j])
				return ((char *)(str1 + i));
		}
		i++;
	}
	return (NULL);
}
