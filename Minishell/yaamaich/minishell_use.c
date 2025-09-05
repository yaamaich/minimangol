/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_use.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:26:42 by yaamaich          #+#    #+#             */
/*   Updated: 2025/09/05 20:56:45 by marvin           ###   ########.fr       */
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

t_token *pop_stack(t_stack **stack)
{
	if (!stack || !*stack || !(*stack)->top)
		return NULL;

	t_stack_node *top_node = (*stack)->top;
	t_token *token = top_node->token;

	(*stack)->top = top_node->next;
	free(top_node);
	return token;
}

// t_token *pop(t_op_stack *stack) {
//     if (!stack->top) return NULL;
	
//     t_stack_node *temp = stack->top;
//     t_token *popped_token = temp->token;
//     stack->top = stack->top->next;
//     free(temp);
//     return popped_token;
// }

t_stack_node *top_stack(t_stack *stack)
{
	if(!stack)
		return NULL;
	return (stack->top);
}

int size_node_stack(t_stack *stack)
{
	int size = 0;
	t_stack_node *current = stack->top;  // Use a separate pointer
	
	while (current)
	{
		size++;
		current = current->next;  // Move the temporary pointer, not stack->top
	}
	return (size);
}

//phase 3//
t_ast_stack *create_ast_stack() {
	t_ast_stack *stack = malloc(sizeof(t_ast_stack));
	stack->top = NULL;
	stack->size = 0;
	return stack;
}

void ast_push(t_ast_stack *stack, t_node *node)
{
	if (!stack || !node) return;

	t_ast_stack_node *new_node = malloc(sizeof(t_ast_stack_node));
	new_node->node = node;
	new_node->next = stack->top;
	stack->top = new_node;
	stack->size++;
}

t_node *ast_pop(t_ast_stack *stack) {
	if (!stack->top) return NULL;
	
	t_ast_stack_node *temp = stack->top;
	t_node *popped_node = temp->node;
	stack->top = stack->top->next;
	stack->size--;
	free(temp);
	return popped_node;
}
int ast_stack_size(t_ast_stack *stack) {
	return stack->size;
}
