/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 04:40:28 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/26 06:44:52 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//PHASE 2//

int precedence(t_token_type type)
{
	if (type == OP_TOKEN)
		return (1);
	else if (type == AND_IF)
		return (2);
	else if (type == OR_IF)
		return (3);
	else if (type == REDIR_IN || type == REDIR_OUT)
		return (4);
	else if (type == APPEND || type == HEREDOC)
		return (5);
	else
		return (0);
}
void enqueue(t_queue *queue, t_token *token)
{
	t_queue_node *new_queue;

	new_queue = malloc (sizeof(t_queue_node));
	new_queue->token = token;
	new_queue->next = NULL;
	
	if (queue->head == NULL)
	{
		queue->head = new_queue;
		queue->tail = new_queue;
	}else
	{
		queue->tail->next = new_queue;
		queue->tail = new_queue;
	}
}

void handle_operators(t_parser *parser, t_token *op_token)
{
	t_node *top_op;

	
	while (!(is_empty(parser->op_stack)) && precedence(top_stack(parser->stack)->token->type)
		>= precedence(op_token->type) && top_stack(parser->stack)->token->type != L_PAREN_TOKEN)
	{
		top_op = pop_stack(&parser->stack);
		
		enqueue(parser->output_queue, top_op->token);
	}
	push_stack(&parser->stack, op_token);
}
void process_token(t_parser *parser, t_token *token)
{
	t_node *op;

	if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
		enqueue(parser->output_queue, token);
	else if (token->type == OP_TOKEN)
		handle_operators(parser, token);
	else if (token->type == L_PAREN_TOKEN)
		push_stack(&parser->op_stack, token);
	else if (token->type == L_PAREN_TOKEN)
	{
		while (!(is_empty(parser->op_stack)) && top_stack(parser->stack)->token->type)
		{
			op = pop_stack(&parser->op_stack);
			enqueue(parser->output_queue, op->token);
		}
		if (!is_empty(parser->op_stack))
			pop_stack(&parser->stack);
		else
			report_mesage("Mismatched paentheses");
	}
}

void finalize_parsing(t_parser *parser)
{
	t_node *op;
	
	while (!(is_empty(parser->op_stack)))
	{
		op = pop_stack(&parser->stack);
		if (op->token->type == L_PAREN_TOKEN)
			report_mesage("Mismatched parentheses");
		enqueue(parser->output_queue, op->token);
	}
}