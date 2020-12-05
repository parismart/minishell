/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 16:39:19 by marvin            #+#    #+#             */
/*   Updated: 2020/12/05 09:38:52 by parmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_dir(char *path, t_data *param)
{
	char		buff[4097];
	char		*oldpwd;
	struct stat	s;

	oldpwd = getcwd(buff, 4096);
	if (chdir(path) == 0)
	{
		param->argc = 4;
		free_env(param->argv);
		param->argv = (char **)ft_calloc(sizeof(char *), 4);
		param->argv[0] = ft_strdup("export");
		param->argv[1] = ft_strdup("OLDPWD=");
		param->argv[2] = ft_strdup(oldpwd);
		param->envp = export_command(param);
		free_env(param->argv);
		param->argv = (char **)ft_calloc(sizeof(char *), 4);
		param->argv[0] = ft_strdup("export");
		param->argv[1] = ft_strdup("PWD=");
		param->argv[2] = ft_strdup(getcwd(buff, 4096));
		param->envp = export_command(param);
	}
	else if (ft_putstrs_fd("-bash: cd: ", 0, 0, 1) && lstat(path, &s) != -1)
		ft_putstrs_fd(0, param->argv[1], ": ", 1);
}

void		cd_command(t_data *param)
{
	char *path;

	if (param->argc <= 2)
	{
		if (!param->argv[1] || !ft_strncmp(param->argv[1], "--", 3) ||
			!ft_strncmp(param->argv[1], "~", 2))
			path = get_env(param->envp, "HOME");
		else if (!ft_strncmp(param->argv[1], "-", 2))
			path = get_env(param->envp, "OLDPWD");
		else
			path = param->argv[1];
		change_dir(path, param);
		if (errno > 0)
			ft_putstrs_fd(strerror(errno), "\n", 0, 1);
		errno = 0;
	}
	else
		ft_putstr_fd("-bash: cd: too many arguments\n", 1);
}