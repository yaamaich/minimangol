/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:00:24 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/01 23:16:00 by yaamaich         ###   ########.fr       */
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
	R_PAREN_TOKEN
}		t_token_type;

typedef struct s_token
{
	t_token_type	type;
	t_token_type	op_type;
	char			*value;
	char			quete_type;
	
}				t_token;

#endif