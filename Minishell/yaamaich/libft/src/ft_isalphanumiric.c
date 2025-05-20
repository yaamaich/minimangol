/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalphanumiric.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 22:28:47 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/18 22:37:18 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalphanumiric(char str)
{
	if (ft_isalpha(str) || (ft_isdigit(str)))
		return (0);
	else
		return (1);
}