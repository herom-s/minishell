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

/*
 *   setup: single command treated as pipeline
 *          (echo hello)
 */
int	setup_pipe_single_command(void **state)
{
	t_ast	*cmds[1];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	if (!cmds[0])
		return (-1);
	*state = create_pipeline_ast(cmds, 1);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_single_command(void **state)
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
	assert_string_equal(res->output, "hello\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: 12-stage pipeline
 *          (echo test | cat | cat | ... | wc -c)
 */
int	setup_pipe_very_long_chain(void **state)
{
	t_ast	*cmds[12];
	int		i;

	cmds[0] = create_cmd_ast("echo", (char *[]){"test", NULL}, 1);
	if (!cmds[0])
		return (-1);
	i = 1;
	while (i < 11)
	{
		cmds[i] = create_cmd_ast("cat", NULL, 0);
		if (!cmds[i])
			return (-1);
		i++;
	}
	cmds[11] = create_cmd_ast("wc", (char *[]){"-c", NULL}, 1);
	if (!cmds[11])
		return (-1);
	*state = create_pipeline_ast(cmds, 12);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_very_long_chain(void **state)
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
	assert_string_equal(res->output, "5\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: yes | head -n 5
 *          tests SIGPIPE handling (yes generates infinite output)
 */
int	setup_pipe_sigpipe_test(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("yes", NULL, 0);
	cmds[1] = create_cmd_ast("head", (char *[]){"-n", "5", NULL}, 2);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_sigpipe(void **state)
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
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: pwd | cat
 *          builtin piped to external
 */
int	setup_pipe_builtin_pwd_to_cat(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("pwd", NULL, 0);
	cmds[1] = create_cmd_ast("cat", NULL, 0);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_builtin_pwd_to_cat(void **state)
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
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo test | cd /tmp
 *          cd in pipeline (should not affect parent shell)
 */
int	setup_pipe_cd_in_pipeline(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"test", NULL}, 1);
	cmds[1] = create_cmd_ast("cd", (char *[]){"/tmp", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_cd_in_pipeline(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				cwd_before[1024];
	char				cwd_after[1024];
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	getcwd(cwd_before, sizeof(cwd_before));
	res = eval_ast(ast, env);
	getcwd(cwd_after, sizeof(cwd_after));
	assert_non_null(res);
	assert_string_equal(cwd_before, cwd_after);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo hello | exit 42 | wc -l
 *          exit in middle of pipeline
 */
int	setup_pipe_exit_in_middle(void **state)
{
	t_ast	*cmds[3];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	cmds[1] = create_cmd_ast("exit", (char *[]){"42", NULL}, 1);
	cmds[2] = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_exit_in_middle(void **state)
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
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat /tmp/minishell_cat_test.txt | wc -l
 *          pipeline with cat output
 */
int	setup_pipe_cat_file_to_wc(void **state)
{
	const char	*path = "/tmp/minishell_cat_test.txt";
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

void	test_eval_pipe_cat_file_to_wc(void **state)
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
	unlink("/tmp/minishell_redir_test.txt");
}

/*
 *   setup: export TESTVAR=value | env | grep TESTVAR
 *          environment changes should not propagate
 */
int	setup_pipe_export_in_pipeline(void **state)
{
	t_ast	*cmds[3];

	cmds[0] = create_cmd_ast("export", (char *[]){"TESTVAR=pipeline_test",
			NULL}, 1);
	cmds[1] = create_cmd_ast("env", NULL, 0);
	cmds[2] = create_cmd_ast("grep", (char *[]){"TESTVAR", NULL}, 1);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_export_in_pipeline(void **state)
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
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: notfound | cat
 *          command not found at first position
 */
int	setup_pipe_notfound_first(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("notfound_minishell", NULL, 0);
	cmds[1] = create_cmd_ast("cat", NULL, 0);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_notfound_first(void **state)
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
	assert_non_null(res->erro_msg);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo hello | notfound
 *          command not found at last position
 */
int	setup_pipe_notfound_last(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	cmds[1] = create_cmd_ast("notfound_minishell", NULL, 0);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_notfound_last(void **state)
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
	assert_non_null(res->erro_msg);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: env | grep PATH | grep bin
 *          multiple grep stages
 */
int	setup_pipe_grep_chain(void **state)
{
	t_ast	*cmds[3];

	cmds[0] = create_cmd_ast("env", NULL, 0);
	cmds[1] = create_cmd_ast("grep", (char *[]){"PATH", NULL}, 1);
	cmds[2] = create_cmd_ast("grep", (char *[]){"bin", NULL}, 1);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_grep_chain(void **state)
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
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo -e "b\na\nb\na" | sort | uniq
 *          test sort and uniq in pipeline
 */
int	setup_pipe_sort_uniq(void **state)
{
	const char	*path = "/tmp/minishell_sort_test.txt";
	int			fd;
	const char	*content = "b\na\nb\na\n";
	t_ast		*cmds[3];

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
	cmds[1] = create_cmd_ast("sort", NULL, 0);
	cmds[2] = create_cmd_ast("uniq", NULL, 0);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_sort_uniq(void **state)
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
	assert_string_equal(res->output, "a\nb\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/minishell_sort_test.txt");
}

/*
 *   setup: cat file | grep pattern | grep -v exclude
 *          test grep with inverse match
 */
int	setup_pipe_grep_inverse(void **state)
{
	const char	*path = "/tmp/minishell_grep_test.txt";
	int			fd;
	const char	*content = "apple\nbanana\napricot\ncherry\n";
	t_ast		*cmds[3];

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
	cmds[1] = create_cmd_ast("grep", (char *[]){"a", NULL}, 1);
	cmds[2] = create_cmd_ast("grep", (char *[]){"-v", "banana", NULL}, 2);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_grep_inverse(void **state)
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
	assert_non_null(strstr(res->output, "apple"));
	assert_non_null(strstr(res->output, "apricot"));
	assert_null(strstr(res->output, "banana"));
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/minishell_grep_test.txt");
}

/*
 *   setup: /bin/echo test | cat | /usr/bin/wc -c
 *          mixed absolute and relative paths
 */
int	setup_pipe_mixed_paths(void **state)
{
	t_ast	*cmds[3];

	cmds[0] = create_cmd_ast("/bin/echo", (char *[]){"test", NULL}, 1);
	cmds[1] = create_cmd_ast("cat", NULL, 0);
	cmds[2] = create_cmd_ast("/usr/bin/wc", (char *[]){"-c", NULL}, 1);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_mixed_paths(void **state)
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
	assert_string_equal(res->output, "5\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "hello|world" | cat
 *          pipe character inside quotes
 */
int	setup_pipe_special_chars(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello|world", NULL}, 1);
	cmds[1] = create_cmd_ast("cat", NULL, 0);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_special_chars(void **state)
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
	assert_string_equal(res->output, "hello|world\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat file | head -n 5 | tail -n 2
 *          test head and tail combination
 */
int	setup_pipe_head_tail(void **state)
{
	const char	*path = "/tmp/minishell_head_tail_test.txt";
	int			fd;
	const char	*content = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n";
	t_ast		*cmds[3];

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
	cmds[1] = create_cmd_ast("head", (char *[]){"-n", "5", NULL}, 2);
	cmds[2] = create_cmd_ast("tail", (char *[]){"-n", "2", NULL}, 2);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_head_tail(void **state)
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
	assert_string_equal(res->output, "4\n5\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/minishell_head_tail_test.txt");
}

/*
 *   setup: echo "hello" | tr 'a-z' 'A-Z'
 *          test tr command for uppercase conversion
 */
int	setup_pipe_tr_uppercase(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	cmds[1] = create_cmd_ast("tr", (char *[]){"a-z", "A-Z", NULL}, 2);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_tr_uppercase(void **state)
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
	assert_string_equal(res->output, "HELLO\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "field1:field2:field3" | cut -d':' -f2
 *          test cut command for field extraction
 */
int	setup_pipe_cut_field(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"field1:field2:field3", NULL},
			1);
	cmds[1] = create_cmd_ast("cut", (char *[]){"-d:", "-f2", NULL}, 2);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_cut_field(void **state)
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
	assert_string_equal(res->output, "field2\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "hello world" | awk '{print $2}'
 *          test awk command for field printing
 */
int	setup_pipe_awk_field(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello world", NULL}, 1);
	cmds[1] = create_cmd_ast("awk", (char *[]){"{print $2}", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_awk_field(void **state)
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
	assert_string_equal(res->output, "world\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "hello world" | sed 's/world/universe/'
 *          test sed substitution
 */
int	setup_pipe_sed_substitute(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello world", NULL}, 1);
	cmds[1] = create_cmd_ast("sed", (char *[]){"s/world/universe/", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_sed_substitute(void **state)
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
	assert_string_equal(res->output, "hello universe\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "file1 file2 file3" | xargs -n 1 echo "Processing:"
 *          test xargs command
 */
int	setup_pipe_xargs(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"test", NULL}, 1);
	cmds[1] = create_cmd_ast("xargs", (char *[]){"-I", "{}", "echo",
			"Value: {}", NULL}, 4);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_xargs(void **state)
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
	assert_string_equal(res->output, "Value: test\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "test" | tee /tmp/minishell_tee_test.txt | wc -c
 *          test tee command (write to file and stdout)
 */
int	setup_pipe_tee(void **state)
{
	t_ast	*cmds[3];

	cmds[0] = create_cmd_ast("echo", (char *[]){"test", NULL}, 1);
	cmds[1] = create_cmd_ast("tee", (char *[]){"/tmp/minishell_tee_test.txt",
			NULL}, 1);
	cmds[2] = create_cmd_ast("wc", (char *[]){"-c", NULL}, 1);
	if (!cmds[0] || !cmds[1] || !cmds[2])
		return (-1);
	*state = create_pipeline_ast(cmds, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_tee(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[100];
	int					fd;
	ssize_t				bytes_read;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_string_equal(res->output, "5\n");
	assert_int_equal(res->exit_code, 0);
	memset(buffer, 0, sizeof(buffer));
	fd = open("/tmp/minishell_tee_test.txt", O_RDONLY);
	assert_true(fd > 0);
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	assert_true(bytes_read > 0);
	buffer[bytes_read] = '\0';
	close(fd);
	assert_string_equal(buffer, "test\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/minishell_tee_test.txt");
}

/*
 *   setup: echo "hello" | rev
 *          test rev command (reverse string)
 */
int	setup_pipe_rev(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	cmds[1] = create_cmd_ast("rev", NULL, 0);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_rev(void **state)
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
	assert_string_equal(res->output, "olleh\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "hello world" | wc -w
 *          test word count
 */
int	setup_pipe_wc_words(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"hello world test", NULL}, 1);
	cmds[1] = create_cmd_ast("wc", (char *[]){"-w", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_wc_words(void **state)
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
}

/*
 *   setup: ls /tmp | grep minishell
 *          test ls output piped to grep
 */
int	setup_pipe_ls_grep(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("ls", (char *[]){"/tmp", NULL}, 1);
	cmds[1] = create_cmd_ast("grep", (char *[]){"minishell", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_ls_grep(void **state)
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
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "start" | cat | cat | cat
 *          multiple cat stages (passthrough test)
 */
int	setup_pipe_multiple_cats(void **state)
{
	t_ast	*cmds[4];

	cmds[0] = create_cmd_ast("echo", (char *[]){"passthrough", NULL}, 1);
	cmds[1] = create_cmd_ast("cat", NULL, 0);
	cmds[2] = create_cmd_ast("cat", NULL, 0);
	cmds[3] = create_cmd_ast("cat", NULL, 0);
	if (!cmds[0] || !cmds[1] || !cmds[2] || !cmds[3])
		return (-1);
	*state = create_pipeline_ast(cmds, 4);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_multiple_cats(void **state)
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
	assert_string_equal(res->output, "passthrough\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: echo "" | wc -c
 *          test empty string through pipeline
 */
int	setup_pipe_empty_output(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("echo", (char *[]){"", NULL}, 1);
	cmds[1] = create_cmd_ast("wc", (char *[]){"-c", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_empty_output(void **state)
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
 *   setup: printf "test\nline" | wc -l
 *          test printf command in pipeline
 */
int	setup_pipe_printf(void **state)
{
	t_ast	*cmds[2];

	cmds[0] = create_cmd_ast("printf", (char *[]){"test\\nline\\n", NULL}, 1);
	cmds[1] = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	if (!cmds[0] || !cmds[1])
		return (-1);
	*state = create_pipeline_ast(cmds, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_pipe_printf(void **state)
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
	assert_string_equal(res->output, "2\n");
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}