/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_use.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:26:42 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/13 20:35:13 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    push_stack(t_node_stack **stack, t_node *node)
{
	t_node_stack *new_node;
	
	new_node = malloc (sizeof(t_node_stack));
	new_node->node = node;
	new_node->next = *stack;
	*stack = new_node;
}

t_node  *pop_stack(t_node_stack **stack)
{
	t_node_stack *top;
	t_node  *node;

	if (!stack || !*stack)
		return NULL;
	top = *stack;
	node = top->node;
	*stack = top->next;
	free (top);
	return node;
}

t_node *top_stack(t_node_stack *stack)
{
	if(!stack)
		return NULL;
	return (stack->node);
}

int	size_node_stack(t_node_stack *stack)
{
	int size;
	
	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}
