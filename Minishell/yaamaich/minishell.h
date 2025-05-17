/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:00:24 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/13 22:02:21 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINI_SHELL_H
# define MINI_SHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
# include "libft/src/libft.h"


typedef struct s_lexer
{
	char *input;
	int  position;
	int  length;
}               t_lexer;

typedef enum {
	CMD_TOKEN,
	WORD_TOKEN,
	OP_TOKEN,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	L_PAREN_TOKEN,
	R_PAREN_TOKEN,
	R_PAREN_TOKEN
}		t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	char			quete_type;
	
}				t_token;
typedef	struct	s_stack_node
{
	t_token					*token;
	struct s_stack_node		*next;
}				t_stack_node;

typedef struct s_stack
{
	t_stack_node	*top;
} 				t_stack;

typedef struct s_queue_node
{
	t_token					*token;
	struct s_queue_node		*next;
}				t_queue_node;

typedef struct s_queue
{
	t_queue_node	*head;
	t_queue_node	*tail;
}				t_queue;

typedef struct s_parser
{
    t_stack  *op_stack;
    t_queue  *output_queue;
}
				t_parser;
				
typedef struct s_node {
    t_token        *token;
    struct s_node *left;
    struct s_node *right;
} 				t_node;

typedef struct s_node_stack
{
	t_node				*node;
	struct s_node_stack	*next;
}				t_node_stack;



int whitespaces(char str);
t_lexer *initialize_lexer(char *string);
t_parser *initialize_shunting_yard(void);
t_node *create_tree_node(t_token *token);
t_node *create_operator_node(t_token *token, t_node *left, t_node *right);

char    *get_next_token(t_lexer *lexer);
void finalize_parsing(t_parser *parser);
t_list *build_command_tree(t_parser *parser);
t_token_type    classify_token(t_token *token);
void process_token(t_parser *parser, t_token *token);
void handle_operators(t_parser *parser, t_token *op_token);
t_token     *handle_quotes(t_lexer *lexer, char quote_char);

t_node *top_stack(t_node_stack *stack);
int	size_node_stack(t_node_stack *stack);
t_node  *pop_stack(t_node_stack **stack);
void    push_stack(t_node_stack **stack, t_node *node);

#endif 