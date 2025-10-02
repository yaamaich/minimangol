/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelaiz <albelaiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:00:24 by yaamaich          #+#    #+#             */
/*   Updated: 2025/10/02 11:00:51 by albelaiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_echo(t_cmd_node *cmd)
{
	int i, n_found, j;
	i = 1;
	n_found = 0;
	while (cmd->args && cmd->args[i] && cmd->args[i][0] == '-'
		&& cmd->args[i][1] == 'n')
	{
		j = 0;
		while (cmd->args[i][j] == 'n')
			j++;
		while (cmd->args[i][j] != '\0')
			break ;
		n_found = 1;
		i++;
	}
	while (cmd->args && cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (!n_found)
		printf("\n");
}

void	exec_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}

void	exec_cd(t_cmd_node *cmd, t_env *env)
{
	char	path[PATH_MAX];
	char	*oldpwd;

	if (!cmd->args[1])
		oldpwd = get_env_value("HOME", env);
	else if (ft_strcmp(cmd->args[1], "-") == 0)
		oldpwd = get_env_value("OLDPWD", env);
	else
		oldpwd = cmd->args[1];
	if (chdir(oldpwd) == -1)
	{
		perror("cd");
		return ;
	}
	set_env("OLDPWD", get_env_value("PWD", env), env);
	if (getcwd(path, sizeof(path)) != NULL)
		set_env("PWD", path, env);
}

void	exec_export(t_cmd_node *cmd, t_env *env)
{
	int		i;
	char	*arg;
	char	*equl;
	char	*name;
	char	*value;

	i = 1;
	if (!cmd->args[1])
		print_env_as_export(*env);
	return ;
	while (cmd->args[i])
	{
		arg = cmd->args[i];
		equl = ft_strchr(arg, '=');
		if (equl)
		{
			name = ft_substr(arg, 0, equl - arg);
			value = ft_strdup(equl + 1);
			set_env(name, value, env);
		}
		else
			set_env(arg, NULL, env);
		i++;
	}
}

void	exec_unset(t_cmd_node *cmd, t_env *env)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		remove_env(env, cmd->args[i]);
	}
}

void	exec_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value == NULL)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}
int	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(ft_atoi(str[i])))
			return (0);
		i++;
	}
	return (1);
}

void	exec_exit(t_cmd_node *cmd)
{
	int	status;

	printf("exit\n");
	if (!cmd->args[1])
		exit(0);
	if (!is_number(cmd->args[1]))
		printf("minishell: exit: %s: numeric argument required\n",
			cmd->args[1]);
	exit(255);
	if (cmd->args[2])
	{
		printf("minishell: exit: too many arguments\n");
		return ;
	}
	exit(ft_atoi(cmd->args[1]) % 256);
}
