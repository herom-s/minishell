/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_echo_eval.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:58:33 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 12:13:15 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "test_eval.h"
#include <fcntl.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cmocka.h>

/*
 *   setup: echo -n basic
 */
int	setup_built_in_echo_n_basic_ast(void **state)
{
	char	*args[] = {"-n", "hello world"};

	*state = create_cmd_ast("echo", args, 2);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: echo -n should not print newline
 */
void	test_eval_built_in_echo_n_basic(void **state)
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
	assert_string_equal(res->output, "hello world");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
    destroy_shell_env(env);
}

/*
 *   setup: echo basic
 */
int	setup_built_in_echo_basic_ast(void **state)
{
	char	*args[] = {"hello world"};

	*state = create_cmd_ast("echo", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: basic eval test of echo command printing a hello world
 */
void	test_eval_built_in_echo_basic(void **state)
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
}

/*
 *   setup: echo empty args
 */
int	setup_built_in_echo_empty_ast(void **state)
{
	char	*args[] = {""};

	*state = create_cmd_ast("echo", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: empty eval test of echo command printing nothing
 */
void	test_eval_built_in_echo_empty(void **state)
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
	assert_string_equal(res->output, "\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
    destroy_shell_env(env);
}

/*
 *   setup: echo empty args
 */
int	setup_built_in_echo_n_empty_ast(void **state)
{
	char	*args[] = {"-n", ""};

	*state = create_cmd_ast("echo", args, 2);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: empty eval test of echo command printing nothing
 */
void	test_eval_built_in_echo_n_empty(void **state)
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
	assert_string_equal(res->output, "");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
    destroy_shell_env(env);
}

/*
 *   setup: echo mutiple args
 */
int	setup_built_in_echo_mutiple_args_ast(void **state)
{
	char	*args[] = {"hello", "world", "world"};

	*state = create_cmd_ast("echo", args, 3);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: mutiple args eval test of echo command printing hello world world\n
 */
void	test_eval_built_in_echo_mutiple_args(void **state)
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
	assert_string_equal(res->output, "hello world world\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
    destroy_shell_env(env);
}

/*
 *   setup: echo mutiple args
 */
int	setup_built_in_echo_n_mutiple_args_ast(void **state)
{
	char	*args[] = {"-n", "hello", "world", "world"};

	*state = create_cmd_ast("echo", args, 4);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: mutiple args eval test of echo command printing hello world world\n
 */
void	test_eval_built_in_echo_n_mutiple_args(void **state)
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
	assert_string_equal(res->output, "hello world world");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
    destroy_shell_env(env);
}
