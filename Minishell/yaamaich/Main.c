/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:23:49 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/29 16:16:48 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

// int main(void) {
//     char *input;
//     t_lexer *lexer;
//     t_parser *parser;
//     t_token *token;
//     t_node *ast;

//     while (1) {
//         input = readline("minishell> ");
//         if (!input) break;
//         if (*input) add_history(input);

//         // Initialize lexer and tokenization
//         lexer = initialize_lexer(input);
//         parser = initialize_shunting_yard();

//         // Tokenization phase
//         printf("\n--- Tokenization ---\n");
//         while ((token = get_next_token(lexer))) {
//             printf("Token: type=%d, value='%s'\n", token->type, token->value);
//             process_token(parser, token);
//         }

//         // Shunting Yard completion
//         finalize_parsing(parser);

//         // AST construction
//         printf("\n--- AST Construction ---\n");
//         ast = build_command_tree(parser);
//         if (ast) printf("AST built successfully\n");

//         // Cleanup
//         free(input);
//         free(lexer);
        
//         // Reset for next input
//         printf("\n--------------------------------\n");
//     }
//     return 0;
// }

void print_ast(t_node *node, int depth)
{
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    printf("Node: type=%d, value='%s'\n", node->token->type, node->token->value);
    
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

t_queue *combine_command_tokens(t_queue *tokens)
{
	t_queue *combined = creat_empty_queue();
	t_token *token;
	char *command_str = NULL;

	while ((token = dequeue(tokens)))
	{
		if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
		{
			if (!command_str)
				command_str = ft_strdup(token->value);
			else
			{
				char *tmp = command_str;
				command_str = ft_strjoin(tmp, " ");
				free(tmp);
				tmp = command_str;
				command_str = ft_strjoin(tmp, token->value);
				free(tmp);
			}
		}
		else
		{
			if (command_str)
			{
				t_token *cmd_token = create_token(CMD_TOKEN, command_str);
				enqueue(combined, cmd_token);
				free(command_str);
				command_str = NULL;
			}
			enqueue(combined, token);
		}
	}

	if (command_str)
	{
		t_token *cmd_token = create_token(CMD_TOKEN, command_str);
		enqueue(combined, cmd_token);
		free(command_str);
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

        // Initialize lexer and parser
        lexer = initialize_lexer(input);
        parser = initialize_shunting_yard();

        // Tokenization phase
        printf("\n--- Tokenization ---\n");
        t_queue *raw_tokens = creat_empty_queue();
        while ((token = get_next_token(lexer))) {
            printf("Token: type=%d, value='%s'\n", token->type, token->value);
            enqueue(raw_tokens, token);
        }

        t_queue *combined_tokens = combine_command_tokens(raw_tokens);
        
        // Post-token-combination
        t_token *combined_token;
        while ((combined_token = dequeue(combined_tokens)))
            process_token(parser, combined_token);
            
        // Shunting Yard completion
        finalize_parsing(parser);

        // Print postfix expression for debugging
        printf("\n--- Postfix Expression ---\n");
        t_queue_node *current = parser->output_queue->head;
        while (current) {
            printf("%s ", current->token->value);
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

        // Cleanup
        free(input);
        free(lexer);
        
        // Reset for next input
        printf("\n--------------------------------\n");
    }
    return 0;
}