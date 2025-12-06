/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pwd_eval.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 12:13:46 by hermarti         ###   ########.fr       */
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
 *   setup: pwd basic
 */
int	setup_built_in_pwd_basic_ast(void **state)
{
	char	*args[] = {0};

	*state = create_cmd_ast("pwd", args, 0);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: pwd should print the current dir
 */
void	test_eval_built_in_pwd_basic(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	char				*buffer;
	char				*curr_dir;
	char				*expected;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	buffer = calloc(1, PATH_MAX + 1);
	if (!buffer)
		return ;
	curr_dir = getcwd(buffer, PATH_MAX);
	expected = calloc(1, strlen(curr_dir) + 2);
	if (!expected)
	{
		free(buffer);
		return ;
	}
	strcpy(expected, curr_dir);
	strcat(expected, "\n");
	assert_string_equal(res->output, expected);
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	free(buffer);
	free(expected);
}
