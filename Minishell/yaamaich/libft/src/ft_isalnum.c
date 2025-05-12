/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:28:42 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/17 02:03:40 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int str)
{
	if (ft_isalpha(str) || (ft_isdigit(str)))
		return (1);
	else
		return (0);
}
