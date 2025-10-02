/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_exectution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelaiz <albelaiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:30:12 by albelaiz          #+#    #+#             */
/*   Updated: 2025/10/02 11:01:40 by albelaiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*set_str_env(char *key, char *value)
{
	char	*res;

	res = malloc(ft_strline(key) + ft_strline(value) + 1);
	if (!res)
		return (NULL);
	strcpy(res, key);
	strcat(res, "=");
	strcat(res, value);
	return (res);
}

void	set_env(char *key, char *value, char **env)
{
	char	*new_var;
	char	**new_env;

	int i, j, len_key;
	i = 0;
	j = 0;
	len_key = ft_strlen(key);
	new_var = malloc(len_key + ft_strlen(value) + 2);
	new_env = malloc(sizeof(char *) + (i + 2));
	if (!new_var)
		return (NULL);
	new_var = set_str_env(key, value);
	while (env[i])
	{
		if (ft_strncmp(key, value, ft_strlen(key)) == 0
			&& env[i][len_key] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return (env);
		}
		i++;
	}
	while (j < i)
	{
		new_env[j] = env[j];
		new_env[i] = new_var;
		new_env[i + 1] = NULL;
	}
	return (new_env);
}

void	print_env_as_export(t_env *env)
{
	while (env)
	{
		printf("declare -x %s", env->key);
		if (env->value)
		{
			printf("=\"%s\"", env->value);
			printf("\n");
			env = env->next;
		}
	}
}

void	remove_env(t_env **env, char *name)
{
	t_env	*tmp;
	t_env	*prev;

	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, name) == 0)
		{
			if (prev == NULL)
				*env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
