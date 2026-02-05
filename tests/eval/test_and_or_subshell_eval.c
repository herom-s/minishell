/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_and_or_subshell_eval.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:13:07 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/29 14:13:11 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "tests.h"
#include "test_eval.h"
#include "lexer.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cmocka.h>
#include <linux/limits.h>

static t_ast *make_and_or_ast(t_ast *left, t_token_type op_type, t_ast *right)
{
	t_ast	*node;
	t_token 	tok;
	t_token	*op;

	node = calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_AND_OR;
	node->u_ast.s_and_or.left = left;
	node->u_ast.s_and_or.right = right;
	tok = (t_token){op_type, (op_type == AND_IF) ? "&&" : "||", 2};
	op = create_token(tok);
	if (!op)
	{
		free(node);
		return (NULL);
	}
	node->u_ast.s_and_or.op = op;
	return (node);
}

static t_ast *make_subshell_ast(t_ast *inner)
{
	t_ast *node;

	node = calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_SUBSHELL;
	node->u_ast.s_subshell.and_or = inner;
	return (node);
}

/*
 *   setup: left /bin/true && right echo and_ok
 */
int	setup_and_true_and_echo(void **state)
{
	t_ast *left = create_cmd_ast("/bin/true", NULL, 0);
	t_ast *right = create_cmd_ast("echo", (char *[]){"and_ok", NULL}, 1);
	*state = make_and_or_ast(left, AND_IF, right);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: AND (&&) should run the right side when left returns success
 */
void	test_eval_and_runs_right_on_success(void **state)
{
	t_ast *ast = (t_ast *)(*state);
	t_shell_env *env;
	t_shell_response *res = NULL;
	char *captured;
	extern char **environ;

	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_non_null(captured);
	assert_non_null(strstr(captured, "and_ok"));
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: left /bin/false && right echo and_should_not
 */
int	setup_and_false_and_echo(void **state)
{
	t_ast *left = create_cmd_ast("/bin/false", NULL, 0);
	t_ast *right = create_cmd_ast("echo", (char *[]){"and_should_not", NULL}, 1);
	*state = make_and_or_ast(left, AND_IF, right);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: AND (&&) should skip the right side when left fails
 */
void	test_eval_and_skips_right_on_failure(void **state)
{
	t_ast *ast = (t_ast *)(*state);
	t_shell_env *env;
	t_shell_response *res = NULL;
	char *captured;
	extern char **environ;

	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_non_null(captured);
	/* should be empty output */
	assert_int_equal(strlen(captured), 0);
	assert_int_equal(res->exit_code, 1);
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: left /bin/false || right echo or_ok
 */
int	setup_or_false_or_echo(void **state)
{
	t_ast *left = create_cmd_ast("/bin/false", NULL, 0);
	t_ast *right = create_cmd_ast("echo", (char *[]){"or_ok", NULL}, 1);
	*state = make_and_or_ast(left, OR_IF, right);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: OR (||) should run the right side when left fails
 */
void	test_eval_or_runs_right_on_failure(void **state)
{
	t_ast *ast = (t_ast *)(*state);
	t_shell_env *env;
	t_shell_response *res = NULL;
	char *captured;
	extern char **environ;

	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_non_null(captured);
	assert_non_null(strstr(captured, "or_ok"));
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: left /bin/true || right echo or_should_not
 */
int	setup_or_true_or_echo(void **state)
{
	t_ast *left = create_cmd_ast("/bin/true", NULL, 0);
	t_ast *right = create_cmd_ast("echo", (char *[]){"or_should_not", NULL}, 1);
	*state = make_and_or_ast(left, OR_IF, right);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: OR (||) should skip the right side when left succeeds
 */
void	test_eval_or_skips_right_on_success(void **state)
{
	t_ast *ast = (t_ast *)(*state);
	t_shell_env *env;
	t_shell_response *res = NULL;
	char *captured;
	extern char **environ;

	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_non_null(captured);
	/* should be empty output */
	assert_int_equal(strlen(captured), 0);
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: subshell that runs `export FOO=bar`
 */
int	setup_subshell_export(void **state)
{
	t_ast *inner = create_cmd_ast("export", (char *[]){"FOO=bar", NULL}, 1);
	*state = make_subshell_ast(inner);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: export in subshell should not modify parent environment
 */
void	test_subshell_does_not_modify_parent_env(void **state)
{
	t_ast *ast = (t_ast *)(*state);
	t_ast *env_ast;
	t_shell_env *env;
	t_shell_response *res1 = NULL;
	t_shell_response *res2 = NULL;
	char *captured1;
	char *captured2;
	extern char **environ;

	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	/* run subshell export */
	captured1 = eval_and_capture(ast, env, &res1);
	assert_non_null(res1);
	/* now run env to ensure FOO is not present */
	env_ast = create_cmd_ast("env", NULL, 0);
	captured2 = eval_and_capture(env_ast, env, &res2);
	assert_non_null(res2);
	assert_non_null(captured2);
	/* FOO should not be present in parent env */
	assert_null(strstr(captured2, "FOO=bar"));
	free(captured1);
	free(captured2);
	free(res1);
	free(res2);
	free_cmd_ast(env_ast);
	destroy_shell_env(env);
}

/*
 *   setup: subshell that runs `echo hello`
 */
int	setup_subshell_echo(void **state)
{
	t_ast *inner = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	*state = make_subshell_ast(inner);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: echo inside subshell should produce output and exit 0
 */
void	test_subshell_echo_outputs_and_exit_zero(void **state)
{
	t_ast *ast = (t_ast *)(*state);
	t_shell_env *env;
	t_shell_response *res = NULL;
	char *captured;
	extern char **environ;

	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_non_null(captured);
	assert_non_null(strstr(captured, "hello"));
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: subshell that runs `exit 5`
 */
int	setup_subshell_exit_5(void **state)
{
	t_ast *inner = create_cmd_ast("exit", (char *[]){"5", NULL}, 1);
	*state = make_subshell_ast(inner);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: exit in subshell should propagate exit code to parent
 */
void	test_subshell_exit_code_propagated(void **state)
{
	t_ast *ast = (t_ast *)(*state);
	t_shell_env *env;
	t_shell_response *res = NULL;
	char *captured;
	extern char **environ;

	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	/* exit 5 should be propagated */
	assert_int_equal(res->exit_code, 5);
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: subshell that runs `cd /tmp` (should not change parent cwd)
 */
int	setup_subshell_cd(void **state)
{
	t_ast *inner = create_cmd_ast("cd", (char *[]){"/tmp", NULL}, 1);
	*state = make_subshell_ast(inner);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: cd inside subshell should not change parent's current directory
 */
void	test_subshell_cd_does_not_change_parent_cwd(void **state)
{
	t_ast *ast = (t_ast *)(*state);
	t_shell_env *env;
	t_shell_response *res = NULL;
	char *captured;
	char buf[PATH_MAX + 1];
	char *cwd_before;
	extern char **environ;

	assert_non_null(ast);
	cwd_before = getcwd(buf, PATH_MAX);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_string_equal(cwd_before, getcwd(buf, PATH_MAX));
	free(captured);
	free(res);
	destroy_shell_env(env);
}

/*
 *   helper: recursively free tokens in AST_AND_OR nodes before calling free_ast
 *   Tokens in tests are manually created and not owned by a lexer, so we must
 *   free them explicitly to avoid memory leaks.
 */
static void	free_and_or_tokens_recursive(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AST_AND_OR)
	{
		if (ast->u_ast.s_and_or.op)
		{
			free_token(ast->u_ast.s_and_or.op);
			ast->u_ast.s_and_or.op = NULL;
		}
		free_and_or_tokens_recursive(ast->u_ast.s_and_or.left);
		free_and_or_tokens_recursive(ast->u_ast.s_and_or.right);
	}
	else if (ast->type == AST_PIPE_SEQ)
	{
		free_and_or_tokens_recursive(ast->u_ast.s_pipe_seq.left);
		free_and_or_tokens_recursive(ast->u_ast.s_pipe_seq.right);
	}
	else if (ast->type == AST_SUBSHELL)
	{
		free_and_or_tokens_recursive(ast->u_ast.s_subshell.and_or);
	}
}

int	teardown_free_and_or_subshell(void **state)
{
	t_ast	*ast;

	if (!state || !*state)
		return (0);
	ast = (t_ast *)(*state);
	free_and_or_tokens_recursive(ast);
	free_ast(ast);
	*state = NULL;
	return (0);
}
