/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 04:40:28 by yaamaich          #+#    #+#             */
/*   Updated: 2025/08/28 07:54:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//PHASE 2//

int precedence(t_token_type type)
{
    if (type == REDIR_IN || type == REDIR_OUT)       // < or >
        return 4;
    else if (type == APPEND || type == HEREDOC)     // >> or <<
        return 4;
    else if (type == OP_TOKEN)                      // |
        return 3;
    else if (type == AND_IF || type == OR_IF)       // && or ||
        return 1;
    else
        return 0;
}


void enqueue(t_queue *queue, t_token *token)
{
	t_queue_node *new_queue;

	new_queue = malloc (sizeof(t_queue_node));
    new_queue->cmd_token = NULL;
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
void cmd_enqueue(t_queue *queue, t_cmd_node *token)
{
	if (!queue || !token)
		return;
		
	t_queue_node *new_queue = malloc(sizeof(t_queue_node));
	if (!new_queue)
		return;
		
	new_queue->cmd_token = token;
	new_queue->token = NULL;  // Make sure token field is NULL
	new_queue->next = NULL;
	
	if (queue->head == NULL)
	{
		queue->head = new_queue;
		queue->tail = new_queue;
	}
	else
	{
		queue->tail->next = new_queue;
		queue->tail = new_queue;
	}
}


void handle_operators(t_parser *parser, t_token *op_token)
{
    // Pop while: stack not empty, top is NOT '(', and top precedence >= new op
    while (!is_empty(parser->op_stack)
        && top_stack(parser->op_stack)->token->type != L_PAREN_TOKEN
        && precedence(top_stack(parser->op_stack)->token->type) >= precedence(op_token->type))
    {
        t_token *top_op = pop_stack(&parser->op_stack);
        enqueue(parser->output_queue, top_op);
    }
    // Push the new operator
    push_stack(&parser->op_stack, op_token);
}

void process_token(t_parser *parser, t_token *token)
{
    if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
    {
        // operands go straight to output
        enqueue(parser->output_queue, token);
        return;
    }

    // operators (|, <, >, >>, <<, &&, ||)
    if (token->type == OP_TOKEN || token->type == REDIR_IN || token->type == REDIR_OUT
     || token->type == APPEND  || token->type == HEREDOC
     || token->type == AND_IF  || token->type == OR_IF)
    {
        handle_operators(parser, token);
        return;
    }

    // '(' just gets pushed
    if (token->type == L_PAREN_TOKEN)
    {
        push_stack(&parser->op_stack, token);
        return;
    }

    // ')' pops until '(' (do NOT enqueue '(')
    if (token->type == R_PAREN_TOKEN)
    {
        while (!is_empty(parser->op_stack)
            && top_stack(parser->op_stack)->token->type != L_PAREN_TOKEN)
        {
            t_token *op = pop_stack(&parser->op_stack);
            enqueue(parser->output_queue, op);
        }

        if (is_empty(parser->op_stack))
        {
            report_mesage("Mismatched parentheses\n");
            return; // nothing to pop
        }

        // Pop the '(' and discard it (do NOT enqueue it)
        t_token *lp = pop_stack(&parser->op_stack);
        (void)lp; // unused
        return;
    }

    // Any other token? Just enqueue (safe default)
    enqueue(parser->output_queue, token);
}
void finalize_parsing(t_parser *parser)
{
    while (!is_empty(parser->op_stack))
    {
        t_token *op = pop_stack(&parser->op_stack);
        if (op->type == L_PAREN_TOKEN)
        {
            report_mesage("Mismatched parentheses\n");
            // DO NOT enqueue '('
            continue;
        }
        enqueue(parser->output_queue, op);
    }
}
