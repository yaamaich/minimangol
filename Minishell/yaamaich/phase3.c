/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 04:43:16 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/24 04:43:24 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

include "minishell.h"

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

// t_node*create_tree_node(t_token *token)
// {
// 	t_node *node;
	
// 	node = malloc(sizeof(t_node));
// 	node->token = token;
// 	node->left = NULL;
// 	node->right = NULL;
// 	return (node);
// }

// t_node *create_operator_node(t_token *token, t_node *left, t_node *right)
// {
// 	t_node *new_node;

// 	new_node = malloc(sizeof(t_node));
// 	new_node->token = token;
// 	new_node->left = left;
// 	new_node->right = right;
// 	return (new_node);	
// }

t_node *build_command_tree(t_parser *parser)
{
	t_node_stack	*stack;
	t_token 		*token;
	t_node			*node;
	t_node			*left;
	t_node			*right;
	
	stack = malloc (sizeof(t_node_stack));
	stack->next = NULL;
	while (!(is_empty(parser->op_stack)))
	{
		token = dequeue(parser->output_queue);
		if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
		{
			node = create_tree_node(token);
			push_stack(&stack, node);
		}else if (token->type == OP_TOKEN)
		{
			if (size_node_stack(stack) < 2)
			{
				report_mesage("Invalid expression");
				return NULL;
			}

			right = pop_stack(&stack);
			left = pop_stack(&stack);
			node = create_operator_node(token, left, right);
			push_stack(&stack, node);
		}
	}
	if (size_node_stack(stack) != 1)
	{
		report_mesage("Invalid expression");
		return (NULL);
	}
	return (pop_stack(&stack));
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
	t_node	*node;

	node = malloc(sizeof(t_node));
	node->token = op->token;
	node->token_type = OP_TOKEN;
	node->right = right;
	node->left = left;
	return (node);
}