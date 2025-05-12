/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:02:29 by yaamaich          #+#    #+#             */
/*   Updated: 2024/11/19 20:02:32 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*p;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		p = *lst;
		*lst = (*lst)->next;
		del(p->content);
		free(p);
	}
	*lst = NULL;
}
