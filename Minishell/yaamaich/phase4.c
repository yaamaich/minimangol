/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 04:44:46 by yaamaich          #+#    #+#             */
/*   Updated: 2025/08/28 12:24:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			if (var_name[j] == '\0' && env[i][j] == '=')
				return (&env[i][j + 1]);
		}
		i++;
	}
	return (NULL);
}
char *expand_variables(char *str, t_env *env)
{
    int i = 0;
    int start;
    char *var_name;
    char *var_value;
    char *result = ft_strdup("");

    while (str[i])
    {
        if (str[i] == '$' && i + 1 < ft_strlen(str))
        {
            i++;
            start = i;  
            while (ft_isalphanumiric(str[i]) || str[i] == '_')
                i++;
            var_name = ft_substr(str, start, i - start);
            var_value = get_env_value(&env->value, var_name);

            if (var_value)
                result = ft_strjoin(result, var_value);
            
            free(var_name);
        }
        else
        {
            char tmp[2] = {str[i], '\0'};
            result = ft_strjoin(result, tmp);
            i++;
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
