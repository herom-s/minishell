/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pipe_eval.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:39:11 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/23 16:39:13 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "test_eval.h"
#include <fcntl.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cmocka.h>

/*
 *   setup: simple two-stage pipeline using absolute echo to force external
 *          (/bin/echo hello | wc -l)
 */
int	setup_pipe_two_stage(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("/bin/echo", (char *[]){"hello", NULL}, 1);
	cmds[1] = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

/*
 *   test: basic eval of two-stage pipeline expecting "1\n"
 */
void	test_eval_pipe_two_stage(void **state)
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
	assert_string_equal(res->output, "1\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: file-based pipeline (cat /tmp/minishell_pipe_test.txt | wc -l)
 *          writes a 3-line file in setup
 */
int	setup_pipe_file_cat_wc(void **state)
{
	const char	*path = "/tmp/minishell_pipe_test.txt";
	int			fd;
	const char	*content = "line1\nline2\nline3\n";
	t_ast		*cmds[2];

	fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	if (write(fd, content, strlen(content)) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	cmds[0] = create_cmd_ast("cat", (char *[]){(char *)path, NULL}, 1);
	cmds[1] = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

/*
 *   test: file-based pipeline returns the number of lines (3)
 */
void	test_eval_pipe_file_cat_wc(void **state)
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
	assert_string_equal(res->output, "3\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/minishell_pipe_test.txt");
}

/*
 *   setup: longer chain (/bin/echo hello | cat | wc -l | cat)
 */
int	setup_pipe_long_chain(void **state)
{
	t_ast	*cmds[4];

	cmds[0] = create_cmd_ast("/bin/echo", (char *[]){"hello", NULL}, 1);
	cmds[1] = create_cmd_ast("cat", NULL, 0);
	cmds[2] = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	cmds[3] = create_cmd_ast("cat", NULL, 0);
	if (!cmds[0] || !cmds[1] || !cmds[2] || !cmds[3])
		return (-1);
	*state = create_pipeline_ast(cmds, 4);
	if (*state == NULL)
		return (-1);
	return (0);
}

/*
 *   test: longer chain should produce "1\n"
 */
void	test_eval_pipe_long_chain(void **state)
{
	t_ast *ast;
	t_shell_response *res;
	t_shell_env *env;
	extern char **environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env, environ);
	assert_non_null(res);
	assert_string_equal(res->output, "1\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}
