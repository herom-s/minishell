/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_echo_eval.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:58:33 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/03 16:58:52 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
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
 *   Helper struct to pass test parameters
 */
typedef struct s_test_echo_params
{
	char		*cmd_name;
	char		**args;
	int			arg_count;
	char		*expected_output;
	int			expected_exit_code;
}				t_test_echo_params;

/*
 *   create: creates echo AST from parameters
 */
static t_ast	*create_echo_ast(char *cmd_name, char **args, int arg_count)
{
	t_ast	*simple;
	t_ast	*suffix;
	t_ast	*prev_suffix;
	int		i;

	simple = calloc(1, sizeof(t_ast));
	if (!simple)
		return (NULL);
	simple->type = SIMPLE_CMD;
	simple->s_simple_cmd.cmd_prefix = NULL;
	simple->s_simple_cmd.cmd_name = strdup(cmd_name);
	simple->s_simple_cmd.cmd_suffix = NULL;
	prev_suffix = NULL;
	i = 0;
	while (i < arg_count)
	{
		suffix = calloc(1, sizeof(t_ast));
		if (!suffix)
			return (NULL);
		suffix->type = CMD_SUFFIX;
		suffix->s_cmd_suffix.io_file = NULL;
		suffix->s_cmd_suffix.word = strdup(args[i]);
		suffix->s_cmd_suffix.cmd_suffix = NULL;
		if (prev_suffix)
			prev_suffix->s_cmd_suffix.cmd_suffix = suffix;
		else
			simple->s_simple_cmd.cmd_suffix = suffix;
		prev_suffix = suffix;
		i++;
	}
	return (simple);
}

/*
 *  teardown: free AST with chained suffixes
 */
int	teardown_free_echo_ast(void **state)
{
	t_ast	*ast;
	t_ast	*suf;
	t_ast	*next;

	if (!state || !*state)
		return (0);
	ast = (t_ast *)(*state);
	if (ast->type == SIMPLE_CMD)
	{
		if (ast->s_simple_cmd.cmd_name)
			free(ast->s_simple_cmd.cmd_name);
		suf = ast->s_simple_cmd.cmd_suffix;
		while (suf)
		{
			next = suf->s_cmd_suffix.cmd_suffix;
			if (suf->s_cmd_suffix.word)
				free((void *)suf->s_cmd_suffix.word);
			free(suf);
			suf = next;
		}
	}
	free(ast);
	*state = NULL;
	return (0);
}

/*
 *   setup: echo -n basic
 */
int	setup_built_in_echo_n_basic_ast(void **state)
{
	char	*args[] = {"-n", "hello world"};

	*state = create_echo_ast("echo", args, 2);
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
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	assert_string_equal(res->output, "hello world");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
}

/*
 *   setup: echo basic
 */
int	setup_built_in_echo_basic_ast(void **state)
{
	char	*args[] = {"hello world"};

	*state = create_echo_ast("echo", args, 1);
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
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	assert_string_equal(res->output, "hello world\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
}

/*
 *   setup: echo empty args
 */
int	setup_built_in_echo_empty_ast(void **state)
{
	char	*args[] = {""};

	*state = create_echo_ast("echo", args, 1);
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
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	assert_string_equal(res->output, "\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
}

/*
 *   setup: echo empty args
 */
int	setup_built_in_echo_n_empty_ast(void **state)
{
	char	*args[] = {"-n", ""};

	*state = create_echo_ast("echo", args, 2);
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
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	assert_string_equal(res->output, "");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
}

/*
 *   setup: echo mutiple args
 */
int	setup_built_in_echo_mutiple_args_ast(void **state)
{
	char	*args[] = {"hello", "world", "world"};

	*state = create_echo_ast("echo", args, 3);
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
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	assert_string_equal(res->output, "hello world world\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
}

/*
 *   setup: echo mutiple args
 */
int	setup_built_in_echo_n_mutiple_args_ast(void **state)
{
	char	*args[] = {"-n", "hello", "world", "world"};

	*state = create_echo_ast("echo", args, 4);
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
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	assert_string_equal(res->output, "hello world world");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
}
