/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:23:49 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/09 22:15:45 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int whitespaces(char str)
{
	if (str[i] == 32 || str[i] >= 9 && str[i] <= 13)
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
char    *get_next_token(t_lexer *lexer)
{
	char current_char; 

	while (lexer->position < lexer->length && is_whitespaces(lexer->input[lexer->position]))
		lexer->position++;
	if (lexer->position >= lexer->length)
		return (NULL);
	current_char = lexer->input[lexer->position];
	
	if (current_char == '\'' || current_char == '"')
		return (handle_quotes(lexer, current_char))
	if (current_char == '|' || current_char == '<' || current_char == '>' 
		|| current_char == '(' || current_char == ')')
			return (handle_operator(lexer));
	return (handle_word(lexer));
}
t_token     *handle_quotes(t_lexer *lexer, char quote_char)
{
	int     start;
	int     content_length;
	t_token *token;
	char    content;
	
	lexer->position++;
	start = lexer->position;
	while (lexer->position < lexer->length && lexer->input[lexer->position] != quote_char)
		lexer->position++;
	if (lexer->position >= lexer->length)
		ft_printf("Unclosed quote");
	content_length = lexer->position - start;
	content = ft_substr(lexer->input, start, content_length);
	
	lexer->position++;

	token = malloc(sizeof(t_token));
	token->value = content;
	token->quete_type = quote_char;
	token->type = WORD_TOKEN;

	return (token);
}

void handle_operator(t_parser *parser, t_token *op_token)
{
	
}

t_token_type    classify_token(t_token *token)
{
	if (ft_strcmp(token->value, "|") == 0)
		return (OP_TOKE);
	else if ((ft_strcmp(token->value, "<") == 0))
		return  (REDIR_IN);
	else if ((ft_strcmp(token->value, ">") == 0))
		return  (REDIR_OUT);
	else if ((ft_strcmp(token->value, ">>") == 0))
		return  (APPEND);
	else if ((ft_strcmp(token->value, "<<") == 0))
		return  (HEREDOC);
	else if ((ft_strcmp(token->value, "(") == 0))
		return (L_PAREN_TOKEN);
	else if ((ft_strcmp(token->value, ")") == 0))
		return (R_PAREN_TOKEN);
	else if (is_the first_token_in_command(token))
		return (CMD_TOKEN)
	else
		return	(WORD_TOKEN);
}

t_parser *initialize_shunting_yard(void)
{
	t_parser *parser;
	
	parser = malloc(sizeof(t_parser));
	parser->op_stack = creat_empty_stack();
	parser->output_queue = creat_empty_queue();
	return (parser);
}

void process_token(t_parser *parser, t_token *token)
{
	int op;

	if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
		enqueue(parser->output_queue, token);
	else if (token->type == OP_TOKEN)
		handle_operator(parser, token);
	else if (token->type == L_PAREN_TOKEN)
		push(parser->op_stack, token);
	else if (token->type == L_PAREN_TOKEN)
	{
		while (!(is_empty(parser->op_stack)) && top((parser->op_stack)->type))
		{
			op = pop(parser->op_stack);
			enqueue(parser->output_queue, op);
		}
		if (!is_empty(parser->op_stack))
			pop(parser->op_stack);
		else
			report_error("Mismatched paentheses");
	}
}
void handle_operator(t_parser *parser, t_token *op_token)
{
	while (!(is_empty(parser->op_stack)) && precedence(top(parser->op_stack)->type)
		>= precedence(op_token) && top(parser->op_stack)->type != L_PAREN_TOKEN)
	{
		top_op = pop(parser->op_stack);	
		
	}
}

int main()
{
	char str = 's';
	printf("%c", str);
}