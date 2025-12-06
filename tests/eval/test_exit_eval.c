/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exit_eval.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:22:50 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 16:28:43 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "test_eval.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cmocka.h>

/*
 *   setup: exit to single args
 */
int	setup_built_in_exit_single_arg_ast(void **state)
{
	char	*args[] = {"1"};
	*state = create_cmd_ast("exit", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: exit should give error to many arguments
 */
void	test_eval_built_in_exit_single_arg_path(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	char				*expected = "too many arguments\n";
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	assert_string_equal(res->erro_msg, expected);
	assert_int_equal(res->exit_code, 1);
	free(res->erro_msg);
	free(res->curr_dir);
	free(res);
}

/*
 *   setup: exit to many args
 */
int	setup_built_in_exit_to_many_args_ast(void **state)
{
	char	*args[] = {"1", "1"};
	*state = create_cmd_ast("exit", args, 2);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: exit should give error to many arguments
 */
void	test_eval_built_in_exit_to_many_args_path(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	char				*expected = "too many arguments\n";
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	assert_string_equal(res->erro_msg, expected);
	assert_int_equal(res->exit_code, 1);
	free(res->erro_msg);
	free(res->curr_dir);
	free(res);
}
