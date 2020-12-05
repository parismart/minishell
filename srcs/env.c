/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:42:40 by marvin            #+#    #+#             */
/*   Updated: 2020/12/04 15:43:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**erase_env(char **envp, int i)
{
	int		j;
	int		len;
	char	**cpy;

	len = 0;
	while (envp[len])
		len++;
	if (!(cpy = (char **)malloc(sizeof(char *) * len)))
		return (0);
	j = -1;
	while (++j < i)
		cpy[j] = ft_strdup(envp[j]);
	i++;
	while (envp[i])
		cpy[j++] = ft_strdup(envp[i++]);
	cpy[j] = 0;
	free_env(envp);
	return (cpy);
}

char		**unset_command(t_data *param)
{
	int		i;
	int		len;
	char	*env;
	char	**cpy;

	if (param->argc < 2)
		return (param->envp);
	len = strlen(param->argv[1]);
	env = ft_strjoin(param->argv[1], "=");
	i = 0;
	while (param->envp[i] && ft_memcmp(env, param->envp[i], len + 1))
		i++;
	if (param->envp[i])
		cpy = erase_env(param->envp, i);
	else
		cpy = param->envp;
	free(env);
	return (cpy);
}

char		**export_command(t_data *param)
{
	int		i;
	char	**cpy;

	i = 0;
	while (param->envp[i] &&
		ft_memcmp(param->envp[i], param->argv[1], ft_strlen(param->argv[1])))
		i++;
	if (!param->envp[i])
	{
		cpy = copy_env(param->envp, 1);
		cpy[i] = ft_strjoin(param->argv[1], param->argv[2]);
		free_env(param->envp);
	}
	else
	{
		cpy = param->envp;
		free(param->envp[i]);
		param->envp[i] = ft_strjoin(param->argv[1], param->argv[2]);
	}
	return (cpy);
}

void		env_command(t_data *param, int fd)
{
	int	i;

	i = 0;
	if (param->argc != 1)
	{
		ft_putstr_fd("Wrong number arguments in 'env'!\n", 1);
		return ;
	}
	while (param->envp[i])
	{
		ft_putstr_fd(param->envp[i++], fd);
		write(fd, "\n", 1);
	}
}