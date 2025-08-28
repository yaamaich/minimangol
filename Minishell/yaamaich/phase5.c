/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 04:46:44 by yaamaich          #+#    #+#             */
/*   Updated: 2025/08/28 17:05:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//PAHSE 5//
// int create_heredoc(const char *delimiter)
// {
// 	int fd;
// 	char *line;
// 	fd = open(delimiter, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	while (1)
// 	{
// 	line = readline("> ");
//     if (!line || ft_strcmp(line, delimiter) == 0)
//         break;
//     write(fd, line, ft_strlen(line));
//     write(fd, "\n", 1);
//     free(line);
// 	}
// 	lseek(fd, 0, SEEK_SET);
// 	return (fd);
// }
int	create_heredoc(const char *delimiter)
{
	int		fd;
	char	*line;

	fd = open( "/tmp/.heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("open"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (perror("open"), -1);
	unlink(filename);
	return (fd);
}


int handle_redirections(t_cmd_node *cmd)
{
	int 	fd;
	t_redir *redir;
	
	
	redir = cmd->redir;
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