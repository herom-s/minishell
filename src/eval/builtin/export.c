/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:42:29 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 16:02:22 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

int	export_args(char **cmd_str, t_shell_env *env, char **output, char **error)
{
	size_t	i;
	int		has_error;
	size_t	num_args;

	i = 0;
	has_error = 0;
	*output = ft_strdup("");
	num_args = num_arguments(cmd_str);
	while (i < num_args)
	{
		if (process_export_arg(cmd_str[i + 1], env, error))
			has_error = 1;
		i++;
	}
	return (has_error);
}

static void	print_and_free(char *out, char *err)
{
	if (out)
		ft_dprintf(STDOUT_FILENO, "%s", out);
	if (err)
		ft_dprintf(STDERR_FILENO, "%s", err);
	free(out);
	free(err);
}

t_cmd_response	*func_built_in_export(t_ast *ast, char **cmd, t_shell_env *env)
{
	char			*out;
	char			*err;
	t_cmd_response	*res;

	(void)ast;
	out = NULL;
	err = NULL;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	if (num_arguments(cmd) == 0)
	{
		out = export_no_args(env);
		res->exit_code = 0;
	}
	else
		res->exit_code = export_args(cmd, env, &out, &err);
	print_and_free(out, err);
	return (res);
}
