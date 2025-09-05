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
    token->quete_type = 0;
    token->pipe_read = -1;
    token->pipe_write = -1;

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
    parser->rider = NULL;
	return (parser);
}
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
//phase 3 // 
t_node *create_tree_node(t_cmd_node *cmd_node) {
    t_node *node = malloc(sizeof(t_node));
    if (!node) return NULL;
    node->token = NULL;
    node->token_type = CMD_TOKEN;
    node->cmd = cmd_node; // Store the command node here!
    node->left = NULL;
    node->right = NULL;
    return node;
}
