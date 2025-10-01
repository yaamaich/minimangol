/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executoin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelaiz <albelaiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:00:24 by yaamaich          #+#    #+#             */
/*   Updated: 2025/10/01 00:57:57 by albelaiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	is_builtin(t_node *cmd)
{
	if (!cmd)
		return (NULL);
	if (!cmd || cmd->token_type != CMD_TOKEN && !cmd->cmd)
		return (0);
	if (ft_strcmp(cmd->cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd, "exit") == 0)
		return (1);
	return (0);
}

void	exec_builtin(t_cmd_node *cmd, t_env *env);
{
	if (!cmd || !cmd->cmd)
		return (NULL);

	if (ft_strcmp(cmd->cmd, "cd") == 0)
		exec_cd(cmd, env);
	else if (ft_strcmp(cmd->cmd, "echo") == 0)
		exec_echo(cmd);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		exec_pwd();
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		exec_export(cmd, env);
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		exec_unset(cmd, env);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		exec_env(env);
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		exec_exit(cmd);
}

void	exec_external(t_cmd_node *cmd, t_env *env)
{
}

void	exec_command(t_cmd_node *cmd, t_env *env)
{
	if (is_builtin(cmd->cmd))
		exec_builtin(cmd, env);
	else
		exec_external(cmd, env);
}

void	execute_ast(t_node *ast, t_env *env)
{
	if (!ast)
		return (NULL);
	if (ast->token_type == CMD_TOKEN)
		exec_command(ast->cmd, env);
}
