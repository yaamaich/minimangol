/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 05:19:41 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/17 03:55:14 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	unsigned int	k;
	char			*p;

	k = 0;
	if (!s)
		return (NULL);
	i = ft_strlen(s);
	if (len > i - start)
		len = i - start;
	if (start >= i)
		return (ft_strdup(""));
	p = malloc(len + 1);
	if (!p)
		return (NULL);
	while (s[start] && k < len)
	{
		p[k] = s[start];
		k++;
		start++;
	}
	p[k] = '\0';
	return (p);
}
