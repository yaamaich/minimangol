/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 04:43:16 by yaamaich          #+#    #+#             */
/*   Updated: 2025/08/28 11:41:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//PHASE 3//

int is_operator(t_token_type str)
{
	if (!str)
		return (0);
	if ((str == OP_TOKEN) || (str == AND_IF) || 
		(str == OR_IF) || (str == REDIR_OUT)  || 
		(str == REDIR_IN) || (str == APPEND) )
		return (1);
	return (0);
}
t_op_node *create_op_node(t_token *token)
{
	t_op_node *op_node = malloc(sizeof(t_op_node));

	if (!op_node)
	return NULL;

	op_node->token = token;
	op_node->precedence = precedence(token->type);
	op_node->type = token->type;
	
	if (token->type == OP_TOKEN) {
		op_node->op_str = "|";
	} else if (token->type == AND_IF) {
		op_node->op_str = "&&";
	} else if (token->type == OR_IF) {
		op_node->op_str = "||";
	}
	return op_node;
}

t_token *dequeue(t_queue *queue)
{
	t_token *token;
	t_queue_node *node_to_remove;
	
	if (!queue || queue->head == NULL)
		return NULL;
	node_to_remove = queue->head;
	token = node_to_remove->token;
	queue->head = node_to_remove->next;

	if (queue->head == NULL)
		queue->tail = (NULL);
	free(node_to_remove);
	return (token);
}

t_cmd_node *cmd_dequeue(t_queue *queue)
{
	t_cmd_node *token;
	t_queue_node *node_to_remove;
	
	if (!queue || queue->head == NULL)
		return NULL;
	node_to_remove = queue->head;
	token = node_to_remove->cmd_token;
	queue->head = node_to_remove->next;

	if (queue->head == NULL)
		queue->tail = (NULL);
	free(node_to_remove);
	return (token);
}
void handle_redir(t_node *node, t_token_type type)
{
        t_redir *redir = create_redir(type, node->cmd->cmd);
        // Attach redir to current command node here!
        add_redirection(node->cmd, redir); // You need to track current_cmd_node
}
t_node *build_command_tree(t_parser *parser)
{
	t_ast_stack		*stack;
	t_op_node 		*op_node;
	t_node 			*new_node; 
	t_queue_node	*cmd;
	t_node			*node;
	t_node			*left;
	t_node			*right;
	
	stack = malloc(sizeof(t_ast_stack));
	if (!stack || !parser || !parser->output_queue)
		return NULL;
	stack->top = NULL;
	stack->size = 0;
	cmd = parser->output_queue->head;
	while (cmd)
	{
		if (cmd && cmd->cmd_token && cmd->cmd_token->cmd)
		{
			node = create_tree_node( cmd->cmd_token);
			if (node)
				ast_push(stack, node);
		}else if (cmd->token->type == OP_TOKEN || 
         cmd->token->type == REDIR_IN || cmd->token->type == REDIR_OUT || 
         cmd->token->type == APPEND || cmd->token->type == HEREDOC ||
         cmd->token->type == AND_IF || cmd->token->type == OR_IF)
		{
			if (ast_stack_size(stack) < 2)
			{
				report_mesage("Invalid expression");
				return NULL;
			}
			right = ast_pop(stack);
			left = ast_pop(stack);
			if ( cmd->token->type == REDIR_IN || cmd->token->type == REDIR_OUT || 
			  cmd->token->type == APPEND || cmd->token->type == HEREDOC )
			{
				handle_redir(right, cmd->token->type);
			}
			if (!left || !right)
			{
				report_mesage("Invalid AST pop");
				return NULL;
			}
			op_node = create_op_node(cmd->token);
			if (!op_node || !op_node->token)
			{
				report_mesage("Invalid operator node");
				return NULL;
			}
		   	new_node = connect_commands(left, right, op_node);
			if (!new_node)
			{
				report_mesage("Invalid command connection");
				return NULL;
			}
			ast_push(stack, new_node);
		}
		cmd = cmd->next;
	}
	if (ast_stack_size(stack) < 1)
	{
		report_mesage("Invalid expression");
		return (NULL);
	}
	return (ast_pop(stack));
}

void add_argument(t_cmd_node *cmd, char *arg)
{
	// Reallocate args array to add new argument
	int new_size;
	int i;
	
	i = 0;
	new_size = cmd->arg_count + 2; // +1 for new arg, +1 for NULL terminator
	char **new_args = malloc(sizeof(char *) * new_size);
	
	// Copy existing arguments
	while (i < cmd->arg_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	// Add new argument
	new_args[cmd->arg_count] = ft_strdup(arg);
	new_args[cmd->arg_count + 1] = NULL;
	
	// Free old array and update
	free(cmd->args);
	cmd->args = new_args;
	cmd->arg_count++;
}
t_cmd_node *create_command_node(char *cmd, char *first_arg)
{
	(void)first_arg;
	if (!cmd)
		return NULL;
	t_cmd_node *node = malloc(sizeof(t_cmd_node));
	if (!node)
		return NULL;
	node->cmd = ft_strdup(cmd);
	if (!node->cmd)
	{
		free(node);
		return NULL;
	}
	// Allocate space for 2 pointers: command + NULL
	node->args = malloc(sizeof(char *) * 2);
	if (!node->args)
	{
		free(node->cmd);
		free(node);
		return NULL;
	}
	node->args[0] = NULL;
	node->args[1] = NULL;
	node->arg_count = 0;  // We have 1 argument (the command)
	node->redir = NULL;
	node->next = NULL;
	return node;
}
t_redir *create_redir(t_token_type type, char *filename)
{
    t_redir *redir = malloc(sizeof(t_redir));
    if (!redir) return NULL;
    redir->type = type;
    redir->filename = ft_strdup(filename);
    redir->next = NULL;
    return redir;
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
	t_node	*node;
	
	if (!left || !right || !op)
		return NULL;
	node = malloc(sizeof(t_node));
	
	if (!node)
		return NULL;
	node->token = op->token;
	node->token_type = op->type;
	node->right = right;
	node->left = left;

	if (op->type == REDIR_OUT || op->type == REDIR_IN || op->type == APPEND || op->type == HEREDOC)
	{
    	if (left->cmd && right->cmd)
        	add_redirection(left->cmd, right->cmd->redir);
    }
	return (node);
}

