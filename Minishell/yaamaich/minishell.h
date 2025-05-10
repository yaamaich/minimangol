/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:00:24 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/09 22:12:46 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINI_SHELL_H
# define MINI_SHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
# include "libft/src/libft.h"


#define top(stack) ((stack)->top->token)


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
	t_queue_node	*front;
	t_queue_node	*rear;
}				t_queue;

typedef struct s_parser
{
    t_stack  *op_stack;
    t_queue  *output_queue;
}				t_parser;

#endif 