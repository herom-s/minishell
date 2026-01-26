/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pipe_redir_eval.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 16:51:51 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/05 18:03:16 by hermarti         ###   ########.fr       */
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
 * setup: cat < /tmp/pipe_redir_in.txt | wc -l
 * Tests input redirection piped to another command
 */
int	setup_pipe_redir_input_to_wc(void **state)
{
	t_ast		*cmd_cat;
	t_ast		*cmd_wc;
	t_ast		*redir_in;
	int			fd;
	const char	*content = "line1\nline2\nline3\n";

	fd = open("/tmp/pipe_redir_in.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	cmd_wc = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	redir_in = create_io_file_node(LESS, "/tmp/pipe_redir_in.txt");
	if (!cmd_cat || !cmd_wc || !redir_in)
		return (-1);
	attach_redir_to_cmd(cmd_cat, redir_in);
	*state = create_pipe_with_redir(cmd_cat, cmd_wc);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_input_to_wc(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_string_equal(captured, "3\n");
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_in.txt");
}

/*
 * setup: echo hello | cat > /tmp/pipe_redir_out.txt
 * Tests pipe with output redirection on the last command
 */
int	setup_pipe_redir_output(void **state)
{
	t_ast	*cmd_echo;
	t_ast	*cmd_cat;
	t_ast	*redir_out;

	unlink("/tmp/pipe_redir_out.txt");
	cmd_echo = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	redir_out = create_io_file_node(GREAT, "/tmp/pipe_redir_out.txt");
	if (!cmd_echo || !cmd_cat || !redir_out)
		return (-1);
	attach_redir_to_cmd(cmd_cat, redir_out);
	*state = create_pipe_with_redir(cmd_echo, cmd_cat);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_output(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pipe_redir_out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "hello\n");
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_out.txt");
}

/*
 * setup: cat < /tmp/pipe_redir_in_out_src.txt | cat >
	/tmp/pipe_redir_in_out_dest.txt

	* Tests input redirection on first command and output redirection on second command
 */
int	setup_pipe_redir_in_out(void **state)
{
	t_ast		*cmd_cat1;
	t_ast		*cmd_cat2;
	t_ast		*redir_in;
	t_ast		*redir_out;
	int			fd;
	const char	*content = "transfer this\n";

	fd = open("/tmp/pipe_redir_in_out_src.txt", O_CREAT | O_TRUNC | O_WRONLY,
			0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	unlink("/tmp/pipe_redir_in_out_dest.txt");
	cmd_cat1 = create_cmd_ast("cat", NULL, 0);
	cmd_cat2 = create_cmd_ast("cat", NULL, 0);
	redir_in = create_io_file_node(LESS, "/tmp/pipe_redir_in_out_src.txt");
	redir_out = create_io_file_node(GREAT, "/tmp/pipe_redir_in_out_dest.txt");
	if (!cmd_cat1 || !cmd_cat2 || !redir_in || !redir_out)
		return (-1);
	attach_redir_to_cmd(cmd_cat1, redir_in);
	attach_redir_to_cmd(cmd_cat2, redir_out);
	*state = create_pipe_with_redir(cmd_cat1, cmd_cat2);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_in_out(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pipe_redir_in_out_dest.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "transfer this\n");
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_in_out_src.txt");
	unlink("/tmp/pipe_redir_in_out_dest.txt");
}

/*
 * setup: echo world | cat >> /tmp/pipe_redir_append.txt
 * Pre-condition: /tmp/pipe_redir_append.txt contains "hello\n"
 */
int	setup_pipe_redir_append(void **state)
{
	t_ast		*cmd_echo;
	t_ast		*cmd_cat;
	t_ast		*redir_append;
	int			fd;
	const char	*content = "hello\n";

	fd = open("/tmp/pipe_redir_append.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	cmd_echo = create_cmd_ast("echo", (char *[]){"world", NULL}, 1);
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	redir_append = create_io_file_node(DGREAT, "/tmp/pipe_redir_append.txt");
	if (!cmd_echo || !cmd_cat || !redir_append)
		return (-1);
	attach_redir_to_cmd(cmd_cat, redir_append);
	*state = create_pipe_with_redir(cmd_echo, cmd_cat);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_append(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pipe_redir_append.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "hello\nworld\n");
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/tmp/pipe_redir_append.txt");
}

/*
 * setup: < /tmp/pipe_redir_prefix_in.txt cat | wc -l
 * Tests prefix input redirection piped to another command
 */
int	setup_pipe_redir_prefix_input_to_wc(void **state)
{
	t_ast		*cmd_cat;
	t_ast		*cmd_wc;
	t_ast		*redir_in;
	int			fd;
	const char	*content = "line1\nline2\nline3\nline4\n";

	fd = open("/tmp/pipe_redir_prefix_in.txt", O_CREAT | O_TRUNC | O_WRONLY,
			0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	cmd_wc = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	redir_in = create_io_file_node(LESS, "/tmp/pipe_redir_prefix_in.txt");
	if (!cmd_cat || !cmd_wc || !redir_in)
		return (-1);
	attach_redir_prefix(cmd_cat, redir_in);
	*state = create_pipe_with_redir(cmd_cat, cmd_wc);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_prefix_input_to_wc(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_string_equal(captured, "4\n");
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_prefix_in.txt");
}

/*
 * setup: echo hello | > /tmp/pipe_redir_prefix_out.txt cat
 * Tests pipe with prefix output redirection on the last command
 */
int	setup_pipe_redir_prefix_output(void **state)
{
	t_ast	*cmd_echo;
	t_ast	*cmd_cat;
	t_ast	*redir_out;

	unlink("/tmp/pipe_redir_prefix_out.txt");
	cmd_echo = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	redir_out = create_io_file_node(GREAT, "/tmp/pipe_redir_prefix_out.txt");
	if (!cmd_echo || !cmd_cat || !redir_out)
		return (-1);
	attach_redir_prefix(cmd_cat, redir_out);
	*state = create_pipe_with_redir(cmd_echo, cmd_cat);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_prefix_output(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pipe_redir_prefix_out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "hello\n");
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_prefix_out.txt");
}

/*
 * setup: < /tmp/pipe_redir_prefix_in_out_src.txt cat | >
	/tmp/pipe_redir_prefix_in_out_dest.txt cat

	* Tests prefix input redirection on first command and prefix output redirection on second command
 */
int	setup_pipe_redir_prefix_in_out(void **state)
{
	t_ast		*cmd_cat1;
	t_ast		*cmd_cat2;
	t_ast		*redir_in;
	t_ast		*redir_out;
	int			fd;
	const char	*content = "prefix transfer\n";

	fd = open("/tmp/pipe_redir_prefix_in_out_src.txt",
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	unlink("/tmp/pipe_redir_prefix_in_out_dest.txt");
	cmd_cat1 = create_cmd_ast("cat", NULL, 0);
	cmd_cat2 = create_cmd_ast("cat", NULL, 0);
	redir_in = create_io_file_node(LESS,
			"/tmp/pipe_redir_prefix_in_out_src.txt");
	redir_out = create_io_file_node(GREAT,
			"/tmp/pipe_redir_prefix_in_out_dest.txt");
	if (!cmd_cat1 || !cmd_cat2 || !redir_in || !redir_out)
		return (-1);
	attach_redir_prefix(cmd_cat1, redir_in);
	attach_redir_prefix(cmd_cat2, redir_out);
	*state = create_pipe_with_redir(cmd_cat1, cmd_cat2);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_prefix_in_out(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pipe_redir_prefix_in_out_dest.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "prefix transfer\n");
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_prefix_in_out_src.txt");
	unlink("/tmp/pipe_redir_prefix_in_out_dest.txt");
}

/*
 * setup: echo world | >> /tmp/pipe_redir_prefix_append.txt cat
 * Pre-condition: /tmp/pipe_redir_prefix_append.txt contains "hello\n"
 * Tests prefix append redirection in pipe
 */
int	setup_pipe_redir_prefix_append(void **state)
{
	t_ast		*cmd_echo;
	t_ast		*cmd_cat;
	t_ast		*redir_append;
	int			fd;
	const char	*content = "hello\n";

	fd = open("/tmp/pipe_redir_prefix_append.txt", O_CREAT | O_TRUNC | O_WRONLY,
			0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	cmd_echo = create_cmd_ast("echo", (char *[]){"world", NULL}, 1);
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	redir_append = create_io_file_node(DGREAT,
			"/tmp/pipe_redir_prefix_append.txt");
	if (!cmd_echo || !cmd_cat || !redir_append)
		return (-1);
	attach_redir_prefix(cmd_cat, redir_append);
	*state = create_pipe_with_redir(cmd_echo, cmd_cat);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_prefix_append(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pipe_redir_prefix_append.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "hello\nworld\n");
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_prefix_append.txt");
}

/*
 * setup: < /tmp/pipe_redir_prefix_complex_in.txt >
	/tmp/pipe_redir_prefix_complex_out.txt cat | wc -c
 * Tests multiple prefix redirections on first command, piped to second command
 */
int	setup_pipe_redir_prefix_complex(void **state)
{
	t_ast		*cmd_cat;
	t_ast		*cmd_wc;
	t_ast		*redir_in;
	t_ast		*redir_out;
	int			fd;
	const char	*content = "complex\n";

	fd = open("/tmp/pipe_redir_prefix_complex_in.txt",
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	unlink("/tmp/pipe_redir_prefix_complex_out.txt");
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	cmd_wc = create_cmd_ast("wc", (char *[]){"-c", NULL}, 1);
	redir_in = create_io_file_node(LESS,
			"/tmp/pipe_redir_prefix_complex_in.txt");
	redir_out = create_io_file_node(GREAT,
			"/tmp/pipe_redir_prefix_complex_out.txt");
	if (!cmd_cat || !cmd_wc || !redir_in || !redir_out)
		return (-1);
	attach_redir_prefix(cmd_cat, redir_in);
	attach_redir_prefix(cmd_cat, redir_out);
	*state = create_pipe_with_redir(cmd_cat, cmd_wc);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_prefix_complex(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	// Output should go to file, not stdout
	fd = open("/tmp/pipe_redir_prefix_complex_out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "complex\n");
	// wc -c should count from its stdin (which gets nothing since cat output went to file)
	// or get the piped content - depending on implementation
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_prefix_complex_in.txt");
	unlink("/tmp/pipe_redir_prefix_complex_out.txt");
}

/*
 * setup: echo data | < /tmp/pipe_redir_prefix_ignore_in.txt >
	/tmp/pipe_redir_prefix_ignore_out.txt cat
 * Tests that prefix input overrides pipe input
 */
int	setup_pipe_redir_prefix_override_pipe(void **state)
{
	t_ast		*cmd_echo;
	t_ast		*cmd_cat;
	t_ast		*redir_in;
	t_ast		*redir_out;
	int			fd;
	const char	*content = "file_content\n";

	fd = open("/tmp/pipe_redir_prefix_ignore_in.txt",
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	unlink("/tmp/pipe_redir_prefix_ignore_out.txt");
	cmd_echo = create_cmd_ast("echo", (char *[]){"pipe_data", NULL}, 1);
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	redir_in = create_io_file_node(LESS,
			"/tmp/pipe_redir_prefix_ignore_in.txt");
	redir_out = create_io_file_node(GREAT,
			"/tmp/pipe_redir_prefix_ignore_out.txt");
	if (!cmd_echo || !cmd_cat || !redir_in || !redir_out)
		return (-1);
	attach_redir_prefix(cmd_cat, redir_in);
	attach_redir_prefix(cmd_cat, redir_out);
	*state = create_pipe_with_redir(cmd_echo, cmd_cat);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_prefix_override_pipe(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pipe_redir_prefix_ignore_out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	// Should contain file content, not pipe data (input redir overrides pipe)
	assert_string_equal(buffer, "file_content\n");
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_prefix_ignore_in.txt");
	unlink("/tmp/pipe_redir_prefix_ignore_out.txt");
}

/*
 * setup: < /tmp/pipe_redir_prefix_mixed_in.txt cat | grep test >
	/tmp/pipe_redir_prefix_mixed_out.txt
 * Tests prefix input on first command and suffix output on second command
 */
int	setup_pipe_redir_prefix_suffix_mixed(void **state)
{
	t_ast		*cmd_cat;
	t_ast		*cmd_grep;
	t_ast		*redir_in;
	t_ast		*redir_out;
	int			fd;
	const char	*content = "test line\nother line\ntest again\n";

	fd = open("/tmp/pipe_redir_prefix_mixed_in.txt",
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	unlink("/tmp/pipe_redir_prefix_mixed_out.txt");
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	cmd_grep = create_cmd_ast("grep", (char *[]){"test", NULL}, 1);
	redir_in = create_io_file_node(LESS, "/tmp/pipe_redir_prefix_mixed_in.txt");
	redir_out = create_io_file_node(GREAT,
			"/tmp/pipe_redir_prefix_mixed_out.txt");
	if (!cmd_cat || !cmd_grep || !redir_in || !redir_out)
		return (-1);
	attach_redir_prefix(cmd_cat, redir_in);
	attach_redir_to_cmd(cmd_grep, redir_out);
	*state = create_pipe_with_redir(cmd_cat, cmd_grep);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_prefix_suffix_mixed(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pipe_redir_prefix_mixed_out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "test line\ntest again\n");
	free(captured);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_prefix_mixed_in.txt");
	unlink("/tmp/pipe_redir_prefix_mixed_out.txt");
}

/*
 * setup: < /tmp/pipe_redir_prefix_nonexist.txt cat | wc -l
 * Tests prefix input redirection with nonexistent file
 */
int	setup_pipe_redir_prefix_input_nonexistent(void **state)
{
	t_ast	*cmd_cat;
	t_ast	*cmd_wc;
	t_ast	*redir_in;

	unlink("/tmp/pipe_redir_prefix_nonexist.txt");
	cmd_cat = create_cmd_ast("cat", NULL, 0);
	cmd_wc = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	redir_in = create_io_file_node(LESS, "/tmp/pipe_redir_prefix_nonexist.txt");
	if (!cmd_cat || !cmd_wc || !redir_in)
		return (-1);
	attach_redir_prefix(cmd_cat, redir_in);
	*state = create_pipe_with_redir(cmd_cat, cmd_wc);
	if (!*state)
		return (-1);
	return (0);
}

void	test_eval_pipe_redir_prefix_input_nonexistent(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	char				*captured;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	captured = eval_and_capture(ast, env, &res);
	assert_non_null(res);
	// should give 0 becaus echo is have succes
	assert_int_equal(res->exit_code, 0);
	free(captured);
	free(res);
	destroy_shell_env(env);
}
