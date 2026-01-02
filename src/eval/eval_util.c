/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:17:45 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:05:36 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "eval.h"
#include <unistd.h>

void	read_stream(int fd, char **str)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		tmp = ft_strjoin(*str, line);
		free(*str);
		free(line);
		if (!tmp)
			break ;
		*str = tmp;
	}
	close(fd);
}

void	create_pipes_or_fail(int *fd_out, int *fd_err)
{
	if (pipe(fd_out) == -1)
		exit(EXIT_FAILURE);
	if (pipe(fd_err) == -1)
	{
		close(fd_out[INPUT_END]);
		close(fd_out[OUTPUT_END]);
		exit(EXIT_FAILURE);
	}
}

void	*closes_pipes(int *fd_out, int *fd_err)
{
	close(fd_out[INPUT_END]);
	close(fd_out[OUTPUT_END]);
	close(fd_err[INPUT_END]);
	close(fd_err[OUTPUT_END]);
	return (NULL);
}

t_cmd_func_call	*check_cmd(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_func_call	*call;

	call = ft_calloc(1, sizeof(t_cmd_func_call));
	if (!call)
		return (NULL);
	call->env = env;
	call->envp = env->envp;
	call->cmd_str = get_cmd_str(shell_ast->u_ast.s_simple_cmd.cmd_name,
			shell_ast->u_ast.s_simple_cmd.cmd_suffix, env->envp);
	call->cmd_func = get_cmd_func(shell_ast->u_ast.s_simple_cmd.cmd_name);
	call->is_builtin = check_builtin(shell_ast->u_ast.s_simple_cmd.cmd_name);
	return (call);
}
