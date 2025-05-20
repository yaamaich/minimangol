/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:23:49 by yaamaich          #+#    #+#             */
/*   Updated: 2025/05/20 19:47:12 by yaamaich         ###   ########.fr       */
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

void handle_operators(t_parser *parser)
{
	t_node *top_op;

	
	while (!(is_empty(parser->op_stack)) && precedence(top_stack(parser->stack)->token->type)
		>= precedence(OP_TOKEN) && top_stack(parser->stack)->token->type != L_PAREN_TOKEN)
	{
		top_op = pop_stack(&parser->stack);
		
		enqueue(parser->output_queue, top_op->token);
	}
	push_stack(&parser->stack, top_op);
}
void process_token(t_parser *parser, t_token *token)
{
	t_node *op;

	if (token->type == CMD_TOKEN || token->type == WORD_TOKEN)
		enqueue(parser->output_queue, token);
	else if (token->type == OP_TOKEN)
		handle_operators(parser);
	else if (token->type == L_PAREN_TOKEN)
		push_stack(&parser->stack, parser->stack->node);
	else if (token->type == L_PAREN_TOKEN)
	{
		while (!(is_empty(parser->op_stack)) && top_stack(parser->stack)->token->type)
		{
			op = pop_stack(&parser->stack);
			enqueue(parser->output_queue, op->token);
		}
		if (!is_empty(parser->op_stack))
			pop_stack(&parser->stack);
		else
			report_mesage("Mismatched paentheses");
	}
}

void finalize_parsing(t_parser *parser)
{
	t_node *op;
	
	while (!(is_empty(parser->op_stack)))
	{
		op = pop_stack(&parser->stack);
		if (op->token->type == L_PAREN_TOKEN)
			report_mesage("Mismatched parentheses");
		enqueue(parser->output_queue, op->token);
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
//PHASE 4//

char *get_env_value(char **env, const char *var_name)
{
	int i;
	int j;
	
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] == var_name[j] && var_name[j] != '\0')
		{
			j++;
			if(var_name == '\0' && env[i][j] == '=')
				return (&env[i][j + 1]);
		}
		i++;
	}
	return (NULL);
}

char *expand_variables(char *str, t_env *env)
{
	int i;
	int start;
	char *var_name;
	char *var_value;
	char *result;

	i = 0;
	result = "";
	while(i < ft_strlen(str))
	{
		if(str[i] == '$' && i + 1 < ft_strlen(str))
		{
			i--;
			start = i;
			while (ft_isalphanumiric(str[i]) || str[i] == '-')
			{
				i++;
				var_name = ft_substr(str, start, i - start);
				var_value = get_env_value(&env->value, var_name);
				if (var_value != NULL)
					return (result = ft_strjoin(result, var_value));
				else
				{
					result += str[i];
					i++;
				}
			}
		}
	}
	return result;
}

char *expand_exit_status(char *str, int last_exit_status)
{
	int i;
	char *result;

	i = 0;
	result = "";
	while (i < ft_strlen(str))
	{
		if (str[i] == '$' && i + 1 < ft_strlen(str) && str[i + 1] == '?')
		{
			result = ft_strjoin(result, ft_itoa(last_exit_status));
			i += 2;
		}else
		{
			result += str[i];
			i++;
		}
	}
	return (result);
}
 //PAHSE 5//

int create_heredoc(const char *delimiter)
{
	int fd;
	char *line;
	fd = open(delimiter, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	while (1)
	{
	line = readline("> ");
    if (!line || ft_strcmp(line, delimiter) == 0)
        break;
    write(fd, line, ft_strlen(line));
    write(fd, "\n", 1);
    free(line);
	}
	lseek(fd, 0, SEEK_SET);
	return (fd);
}
int handle_redirections(t_cmd_node *cmd)
{
	t_redir *redir = cmd->redir;
	int fd;

	while (redir)
	{
		if (redir->type == REDIR_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIR_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == HEREDOC)
			fd = create_heredoc(redir->filename);  // أو اسم آخر

		if (fd < 0)
		{
			perror("open");
			return 1;
		}

		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return 0;
}

int setup_pipes(t_node *cmd_tree)
{
    int pipe_fds[2];

    if (cmd_tree == NULL)
        return 1;

    if (cmd_tree->token_type != OP_TOKEN)
        return 1;

    t_token *op = cmd_tree->token;

    if (op && op->type == OP_TOKEN && op->value && op->value[0] == '|')
    {
        if (pipe(pipe_fds) == -1)
        {
            perror("pipe");
            return 0;
        }

        op->pipe_read = pipe_fds[0];
        op->pipe_write = pipe_fds[1];

        if (!setup_pipes(cmd_tree->left))
            return 0;

        if (!setup_pipes(cmd_tree->right))
            return 0;
    }
    else
    {
        if (!setup_pipes(cmd_tree->left))
            return 0;

        if (!setup_pipes(cmd_tree->right))
            return 0;
    }

    return 1;
}

// #include "minishell.h"

// int main(void)
// {
//     // 1. اختبار initialize_lexer
//     char *input = "echo hello world";
//     t_lexer *lexer = initialize_lexer(input);
//     if (!lexer)
//     {
//         report_mesage("Failed to initialize lexer\n");
//         return 1;
//     }
//     report_mesage("Lexer initialized successfully\n");

//     // 2. اختبار creat_empty_stack و is_empty
//     t_stack *stack = creat_empty_stack();
//     if (is_empty(stack))
//         report_mesage("Stack is empty\n");
//     else
//         report_mesage("Stack is not empty\n");

//     // 3. اختبار creat_empty_queue
//     t_queue *queue = creat_empty_queue();
//     if (queue && !queue->head && !queue->tail)
//         report_mesage("Queue initialized empty\n");
//     else
//         report_mesage("Queue initialization failed\n");

//     // 4. اختبار initialize_shunting_yard
//     t_parser *parser = initialize_shunting_yard();
//     if (parser && parser->op_stack && parser->output_queue)
//         report_mesage("Parser initialized successfully\n");
//     else
//         report_mesage("Parser initialization failed\n");

//     // 5. اختبار create_tree_node و create_operator_node
//     t_token dummy_token = {CMD_TOKEN, "echo", 0, 0, 0};
//     t_node *node1 = create_tree_node(&dummy_token);
//     t_node *node2 = create_tree_node(&dummy_token);
//     t_node *op_node = create_operator_node(&dummy_token, node1, node2);

//     if (node1 && node2 && op_node)
//         report_mesage("Tree nodes created successfully\n");
//     else
//         report_mesage("Failed to create tree nodes\n");

//     // تنظيف الذاكرة (اختياري هنا لكن جيد)
//     free(lexer);
//     free(stack);
//     free(queue);
//     free(parser->op_stack);
//     free(parser->output_queue);
//     free(parser);
//     free(node1);
//     free(node2);
//     free(op_node);

//     return 0;
// }
#include "minishell.h"

#include "minishell.h"

int main(void)
{
    char *input;
    t_lexer *lexer;
    t_parser *parser;
    t_token *token;

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

            // تهيئة lexer بالمدخل
            lexer = initialize_lexer(input);

            // تهيئة parser (شنتينج يارد)
            parser = initialize_shunting_yard();

            ft_printf("Tokens:\n");
            // نستخرج التوكنات واحدة واحدة حتى ما نخلص
            while ((token = get_next_token(lexer)) != NULL)
            {
                ft_printf("Type: %d, Value: %s\n", token->type, token->value);
                free(token->value);
                free(token);
            }

            // هنا ممكن نكمل بناء الشجرة مثلاً لاحقاً باستخدام parser

            free(lexer);
            free(parser);
        }
        free(input);
    }
    ft_printf("Goodbye!\n");
    return 0;
}

