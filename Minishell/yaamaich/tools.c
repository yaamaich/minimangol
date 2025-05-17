/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:07:51 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/13 21:04:33 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//phase 1 // 

int whitespaces(char str)
{
	if (str == 32 || str >= 9 && str <= 13)
		return (1);
	return (0);
}
t_stack *creat_empty_stack(t_stack *stack)
{
	
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

t_node *create_operator_node(t_token *token, t_node *left, t_node *right)
{
    t_node *node = malloc(sizeof(t_node));
    node->token = token;
    node->left = left;
    node->right = right;
    return node;
}
