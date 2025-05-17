/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:23:49 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/17 12:50:08 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//PHASE 1//

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


char    *get_next_token(t_lexer *lexer)
{
	char current_char; 

	while (lexer->position < lexer->length && is_whitespaces(lexer->input[lexer->position]))
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
//PHASE 2//

int precedence(t_token_type type)
{
	if (type == OP_TOKEN)
		return (1);
	else if (type == AND_IF)
		return (2);
	else if (type == OR_IF)
		return (3);
	else if (type == REDIR_IN || type == REDIR_OUT)
		return (4);
	else if (type == APPEND || type == HEREDOC)
		return (5);
	else
		return (0);
}
void enqueue(t_queue *queue, t_token *token)
{
	t_queue_node *new_queue;

	new_queue = malloc (sizeof(t_queue_node));
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

void handle_operators(t_parser *parser, t_token *op_token)
{
	t_node *top_op;

	
	while (!(is_empty(parser->op_stack)) && precedence(top_stack(parser->op_stack)->token->type)
		>= precedence(op_token) && top_stack(parser->op_stack)->token->type != L_PAREN_TOKEN)
	{
		top_op = pop_stack(parser->op_stack);
		
		enqueue(parser->output_queue, top_op->token);
	}
	push_stack(parser->op_stack, op_token);
}
void process_token(t_parser *parser, t_token *token)
{
	t_node *op;

	if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
		enqueue(parser->output_queue, token);
	else if (token->type == OP_TOKEN)
		handle_operators(parser, token);
	else if (token->type == L_PAREN_TOKEN)
		push_stack(parser->op_stack, token);
	else if (token->type == L_PAREN_TOKEN)
	{
		while (!(is_empty(parser->op_stack)) && top_stack((parser->op_stack)->top->token->type))
		{
			op = pop_stack(parser->op_stack);
			enqueue(parser->output_queue, op);
		}
		if (!is_empty(parser->op_stack))
			pop_stack(parser->op_stack);
		else
			report_error("Mismatched paentheses");
	}
}

void finalize_parsing(t_parser *parser)
{
	t_token *op;
	
	while (!(is_empty(parser->op_stack)))
	{
		op = pop_stack(parser->op_stack);
		if (op->type == L_PAREN_TOKEN)
		{
			report_error("Mismatched parentheses");
		}
		enqueue(parser->output_queue, op);
	}
}
//PHASE 3//
t_token *dequeue(t_queue *queue)
{
	t_token *token;
	t_queue_node *node_to_remove;
	
	if (queue->head == NULL)
		return NULL;
	node_to_remove = queue->head;
	token = node_to_remove->token;
	queue->head = node_to_remove->next;

	if (queue->head == NULL)
		queue->tail = (NULL);
	free(node_to_remove);
	return (token);
}

t_node*create_tree_node(t_token *token)
{
	t_node *node;
	
	node = malloc(sizeof(t_node));
	node->token = token;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node *create_operator_node(t_token *token, t_node *left, t_node *right)
{
	t_node *new_node;

	new_node = malloc(sizeof(t_node));
	new_node->token = token;
	new_node->left = left;
	new_node->right = right;
	return (new_node);	
}
t_node *build_command_tree(t_parser *parser)
{
	t_node_stack	*stack;
	t_token 		*token;
	t_node			*node;
	t_node			*left;
	t_node			*right;
	
	stack = malloc (sizeof(t_node_stack));
	stack->next = NULL;
	while (!(is_empty(parser->output_queue)))
	{
		token = dequeue(parser->output_queue);
		if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
		{
			node = creat_tree_node(token);
			push_stack(&stack, node);
		}else if (token->type == OP_TOKEN)
		{
			if (size_node_stack(stack) < 2)
			{
				report_message("Invalid expression");
				return NULL;
			}

			right = pop_stack(stack);
			left = pop_stack(stack);
			node = create_operator_node(token, left, right);
			push_stack(&stack, node);
		}
	}
	if (size_node_stack(stack) != 1)
	{
		report_message("Invalid expression");
		return (NULL);
	}
	return (pop_stack(stack));
}

t_cmd_node	*creat_command_node(char *cmd, char *args)
{
	t_cmd_node *node;

	node = malloc(sizeof(t_cmd_node));
	node->cmd = ft_strdup(cmd);
	node->args = ft_strdup(args);
	node->redir = NULL;
	node->next = NULL;
	return (node);
}
void add_redirection(t_cmd_node *cmd, t_redir *redir)
{
	t_redir *current;

	
	if (cmd->redir == NULL)
		cmd->redir = redir;
	else
	{
		current = cmd->redir;
		while (current->next != NULL)
			current = current->next;
		current->next = redir;
	}
}

t_node *connect_commands(t_node *left, t_node *right, t_op_node *op)
{
	
}

int main()
{
	char str = 's';
	printf("%c", str);
}