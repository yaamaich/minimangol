/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 00:33:21 by yaamaich          #+#    #+#             */
/*   Updated: 2025/04/03 23:17:45 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word(char *p, char c)
{
	int	i;
	int	count;

	if (!p)
		return (0);
	count = 0;
	i = 0;
	while (p[i])
	{
		if ((p[i] != c && (p[i + 1] == '\0' || p[i + 1] == c)))
			count++;
		i++;
	}
	return (count);
}

static void	free_split(char **split, int j)
{
	int	k;

	k = 0;
	while (k <= j)
	{
		free(split[k]);
		k++;
	}
	free(split);
}

static char	**ft_split2(char const *s, char c, char **split, int i)
{
	int	j;
	int	start;

	j = 0;
	start = 0;
	split = malloc((count_word((char *)s, c) + 1) * sizeof(char *));
	if (!s || !split)
		return (NULL);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			split[j] = ft_substr(s, start, i - start);
			if (!split[j])
				return (free_split(split, j - 1), NULL);
			j++;
		}
	}
	split[j] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		i;

	split = NULL;
	i = 0;
	return (ft_split2(s, c, split, i));
}
