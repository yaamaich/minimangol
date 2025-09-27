/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:00:24 by yaamaich          #+#    #+#             */
/*   Updated: 2025/08/28 03:34:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINI_SHELL_H
# define MINI_SHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/src/libft.h"

extern int g_exit_status;

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
	AND_IF,
	OR_IF,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	L_PAREN_TOKEN,
	R_PAREN_TOKEN,
	PIPE,
	CMD
}		t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	char			quete_type;
	int				pipe_read;
	int				pipe_write;
}				t_token;

typedef	struct	s_stack_node
{
	t_token					*token;
	struct s_stack_node		*next;
}				t_stack_node;

typedef struct s_stack
{
	t_stack_node	*top;
} 				t_stack;

typedef struct s_cmd_node {
    char            *cmd;
    char            **args;      	// Changed from char*
    int             arg_count;    	// Added missing field
    struct s_redir  *redir;			// Forward References with Pointers
    struct s_cmd_node *next;
} t_cmd_node;

typedef struct s_queue_node
{
	t_token				*token;
	t_cmd_node			*cmd_token;
	struct s_queue_node	*next;
}				t_queue_node;

typedef struct s_queue
{
	t_queue_node	*head;
	t_queue_node	*tail;
}				t_queue;

typedef struct s_node {
    t_token         *token;
    int             token_type;
    t_cmd_node      *cmd;        // Use correct type
    struct s_node   *left;
    struct s_node   *right;
} 				t_node;

typedef struct s_ast_stack_node {
    t_node *node;
    struct s_ast_stack_node *next;
}				t_ast_stack_node;

typedef struct s_ast_stack{
    t_ast_stack_node *top;
    int				size;
}			t_ast_stack;

typedef struct s_parser
{
	t_stack  *op_stack;
	t_token		*rider;
	t_ast_stack *stack;
	t_queue  *output_queue;
}				t_parser;

typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	struct s_redir	*next;
} 				t_redir;

typedef struct s_op_node {
    t_token		*token;
	char		*op_str;
    int			precedence;
    int			type;
}				t_op_node;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;


//phase1//
t_token *handle_word(t_lexer *lexer);
t_token *get_next_token(t_lexer *lexer);
t_token *handle_operator(t_lexer *lexer);
t_token_type    classify_token(t_token *token);
t_token	*first_token_in_command(t_token *token);
t_token *handle_quotes(t_lexer *lexer, char quote);

//pahse2//
int precedence(t_token_type type);
void finalize_parsing(t_parser *parser);
t_cmd_node *cmd_dequeue(t_queue *queue);
void enqueue(t_queue *queue, t_token *token);
void handl_word(t_parser *parser, t_token *token);
void cmd_enqueue(t_queue *queue, t_cmd_node *token);
void process_token(t_parser *parser, t_token *token);
void handle_operators(t_parser *parser, t_token *op_token);
t_token *create_token(t_token_type type, const char *value);

//phase3//
t_token *dequeue(t_queue *queue);
int is_operator(t_token_type type);
t_node *create_tree_node(t_cmd_node *token);
t_node *build_command_tree(t_parser *parser);
void add_argument(t_cmd_node *cmd, char *arg);
void add_redirection(t_cmd_node *cmd, t_redir *redir);
void handle_redir(t_node *node, t_token_type type);
t_redir *create_redir(t_token_type type, char *filename);
t_cmd_node *create_command_node(char *cmd);  // Fixed typo
t_node *connect_commands(t_node *left, t_node *right, t_op_node *op);
t_node *create_operator_node(t_token *token, t_node *left, t_node *right);

//phase4//
char *expand_variables(char *str, t_env *env);
char *get_env_value(char **env, const char *var_name);
char *expand_exit_status(char *str, int last_exit_status);


//phase5//
int setup_pipes(t_node *cmd_tree);
char *handle_quote(const char *quote);
int delimiter_is_quoted(const char *delimiter);
int handle_redirections(t_cmd_node *cmd, t_env *env);
int create_heredoc(const char *delimiter, t_env *env);

//tools//
int whitespaces(char str);
int is_empty(t_stack *stack);
void report_mesage (char *str);
t_queue *creat_empty_queue(void);
t_lexer *initialize_lexer(char *string);
t_parser *initialize_shunting_yard(void);
t_node *create_operator_node(t_token *token, t_node *left, t_node *right);

//minishell_use//
t_ast_stack *create_ast_stack();
t_stack	*creat_empty_stack (void);
int	size_node_stack(t_stack *stack);
t_node *ast_pop(t_ast_stack *stack);
t_token  *pop_stack(t_stack **stack);
int ast_stack_size(t_ast_stack *stack);
t_stack_node *top_stack(t_stack *stack);
void ast_push(t_ast_stack *stack, t_node *node);
void    push_stack(t_stack **stack, t_token *token);

//execution//
int is_builtin(t_node *cmd);
void execute_ast(t_node *ast, t_env *env);
void exec_builtin(t_cmd_node *cmd, t_env *env);
void exec_command(t_cmd_node *cmd, t_env *env);
void exec_external(t_cmd_node *cmd, t_env *env);
void exec_cd(t_cmd_node *cmd, t_env *env);
void exec_echo(t_cmd_node *cmd);
void exec_pwd();
void exec_export(t_cmd_node *cmd, t_env *env);
void exec_unset(t_cmd_node *cmd, t_env *env);
void exec_env(t_env *env);
void exec_exit(t_cmd_node *cmd);
t_env *init_env(char **envp);

#endif 