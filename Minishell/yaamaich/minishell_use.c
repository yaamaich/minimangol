/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_use.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:26:42 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/26 06:49:28 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_stack	*creat_empty_stack (void)
{
	t_stack *stack;
	stack = malloc(sizeof(t_stack));
	stack->top = NULL;
	return (stack);
}

void    push_stack(t_stack **stack, t_token *token)
{
    t_stack_node *new_node = malloc(sizeof(t_stack_node));
    if (!new_node)
        return; // Handle error
    new_node->token = token;
    new_node->next = (*stack)->top;
    (*stack)->top = new_node;
}

t_token *pop_token_stack(t_stack **stack)
{
    if (!stack || !*stack || !(*stack)->top)
        return NULL;

    t_stack_node *top_node = (*stack)->top;
    t_token *token = top_node->token;

    (*stack)->top = top_node->next;
    free(top_node);
    return token;
}
t_node *pop_node_stack(t_node_stack **stack) {
    if (!stack || !*stack) 
        return NULL;

    t_node_stack *top_node = *stack;
    t_node *node = top_node->node;

    *stack = top_node->next;
    free(top_node);
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
