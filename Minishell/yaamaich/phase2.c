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

void handl_word(t_parser *parser, t_token *token)
{
    if (parser->rider)
    {
        if (!is_empty(parser->op_stack))
        {
            t_token *top_op = pop_stack(&parser->op_stack);
            enqueue(parser->output_queue, top_op);
        }
        enqueue(parser->output_queue, token);
        enqueue(parser->output_queue, parser->rider);
        parser->rider = NULL;
    }else
        enqueue(parser->output_queue, token);
}
void handle_operators(t_parser *parser, t_token *op_token)
{
     if (op_token->type == REDIR_IN ||
    op_token->type == REDIR_OUT ||
    op_token->type == APPEND ||
    op_token->type == HEREDOC)
    {
        parser->rider = op_token;
    }else
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
}

void process_token(t_parser *parser, t_token *token)
{
    if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
    {
        handl_word(parser, token);
    } else if (token->type == OP_TOKEN || token->type == REDIR_IN || token->type == REDIR_OUT
     || token->type == APPEND  || token->type == HEREDOC 
     || token->type == AND_IF  || token->type == OR_IF)
    {
        handle_operators(parser, token);
    }else if (token->type == L_PAREN_TOKEN)
    {
        push_stack(&parser->op_stack, token);
    } else if (token->type == R_PAREN_TOKEN)
    {
        while (!is_empty(parser->op_stack) && top_stack(parser->op_stack)->token->type != L_PAREN_TOKEN)
        {
            t_token *op = pop_stack(&parser->op_stack);
            enqueue(parser->output_queue, op);
        }
        if (is_empty(parser->op_stack))
        {
            report_mesage("Mismatched parentheses\n"); return;
        }
        pop_stack(&parser->op_stack);
    }
    return ;
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
