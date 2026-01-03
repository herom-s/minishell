/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env_eval.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 11:38:38 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/11 17:19:59 by hermarti         ###   ########.fr       */
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
 *   helper: check if env var exists in output
 */
static int	env_var_in_output(const char *output, const char *var)
{
	char	*search;
	char	*found;
	int		result;

	search = malloc(strlen(var) + 2);
	if (!search)
		return (0);
	strcpy(search, var);
	strcat(search, "\n");
	found = strstr(output, search);
	result = (found != NULL);
	free(search);
	return (result);
}

/*
 *   setup: env no args
 */
int	setup_built_in_env_no_args_ast(void **state)
{
	*state = create_cmd_ast("env", NULL, 0);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: env no args should contain all env vars
 */
void	test_eval_built_in_env_no_args(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	size_t				i;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_non_null(res->output);
	i = 0;
	while (environ[i])
	{
		assert_true(env_var_in_output(res->output, environ[i]));
		i++;
	}
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res);
    destroy_shell_env(env);
}

/*
 *   test: env output should not be empty
 */
void	test_eval_built_in_env_not_empty(void **state)
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
	assert_non_null(res->output);
	assert_true(strlen(res->output) > 0);
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res);
    destroy_shell_env(env);
}

/*
 *   test: env output should contain PATH variable
 */
void	test_eval_built_in_env_contains_path(void **state)
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
	assert_non_null(res->output);
	assert_non_null(strstr(res->output, "PATH="));
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res);
    destroy_shell_env(env);
}
