/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:23:49 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/26 06:22:06 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *initialize_env(void)
{
    t_env *env = malloc(sizeof(t_env));
    // Initialize with default environment variables
    return env;
}
void expand_tree(t_node *node, t_env *env)
{
    if (!node) return;
    
    if (node->token->type == WORD_TOKEN || node->token->type == CMD_TOKEN) {
        // Handle variable expansion
        char *expanded = expand_variables(node->token->value, env);
        free(node->token->value);
        node->token->value = expanded;
    }
    
    expand_tree(node->left, env);
    expand_tree(node->right, env);
}
void execute_tree(t_node *node)
{
    if (!node) return;
    
    if (node->token->type == OP_TOKEN) {
        // Handle pipes and redirections
        if (ft_strcmp(node->token->value, "|") == 0) {
            execute_pipe(node->left, node->right);
        }
    } else {
        // Execute simple command
        execute_command(node);
    }
}

void free_parser(t_parser *parser)
{
    // Free output queue
    t_queue_node *current = parser->output_queue->head;
    while (current) {
        t_queue_node *next = current->next;
        free(current->token->value);
        free(current->token);
        free(current);
        current = next;
    }
    free(parser->output_queue);
    
    // Free operator stack
    while (!is_empty(parser->op_stack)) {
        t_node *node = pop_stack(&parser->op_stack);
        free(node->token->value);
        free(node->token);
        free(node);
    }
    free(parser);
}
int main(void)
{
    char *input;
    t_lexer *lexer;
    t_parser *parser;
    t_token *token;
    t_node *cmd_tree; 
    t_env *env = initialize_env(); // You'll need to implement this

    while (1)
    {
        input = readline("minishell> ");
        if (!input) // Ctrl+D
            break;
        if (ft_strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
        if (input[0] != '\0')
        {
            add_history(input);

            /* PHASE 1: Lexing */
            lexer = initialize_lexer(input);
            printf("\n=== PHASE 1: Lexing ===\n");
            printf("Tokens:\n");
            while ((token = get_next_token(lexer)) != NULL)
            {
                printf("Type: %d, Value: %s\n", token->type, token->value);
                free(token->value);
                free(token);
            }

            /* Reset lexer for parsing */
            free(lexer);
            lexer = initialize_lexer(input);

            /* PHASE 2: Parsing */
            printf("\n=== PHASE 2: Parsing ===\n");
            parser = initialize_shunting_yard();
            while ((token = get_next_token(lexer)) != NULL)
            {
                process_token(parser, token);
            }
            finalize_parsing(parser);
            
            /* PHASE 3: Command Tree */
            printf("\n=== PHASE 3: Command Tree ===\n");
            cmd_tree = build_command_tree(parser);
            if (cmd_tree) {
                print_tree(cmd_tree, 0);
                
                /* PHASE 4: Expansion */
                printf("\n=== PHASE 4: Expansion ===\n");
                expand_tree(cmd_tree, env); // You'll need to implement this
                
                /* PHASE 5: Execution */
                printf("\n=== PHASE 5: Execution ===\n");
                execute_tree(cmd_tree); // You'll need to implement this
                
                free_tree(cmd_tree);
            } else {
                printf("Error building command tree\n");
            }

            /* Cleanup */
            free_parser(parser);
            free(lexer);
        }
        free(input);
    }
    printf("Goodbye!\n");
    return 0;
}

