/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 01:20:24 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/19 23:53:54 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
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

char	*ft_strchr(const char *ptr, int ch)
{
	int	p;

	if (!ptr)
		return (NULL);
	if ((unsigned char)ch == '\0')
		return ((char *)ptr + ft_strlen(ptr));
	p = ft_strlen(ptr);
	while (p >= 0)
	{
		if (*ptr == (char)ch)
			return ((char *)ptr);
		p--;
		ptr++;
	}
	return (NULL);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	unsigned int	i;
	unsigned int	k;
	char			*p;

	k = 0;
	i = 0;
	if (!s2)
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

char	*ft_strdup(const char *s)
{
	char	*ptr;
	int		len;

	len = ft_strlen(s);
	ptr = malloc(len + 1);
	if (!ptr)
		return (NULL);
	ptr[len] = '\0';
	while (len--)
		ptr[len] = s[len];
	return (ptr);
}
