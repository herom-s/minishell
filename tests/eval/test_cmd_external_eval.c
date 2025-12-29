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
#include <sys/stat.h>
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
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "hello world\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/minishell_test_cat.txt");
}

/*
 *   setup: external echo using absolute path
 */
int	setup_external_echo_abs_ast(void **state)
{
	char	*args[] = {"hello", "world", NULL};

	*state = create_cmd_ast("/bin/echo", args, 2);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_external_echo_abs(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "hello world\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: external true (absolute path) - should exit 0, no output
 */
int	setup_external_true_abs_ast(void **state)
{
	*state = create_cmd_ast("/bin/true", NULL, 0);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_external_true_abs(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: external false (absolute path) - should exit non-zero, no output
 */
int	setup_external_false_abs_ast(void **state)
{
	*state = create_cmd_ast("/bin/false", NULL, 0);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_external_false_abs(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "");
	assert_true(res->exit_code != 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: ls on a temporary dir using absolute path
 */
int	setup_external_ls_abs_ast(void **state)
{
	const char	*dir = "/tmp/minishell_test_ls_dir";
	char		*args[] = {(char *)dir, NULL};
	char		path[PATH_MAX];
	int			fd;

	if (mkdir(dir, 0700) < 0 && access(dir, F_OK) != 0)
		return (-1);
	snprintf(path, sizeof(path), "%s/%s", dir, "file.txt");
	fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	close(fd);
	*state = create_cmd_ast("/bin/ls", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_external_ls_abs(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "file.txt\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	rmdir("/tmp/minishell_test_ls_dir");
}

/*
 *   setup: setup invalid external command
 */
int	setup_external_invalid_abs_ast(void **state)
{
	*state = create_cmd_ast("gat", NULL, 0);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: basic eval test of invalid command printing a error about
 *         command not found
 */
void	test_eval_external_invalid_abs(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->erro_msg, "gat: command not found\n");
	assert_int_equal(res->exit_code, 127);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}
