/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_unset_eval.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 13:07:10 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/15 13:07:11 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "hashtable.h"
#include "tests.h"
#include "test_eval.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

/*
 *   setup: unset no args
 */
int	setup_built_in_unset_no_args_ast(void **state)
{
	*state = create_cmd_ast("unset", NULL, 0);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: unset no args should do nothing
 */
void	test_eval_built_in_unset_no_args(void **state)
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
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: unset single var
 */
int	setup_built_in_unset_single_var_ast(void **state)
{
	char	*args[] = {"FOO"};

	*state = create_cmd_ast("unset", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: unset should remove variable from env
 */
void	test_eval_built_in_unset_single_var(void **state)
{
	t_ast				*ast_export;
	t_ast				*ast_unset;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	char				*export_args[] = {"FOO=bar"};

	ast_unset = (t_ast *)(*state);
	assert_non_null(ast_unset);
	env = create_shell_env(environ);
	assert_non_null(env);
	ast_export = create_cmd_ast("export", export_args, 1);
	res = eval_ast(ast_export, env);
	free(res->output);
	free(res);
	assert_non_null(hashtable_get(env->vars, "FOO"));
	res = eval_ast(ast_unset, env);
	assert_non_null(res);
	assert_string_equal(res->output, "");
	assert_int_equal(res->exit_code, 0);
	assert_null(hashtable_get(env->vars, "FOO"));
	free(res->output);
	free(res);
    free_cmd_ast(ast_export);
	destroy_shell_env(env);
}

/*
 *   setup: unset multiple vars
 */
int	setup_built_in_unset_multiple_vars_ast(void **state)
{
	char	*args[] = {"FOO", "BAR"};

	*state = create_cmd_ast("unset", args, 2);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: unset should remove multiple variables from env
 */
void	test_eval_built_in_unset_multiple_vars(void **state)
{
	t_ast				*ast_export;
	t_ast				*ast_unset;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	char				*export_args[] = {"FOO=bar", "BAR=foo"};

	ast_unset = (t_ast *)(*state);
	assert_non_null(ast_unset);
	env = create_shell_env(environ);
	assert_non_null(env);
	ast_export = create_cmd_ast("export", export_args, 2);
	res = eval_ast(ast_export, env);
	free(res->output);
	free(res);
	assert_non_null(hashtable_get(env->vars, "FOO"));
	assert_non_null(hashtable_get(env->vars, "BAR"));
	res = eval_ast(ast_unset, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_null(hashtable_get(env->vars, "FOO"));
	assert_null(hashtable_get(env->vars, "BAR"));
	free(res->output);
	free(res);
    free_cmd_ast(ast_export);
	destroy_shell_env(env);
}

/*
 *   setup: unset nonexistent var
 */
int	setup_built_in_unset_nonexistent_ast(void **state)
{
	char	*args[] = {"NONEXISTENT_VAR_12345"};

	*state = create_cmd_ast("unset", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: unset nonexistent var should not fail
 */
void	test_eval_built_in_unset_nonexistent(void **state)
{
	t_ast *ast;
	t_shell_response *res;
	t_shell_env *env;
	extern char **environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res);
	destroy_shell_env(env);
}
