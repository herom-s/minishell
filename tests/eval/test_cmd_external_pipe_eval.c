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
	res = eval_ast(ast, env);
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
	res = eval_ast(ast, env);
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
	assert_string_equal(res->output, "1\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: large file pipeline (cat large_file | wc -c)
 *   Writes 100KB to a file. 100KB > 64KB pipe buffer, so this tests
 *   if processes run in parallel.
 */
int	setup_pipe_large_buffer(void **state)
{
	const char	*path = "/tmp/minishell_large_test.txt";
	int			fd;
	char		*buffer;
	t_ast		*cmds[2];

	int size = 100000; // 100KB
	fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	buffer = malloc(size);
	memset(buffer, 'A', size);
	if (write(fd, buffer, size) < 0)
	{
		free(buffer);
		close(fd);
		return (-1);
	}
	free(buffer);
	close(fd);
	cmds[0] = create_cmd_ast("cat", (char *[]){(char *)path, NULL}, 1);
	cmds[1] = create_cmd_ast("wc", (char *[]){"-c", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	return (0);
}

void	test_eval_pipe_large_buffer(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	env = create_shell_env(environ);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "100000\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/minishell_large_test.txt");
}

/*
 *   setup: builtin to external (echo "12345" | wc -c)
 *   Tests if builtins write to pipe correctly
 */
int	setup_pipe_builtin_to_external(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"12345", NULL}, 1);
	cmds[1] = create_cmd_ast("wc", (char *[]){"-c", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	return (0);
}

void	test_eval_pipe_builtin_to_external(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	env = create_shell_env(environ);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "6\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: false | true
 *   Expects exit code 0 (last command wins)
 */
int	setup_pipe_exit_code_success(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("false", NULL, 0);
	cmds[1] = create_cmd_ast("true", NULL, 0);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	return (0);
}

void	test_eval_pipe_exit_code_success(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	env = create_shell_env(environ);
	res = eval_ast(ast, env);
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: true | false
 *   Expects exit code 1 (last command wins)
 */
int	setup_pipe_exit_code_fail(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("true", NULL, 0);
	cmds[1] = create_cmd_ast("false", NULL, 0);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	return (0);
}

void	test_eval_pipe_exit_code_fail(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	env = create_shell_env(environ);
	res = eval_ast(ast, env);
	assert_int_not_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: ls /tmp/does_not_exist | wc -l
 *   'ls' will write an error to stderr and exit with an error code.
 *   'wc' will receive no input on stdout.
 */
int	setup_pipe_error_propagation(void **state)
{
	t_ast	*cmds[2];
	char	*ls_args[] = {"/tmp/does_not_exist_minishell_test", NULL};
	char	*wc_args[] = {"-l", NULL};

	cmds[0] = create_cmd_ast("ls", ls_args, 1);
	cmds[1] = create_cmd_ast("wc", wc_args, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	return (0);
}

void	test_eval_pipe_error_propagation(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;

	ast = (t_ast *)(*state);
	env = create_shell_env(environ);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "0\n");
	assert_non_null(res->erro_msg);
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: /bin/echo hello | nonexistent | wc -l
 *   Middle command doesn't exist -> produces an stderr message,
	final wc sees no stdout.
 */
int	setup_pipe_mid_cmd_not_found(void **state)
{
	t_ast	*cmds[3];

	cmds[0] = create_cmd_ast("/bin/echo", (char *[]){"hello", NULL}, 1);
	cmds[1] = create_cmd_ast("nonexistent_minishell_test", NULL, 0);
	cmds[2] = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_mid_cmd_not_found(void **state)
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
	assert_string_equal(res->output, "0\n");
	assert_non_null(res->erro_msg);
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: env | grep PATH
 *   Ensure builtin env can be piped into external grep and output contains PATH
 */
int	setup_pipe_env_grep(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("env", NULL, 0);
	cmds[1] = create_cmd_ast("grep", (char *[]){"PATH", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_env_grep(void **state)
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
	assert_non_null(strstr(res->output, "PATH="));
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}
