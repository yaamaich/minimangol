/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:23:49 by yaamaich          #+#    #+#             */
/*   Updated: 2025/08/28 04:44:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int g_exit_status = 1;
void free_command_node(t_cmd_node *cmd)
{
	if (!cmd)
		return;
		
	// Free command string
	if (cmd->cmd)
		free(cmd->cmd);
		
	// Free arguments array
	if (cmd->args)
	{
		int i = 0;
		while (i < cmd->arg_count && cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	
	// Free the node itself
	free(cmd);
}
void print_ast(t_node *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    if (node->cmd && node->cmd->cmd) {
        printf("CMD: %s", node->cmd->cmd);
        for (int i = 1; i < node->cmd->arg_count; i++) {
            printf(" ARG: %s", node->cmd->args[i]);
        }
        printf("\n");
    } else if (node->token && node->token->value) {
        printf("OP: %s\n", node->token->value);
    } else {
        printf("UNKNOWN NODE\n");
    }
    if (node->left) {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("├─ Left:\n");
        print_ast(node->left, depth + 1);
    }
    if (node->right) {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("└─ Right:\n");
        print_ast(node->right, depth + 1);
    }
}

t_queue *combine_command_tokens_cmd_only(t_queue *tokens)
{
	t_queue *combined = creat_empty_queue();
	t_token *token;
	t_cmd_node *command_str = NULL;

	// If token is a command or word (argument)
	while ((token = dequeue(tokens))) {
	    if (token->type == CMD_TOKEN) {
	        if (command_str) {
	            cmd_enqueue(combined, command_str);
	            command_str = NULL;
	        }
	        command_str = create_command_node(token->value, NULL);
	    } else if (token->type == WORD_TOKEN) {
	        if (command_str)
	            add_argument(command_str, token->value);
	        // else: error, argument without command
	    } else if (token->type == PIPE || token->type == REDIR_OUT || token->type == APPEND) {
	        if (command_str) {
	            cmd_enqueue(combined, command_str);
	            command_str = NULL;
	        }
	        enqueue(combined, token);
	    } else {
	        if (command_str) {
	            cmd_enqueue(combined, command_str);
	            command_str = NULL;
	        }
	        enqueue(combined, token);
	    }
	}
	if (command_str) {
	    cmd_enqueue(combined, command_str);
	}
	return combined;
}

// Updated main function with better debugging
int main(void) {
    char *input;
    t_lexer *lexer;
    t_parser *parser;
    t_token *token;
    t_node *ast;

    while (1) {
        input = readline("minishell> ");
        if (!input) break;
        if (*input) add_history(input);

        lexer = initialize_lexer(input);
        parser = initialize_shunting_yard();

        printf("\n--- Tokenization ---\n");
        t_queue *raw_tokens = creat_empty_queue();
        while ((token = get_next_token(lexer))) {
            printf("Token: type=%d, value='%s'\n", token->type, token->value);
            enqueue(raw_tokens, token);
        }
		g_exit_status = 1;
        t_token *combined_token;
        while ((combined_token = dequeue(raw_tokens)))
        {
            printf("TYPE: %d, VALUE: %s\n", combined_token->type, combined_token->value);
            process_token(parser, combined_token);
        }

        finalize_parsing(parser);

        t_queue *combined_tokens = combine_command_tokens_cmd_only(parser->output_queue);
        
		parser->output_queue = combined_tokens;
        printf("\n--- Postfix Expression ---\n");
		t_queue_node *current = parser->output_queue->head;
		while (current) {
		    if (current->cmd_token) {
		        printf("%s ", current->cmd_token->cmd); // Print command
		        for (int i = 1; i < current->cmd_token->arg_count; i++) {
		            printf("%s ", current->cmd_token->args[i]); // Print arguments
		        }
		    }
		    if (current->token) {
		        printf("%s ", current->token->value); // Print operator
		    }
		    current = current->next;
		}
        printf("\n");

        // AST construction
        printf("\n--- AST Construction ---\n");
		 
        ast = build_command_tree(parser);
        if (ast) {
            printf("AST built successfully\n");
            print_ast(ast, 0);
        }

        free(input);
        free(lexer);
        printf("\n--------------------------------\n");
    }
    return 0;
}