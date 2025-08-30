/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:07:51 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/29 16:20:19 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//phase 1 // 

void report_mesage (char *str)
{
	ft_printf("%s", str);
}

int is_empty(t_stack *stack)
{
	if (stack->top == NULL)
		return (1);
	else
		return (0);
}
int whitespaces(char str)
{
	if (str == 32 ||( str >= 9 && str <= 13))
		return (1);
	return (0);
}
t_lexer *initialize_lexer(char *string)
{
	t_lexer *lexer;
	
	lexer = malloc(sizeof(t_lexer));
	lexer->input = string;
	lexer->position = 0;
	lexer->length = ft_strlen(string);
	return lexer;
}

// phase 2 //
t_token *create_token(t_token_type type, const char *value)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;

    token->type = type;
    token->value = ft_strdup(value);
    token->quete_type = 0;    // بشكل افتراضي لا يوجد اقتباس
    token->pipe_read = -1;    // افتراضياً لا يوجد اتصال أنبوب قراءة
    token->pipe_write = -1;   // افتراضياً لا يوجد اتصال أنبوب كتابة

    return token;
}

t_queue *creat_empty_queue(void)
{
    t_queue *queue;

    queue = (t_queue *)malloc(sizeof(t_queue));
    if (!queue)
        return (NULL);
    queue->head = NULL;
    queue->tail = NULL;
    return (queue);
}

t_parser *initialize_shunting_yard(void)
{
	t_parser *parser;
	
	parser = malloc(sizeof(t_parser));
	parser->op_stack = creat_empty_stack();
	parser->output_queue = creat_empty_queue();
	return (parser);
}

//phase 3 // 

t_node *create_tree_node(t_token *token)
{
    t_node *node = malloc(sizeof(t_node));
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}
