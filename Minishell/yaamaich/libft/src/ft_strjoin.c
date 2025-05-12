/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 04:21:59 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/15 18:29:59 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	k;
	char			*p;

	k = 0;
	i = 0;
	if (!s1 || !s2)
		return (NULL);
	p = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!p)
		return (NULL);
	while (s1[i])
		p[k++] = (char)s1[i++];
	i = 0;
	while (s2[i])
		p[k++] = (char)s2[i++];
	p[k] = '\0';
	return (p);
}
