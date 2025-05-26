/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 04:48:26 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/26 06:22:29 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token *handle_operator(t_lexer *lexer)
{
    t_token *token;
    char	*input;
    int 	pos;

	input = lexer->input;
	pos = lexer->position;
    token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    if (input[pos] == '>' && input[pos + 1] == '>')
    {
        token->value = ft_strdup(">>");
        lexer->position += 2;
    }else if (input[pos] == '<' && input[pos + 1] == '<')
    {
        token->value = ft_strdup("<<");
        lexer->position += 2;
    }else
    {
        token->value = ft_substr(input, pos, 1);
        lexer->position += 1;
    }
    token->type = classify_token(token);
    return token;
}
t_token *handle_word(t_lexer *lexer)
{
    int start = lexer->position;
    
    while (lexer->position < lexer->length && !whitespaces(lexer->input[lexer->position]))
    {
        lexer->position++;
    }

    int word_length = lexer->position - start;
    if (word_length <= 0)
        return NULL;

    char *word = malloc(word_length + 1);
    if (!word)
        return NULL;

    for (int i = 0; i < word_length; i++)
        word[i] = lexer->input[start + i];
    word[word_length] = '\0';

    t_token *token = malloc(sizeof(t_token));
    if (!token)
    {
        free(word);
        return NULL;
    }

    token->type = CMD_TOKEN;
    token->value = word;
    token->quete_type = 0;
    token->pipe_read = -1;
    token->pipe_write = -1;

    return token;
}


t_token    *get_next_token(t_lexer *lexer)
{
	char current_char; 

	while (lexer->position < lexer->length && whitespaces(lexer->input[lexer->position]))
		lexer->position++;
	if (lexer->position >= lexer->length)
		return (NULL);
	current_char = lexer->input[lexer->position];
	
	if (current_char == '\'' || current_char == '"')
		return (handle_quotes(lexer, current_char));
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
	char    *content;
	
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

t_token	*first_token_in_command(t_token *token)
{
	if (token->type == CMD)
		return token;
	return NULL;
}

 
t_token_type    classify_token(t_token *token)
{
	if (ft_strcmp(token->value, "|") == 0)
		return (OP_TOKEN);
	else if (ft_strcmp(token->value, "||") == 0)
    	return (OR_IF);
	else if (ft_strcmp(token->value, "&&") == 0)
    	return (AND_IF);
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
	else if (first_token_in_command(token))
		return (CMD_TOKEN);
	else
		return	(WORD_TOKEN);
}
