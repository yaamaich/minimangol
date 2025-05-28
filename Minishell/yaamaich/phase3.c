/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 04:43:16 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/28 18:49:01 by yaamaich         ###   ########.fr       */
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

t_node *create_operator_node(t_token *token, t_node *left, t_node *right)
{
    t_node *node = malloc(sizeof(t_node));
    node->token = token;
    node->left = left;
    node->right = right;
    return node;
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
// // }
// t_node *build_command_tree(t_parser *parser)
// {
//     t_ast_stack *stack = create_ast_stack();
//     t_token *token;
//     t_node *cmd_node = NULL;
//     t_cmd_node *current_cmd = NULL;

//     while ((token = dequeue(parser->output_queue)))
//     {
//         if (token->type == CMD_TOKEN || token->type == WORD_TOKEN) {
//             if (!current_cmd) {
//                 current_cmd = create_command_node(token->value, NULL);
//                 cmd_node = create_tree_node(token);
//                 cmd_node->cmd = current_cmd;
//             } else {
//                 // Add argument to existing command
//                 add_argument(current_cmd, token->value);
//             }
//         }
//         else if (is_operator(token->type)) {
//             if (!current_cmd) {
//                 // Handle error: operator without command
//                 return NULL;
//             }
//             ast_push(stack, cmd_node);
//             cmd_node = create_operator_node(token, NULL, NULL);
//             current_cmd = NULL;
//         }
//         free(token->value);
//         free(token);
//     }
    
//     // Push the final command node
//     if (cmd_node)
//         ast_push(stack, cmd_node);

//     // Handle operator nodes
//     while (ast_stack_size(stack) > 1) {
//         t_node *right = ast_pop(stack);
//         t_node *left = ast_pop(stack);
//         t_node *op = create_operator_node(stack->top->node->token, NULL, NULL);
//         op->left = left;
//         op->right = right;
//         ast_push(stack, op);
//     }

//     return ast_pop(stack);
// t_node *build_command_tree(t_parser *parser)
// {
//     ft_printf("im here\n");
// 	t_ast_stack		*stack;
// 	t_op_node 		*op_node;
// 	t_node 			*new_node; 
// 	t_token 		*token;
// 	t_node			*node;
// 	t_node			*left;
// 	t_node			*right;
	
// 	stack = malloc (sizeof(t_ast_stack));
//     stack->top = NULL;
//     stack->size = 0;
//     if (!parser || !parser->output_queue)
//         return NULL;
//     if (!stack)
//         return NULL;
// 	while (is_empty(parser->op_stack) && (token = dequeue(parser->output_queue)))
// 	{
//         ft_printf("im here1\n");
// 		token = dequeue(parser->output_queue);
// 		if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
// 		{
//             ft_printf("im here2\n");
// 			node = create_tree_node(token);
//             if (node)
// 			    ast_push(stack, node);
//             ft_printf("im here2,1\n");
// 		}else if (token->type == OP_TOKEN)
// 		{
//             ft_printf("im here3\n");
// 			if (ast_stack_size(stack) < 2)
// 			{
//                 ft_printf("im here4\n");
// 				report_mesage("Invalid expression");
// 				return NULL;
// 			}
//             ft_printf("im here5\n");
// 			right = ast_pop(stack);
// 			left = ast_pop(stack);
// 			// node = create_operator_node(token, left, right);
// 			op_node = create_op_node(token);
//            	new_node = connect_commands(left, right, op_node);
// 			ast_push(stack, node);
// 		}
// 	}
// 	if (ast_stack_size(stack) != 1)
// 	{
//         ft_printf("bad trip\n");
// 		report_mesage("Invalid expression");
// 		return (NULL);
// 	}
// 	return (ast_pop(stack));
// }

void add_argument(t_cmd_node *cmd, char *arg)
{
    // Reallocate args array to add new argument
    int new_size = cmd->arg_count + 2; // +1 for new arg, +1 for NULL terminator
    char **new_args = malloc(sizeof(char *) * new_size);
    
    // Copy existing arguments
    for (int i = 0; i < cmd->arg_count; i++) {
        new_args[i] = cmd->args[i];
    }
    
    // Add new argument
    new_args[cmd->arg_count] = ft_strdup(arg);
    new_args[cmd->arg_count + 1] = NULL;
    
    // Free old array and update
    free(cmd->args);
    cmd->args = new_args;
    cmd->arg_count++;
}
// }
// t_node *build_command_tree(t_parser *parser)
// {
//     t_ast_stack *stack = create_ast_stack();
//     t_token *token;
//     t_node *node;
//     t_node *left;
//     t_node *right;
//     t_cmd_node *current_cmd = NULL;
//     t_node *cmd_node = NULL;
    
//     while ((token = dequeue(parser->output_queue)))
//     {
//         if (token->type == CMD_TOKEN)
//         {
//             // Start a new command
//             if (cmd_node && current_cmd) {
//                 // Push previous command if exists
//                 ast_push(stack, cmd_node);
//             }
            
//             current_cmd = create_command_node(token->value, NULL);
//             cmd_node = create_tree_node(token);
//             cmd_node->cmd = current_cmd;
//             cmd_node->token_type = CMD_TOKEN;
//         }
//         else if (token->type == WORD_TOKEN)
//         {
//             // Add argument to current command
//             if (current_cmd) {
//                 add_argument(current_cmd, token->value);
//             } else {
//                 // Handle error: argument without command
//                 report_mesage("Argument without command");
//                 return NULL;
//             }
//         }
//         else if (is_operator(token->type))
//         {
//             // Push the current command before processing operator
//             if (cmd_node) {
//                 ast_push(stack, cmd_node);
//                 cmd_node = NULL;
//                 current_cmd = NULL;
//             }
            
//             if (ast_stack_size(stack) < 2)
//             {
//                 report_mesage("Invalid expression");
//                 return NULL;
//             }
            
//             right = ast_pop(stack);
//             left = ast_pop(stack);
//             node = create_operator_node(token, left, right);
//             ast_push(stack, node);
//         }
//     }
    
//     // Push the final command if exists
//     if (cmd_node) {
//         ast_push(stack, cmd_node);
//     }
    
//     if (ast_stack_size(stack) != 1)
//     {
//         report_mesage("Invalid expression");
//         return NULL;
//     }
    
//     return ast_pop(stack);
// }

// t_cmd_node	*creat_command_node(char *cmd, char *args)
// {
// 	t_cmd_node *node;

// 	node = malloc(sizeof(t_cmd_node));
// 	node->cmd = ft_strdup(cmd);
// 	node->args = malloc(sizeof(char *) * 2);
//     if (!node->args)
//         return (NULL);
//     node->args[0] = ft_strdup(args);
//     node->args[1] = NULL;

// 	node->redir = NULL;
// 	node->next = NULL;
// 	return (node);
// }
t_cmd_node *create_command_node(char *cmd , char *first_arg)
{
    first_arg = "";
    t_cmd_node *node = malloc(sizeof(t_cmd_node));
    node->cmd = ft_strdup(cmd);
    node->args = malloc(sizeof(char *) * 2);
    node->args[0] = ft_strdup(cmd);  // First arg is always the command itself
    node->args[1] = NULL;
    node->arg_count = 1;
    node->redir = NULL;
    node->next = NULL;
    return node;
}
// t_cmd_node *create_command_node(char *cmd, char *first_arg)
// {
//     t_cmd_node *node = malloc(sizeof(t_cmd_node));
//     node->cmd = ft_strdup(cmd);
//     node->args = malloc(2 * sizeof(char *));
//     node->args[0] = ft_strdup(cmd);
//     node->args[1] = first_arg ? ft_strdup(first_arg) : NULL;
//     node->arg_count = 1;
//     node->redir = NULL;
//     node->next = NULL;
//     return node;
// }

// t_cmd_node *create_command_node(char *cmd, char *first_arg)
// {
//     t_cmd_node *node = malloc(sizeof(t_cmd_node));
//     node->cmd = ft_strdup(cmd);
//     node->args = malloc(2 * sizeof(char *));
//     node->args[0] = ft_strdup(cmd);
//     node->args[1] = first_arg ? ft_strdup(first_arg) : NULL;
//     node->arg_count = 1;
//     node->redir = NULL;
//     node->next = NULL;
//     return node;
// }
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