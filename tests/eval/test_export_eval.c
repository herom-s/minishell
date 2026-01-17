/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_export_eval.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:18:40 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/11 17:23:41 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "tests.h"
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
 *   helper: check if export var exists in output
 *   export format: "declare -x KEY=VALUE\n"
 */
static int	export_var_in_output(const char *output, const char *var)
{
	char	*search;
	char	*found;
	char	*equal_pos;
	int		result;
	size_t	len;

	len = strlen("declare -x ") + strlen(var) + 4;
	search = malloc(len);
	if (!search)
		return (0);
	strcpy(search, "declare -x ");
	equal_pos = strchr(var, '=');
	if (equal_pos)
	{
		strncat(search, var, equal_pos - var + 1);
		strcat(search, "\"");
		strcat(search, equal_pos + 1);
		strcat(search, "\"\n");
	}
	found = strstr(output, search);
	result = (found != NULL);
	free(search);
	return (result);
}

/*
 *   setup: export no args
 */
int	setup_built_in_export_no_args_ast(void **state)
{
	*state = create_cmd_ast("export", NULL, 0);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: export no args should contain all export vars
 */
void	test_eval_built_in_export_no_args(void **state)
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
	char *captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_non_null(captured);
	i = 0;
	while (environ[i])
	{
		assert_true(export_var_in_output(captured, environ[i]));
		i++;
	}
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: export with arg
 */
int	setup_built_in_export_arg_ast(void **state)
{
	char	*args[] = {"FOO=bar"};

	*state = create_cmd_ast("export", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: export arg should add variable to the env
 */
void	test_eval_built_in_export_arg(void **state)
{
	t_ast				*ast_export;
	t_ast				*ast_list;
	t_shell_response	*res;
	t_shell_response	*res2;
	t_shell_env			*env;
	extern char			**environ;

	ast_export = (t_ast *)(*state);
	assert_non_null(ast_export);
	env = create_shell_env(environ);
	assert_non_null(env);
	char *captured = eval_and_capture(ast_export, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	ast_list = create_cmd_ast("export", NULL, 0);
	char *captured2 = eval_and_capture(ast_list, env, &res2);
	assert_non_null(res2);
	assert_non_null(captured2);
	assert_non_null(strstr(captured2, "declare -x FOO=\"bar\""));
	assert_int_equal(res2->exit_code, 0);
	free(captured2);
	free(res2);
	free_cmd_ast(ast_list);
	destroy_shell_env(env);
}

/*
 *   setup: export with mutiple args
 */
int	setup_built_in_export_mutiple_args_ast(void **state)
{
	char	*args[] = {"FOO=bar", "BAR=foo"};

	*state = create_cmd_ast("export", args, 2);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: export mutiple args should add variables to the env
 */
void	test_eval_built_in_export_mutiple_args(void **state)
{
	t_ast				*ast_export;
	t_ast				*ast_list;
	t_shell_response	*res;
	t_shell_response	*res2;
	t_shell_env			*env;
	extern char			**environ;

	ast_export = (t_ast *)(*state);
	assert_non_null(ast_export);
	env = create_shell_env(environ);
	assert_non_null(env);
	char *captured = eval_and_capture(ast_export, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	ast_list = create_cmd_ast("export", NULL, 0);
	char *captured2 = eval_and_capture(ast_list, env, &res2);
	assert_non_null(res2);
	assert_non_null(captured2);
	assert_non_null(strstr(captured2, "declare -x FOO=\"bar\""));
	assert_non_null(strstr(captured2, "declare -x BAR=\"foo\""));
	assert_int_equal(res2->exit_code, 0);
	free(captured2);
	free(res2);
	free_cmd_ast(ast_list);
	destroy_shell_env(env);
}

/*
 *   setup: export with invalid var (starts with digit)
 */
int	setup_built_in_export_invalid_digit_ast(void **state)
{
	char	*args[] = {"1FOO=bar"};

	*state = create_cmd_ast("export", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: export invalid var should return error
 */
void	test_eval_built_in_export_invalid_digit(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	char *captured = eval_and_capture_ex(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 1);
	assert_non_null(captured);
	assert_non_null(strstr(captured, "not a valid identifier"));
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: export with invalid var (special char)
 */
int	setup_built_in_export_invalid_special_ast(void **state)
{
	char	*args[] = {"FOO-BAR=baz"};

	*state = create_cmd_ast("export", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: export invalid var with special char should return error
 */
void	test_eval_built_in_export_invalid_special(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	char *captured = eval_and_capture_ex(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 1);
	assert_non_null(captured);
	assert_non_null(strstr(captured, "FOO-BAR=baz"));
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: export with mixed valid and invalid args
 */
int	setup_built_in_export_mixed_args_ast(void **state)
{
	char	*args[] = {"VALID=ok", "2INVALID=bad", "ALSO_VALID=yes"};

	*state = create_cmd_ast("export", args, 3);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: export mixed args should set valid vars and error on invalid
 */
void	test_eval_built_in_export_mixed_args(void **state)
{
	t_ast *ast;
	t_ast *ast_list;
	t_shell_response *res;
	t_shell_response *res2;
	t_shell_env *env;
	extern char **environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	char *captured = eval_and_capture_ex(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 1);
	assert_non_null(captured);
	assert_non_null(strstr(captured, "2INVALID=bad"));
	free(captured);
	free(res);
	ast_list = create_cmd_ast("export", NULL, 0);
	char *captured2 = eval_and_capture(ast_list, env, &res2);
	assert_non_null(res2);
	assert_non_null(captured2);
	assert_non_null(strstr(captured2, "declare -x VALID=\"ok\""));
	assert_non_null(strstr(captured2, "declare -x ALSO_VALID=\"yes\""));
	assert_null(strstr(captured2, "2INVALID"));
	assert_int_equal(res2->exit_code, 0);
	free(captured2);
	free(res2);
	free_cmd_ast(ast_list);
	destroy_shell_env(env);
}
