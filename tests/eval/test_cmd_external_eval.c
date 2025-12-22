/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cmd_eval.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 15:54:08 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/19 17:02:36 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "test_eval.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cmocka.h>

/*
 *   setup: setup cat external command
 */
int	setup_external_cat_abs_ast(void **state)
{
	char		*args[] = {"/tmp/minishell_test_cat.txt", NULL};
	int			fd;
	const char	*txt = "hello world\n";

	fd = open(args[0], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	if (write(fd, txt, strlen(txt)) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	*state = create_cmd_ast("cat", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: basic eval test of cat command printing a hello world
 */
void	test_eval_external_cat_abs(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env, environ);
	assert_non_null(res);
	assert_string_equal(res->output, "hello world\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/minishell_test_cat.txt");
}
