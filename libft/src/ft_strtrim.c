/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 02:29:39 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/17 03:39:37 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;
	char	*p;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	if (s1[0] == '\0')
		return (ft_strdup(""));
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	len = ft_strlen(s1);
	while (len > i && ft_strchr(set, s1[len - 1]))
		len--;
	p = ft_substr(s1, i, len - i);
	return (p);
}
