/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_redir_eval.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:39:49 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/02 18:39:51 by hermarti         ###   ########.fr       */
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
 *   setup: echo hello > /tmp/test_redir_out.txt
 *          output redirection creates file
 */
int	setup_redir_output_create(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	unlink("/tmp/test_redir_out.txt");
	cmd = create_cmd_ast("echo", (char *[]){"hello", NULL}, 1);
	redir = create_io_file_node(GREAT, "/tmp/test_redir_out.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_output_create(void **state)
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
	assert_int_equal(access("/tmp/test_redir_out.txt", F_OK), 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/test_redir_out.txt");
}

/*
 *   setup: echo content > /tmp/test_redir_content.txt
 *          verify file contains correct content
 */
int	setup_redir_output_content(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	unlink("/tmp/test_redir_content.txt");
	cmd = create_cmd_ast("echo", (char *[]){"test_content", NULL}, 1);
	redir = create_io_file_node(GREAT, "/tmp/test_redir_content.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_output_content(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	ssize_t				bytes;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/test_redir_content.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	bytes = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_true(bytes > 0);
	assert_string_equal(buffer, "test_content\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/test_redir_content.txt");
}

/*
 *   setup: echo first > /tmp/test_trunc.txt (file has old content)
 *          output redirection truncates existing file
 */
int	setup_redir_output_truncate(void **state)
{
	t_ast		*cmd;
	t_ast		*redir;
	t_ast		*nodes[2];
	int			fd;
	const char	*old_content = "old_content_here\n";

	fd = open("/tmp/test_trunc.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, old_content, strlen(old_content));
	close(fd);
	cmd = create_cmd_ast("echo", (char *[]){"new", NULL}, 1);
	redir = create_io_file_node(GREAT, "/tmp/test_trunc.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_output_truncate(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/test_trunc.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "new\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/test_trunc.txt");
}

/*
 *   setup: echo first >> /tmp/test_append.txt
 *          append redirection creates file
 */
int	setup_redir_append_create(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	unlink("/tmp/test_append.txt");
	cmd = create_cmd_ast("echo", (char *[]){"first", NULL}, 1);
	redir = create_io_file_node(DGREAT, "/tmp/test_append.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_append_create(void **state)
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
	assert_int_equal(access("/tmp/test_append.txt", F_OK), 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/test_append.txt");
}

/*
 *   setup: echo second >> /tmp/test_append2.txt (file has "first\n")
 *          append redirection appends content
 */
int	setup_redir_append_content(void **state)
{
	t_ast		*cmd;
	t_ast		*redir;
	t_ast		*nodes[2];
	int			fd;
	const char	*first = "first\n";

	fd = open("/tmp/test_append2.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, first, strlen(first));
	close(fd);
	cmd = create_cmd_ast("echo", (char *[]){"second", NULL}, 1);
	redir = create_io_file_node(DGREAT, "/tmp/test_append2.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_append_content(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/test_append2.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "first\nsecond\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/test_append2.txt");
}

/*
 *   setup: cat < /tmp/test_input.txt
 *          input redirection reads file
 */
int	setup_redir_input_read(void **state)
{
	t_ast		*cmd;
	t_ast		*redir;
	t_ast		*nodes[2];
	int			fd;
	const char	*content = "input_test\n";

	fd = open("/tmp/test_input.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	cmd = create_cmd_ast("cat", NULL, 0);
	redir = create_io_file_node(LESS, "/tmp/test_input.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_input_read(void **state)
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
	assert_string_equal(res->output, "input_test\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/test_input.txt");
}

/*
 *   setup: cat < /tmp/nonexistent_file.txt
 *          input redirection with nonexistent file
 */
int	setup_redir_input_nonexistent(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	unlink("/tmp/nonexistent_redir_test.txt");
	cmd = create_cmd_ast("cat", NULL, 0);
	redir = create_io_file_node(LESS, "/tmp/nonexistent_redir_test.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_input_nonexistent(void **state)
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
	assert_int_not_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat < /tmp/input.txt > /tmp/output.txt
 *          combined input and output redirection
 */
int	setup_redir_input_output(void **state)
{
	t_ast		*cmd;
	t_ast		*redir_in;
	t_ast		*redir_out;
	t_ast		*nodes[3];
	int			fd;
	const char	*content = "combined_test\n";

	unlink("/tmp/redir_combined_out.txt");
	fd = open("/tmp/redir_combined_in.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	cmd = create_cmd_ast("cat", NULL, 0);
	redir_in = create_io_file_node(LESS, "/tmp/redir_combined_in.txt");
	redir_out = create_io_file_node(GREAT, "/tmp/redir_combined_out.txt");
	if (!cmd || !redir_in || !redir_out)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir_in;
	nodes[2] = redir_out;
	*state = create_redirection_ast(nodes, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_input_output(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/redir_combined_out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "combined_test\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/redir_combined_in.txt");
	unlink("/tmp/redir_combined_out.txt");
}

/*
 *   setup: echo test > /tmp/multi1.txt > /tmp/multi2.txt
 *          multiple output redirections (last one wins)
 */
int	setup_redir_multiple_output(void **state)
{
	t_ast	*cmd;
	t_ast	*redir1;
	t_ast	*redir2;
	t_ast	*nodes[3];

	unlink("/tmp/multi1.txt");
	unlink("/tmp/multi2.txt");
	cmd = create_cmd_ast("echo", (char *[]){"multi_test", NULL}, 1);
	redir1 = create_io_file_node(GREAT, "/tmp/multi1.txt");
	redir2 = create_io_file_node(GREAT, "/tmp/multi2.txt");
	if (!cmd || !redir1 || !redir2)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir1;
	nodes[2] = redir2;
	*state = create_redirection_ast(nodes, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_multiple_output(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/multi2.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "multi_test\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/multi1.txt");
	unlink("/tmp/multi2.txt");
}

/*
 *   setup: echo test > /nonexistent_dir/file.txt
 *          output to invalid path
 */
int	setup_redir_output_invalid_path(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	cmd = create_cmd_ast("echo", (char *[]){"test", NULL}, 1);
	redir = create_io_file_node(GREAT, "/nonexistent_dir_xyz/file.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_output_invalid_path(void **state)
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
	assert_int_not_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: pwd > /tmp/pwd_redir.txt
 *          builtin with output redirection
 */
int	setup_redir_builtin_output(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	unlink("/tmp/pwd_redir.txt");
	cmd = create_cmd_ast("pwd", NULL, 0);
	redir = create_io_file_node(GREAT, "/tmp/pwd_redir.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_builtin_output(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[PATH_MAX];
	char				cwd[PATH_MAX];
	int					fd;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/pwd_redir.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, "\n");
	assert_string_equal(buffer, cwd);
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pwd_redir.txt");
}

/*
 *   setup: env > /tmp/env_redir.txt
 *          env builtin with output redirection
 */
int	setup_redir_env_output(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	unlink("/tmp/env_redir.txt");
	cmd = create_cmd_ast("env", NULL, 0);
	redir = create_io_file_node(GREAT, "/tmp/env_redir.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_env_output(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[4096];
	int					fd;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/env_redir.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_non_null(strstr(buffer, "PATH="));
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/env_redir.txt");
}

/*
 *   setup: wc -l < /tmp/wc_input.txt
 *          external command with input redirection
 */
int	setup_redir_external_input(void **state)
{
	t_ast		*cmd;
	t_ast		*redir;
	t_ast		*nodes[2];
	int			fd;
	const char	*content = "line1\nline2\nline3\n";

	fd = open("/tmp/wc_input.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	cmd = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	redir = create_io_file_node(LESS, "/tmp/wc_input.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_external_input(void **state)
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
	assert_string_equal(res->output, "3\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/wc_input.txt");
}

/*
 *   setup: grep pattern < /tmp/grep_input.txt > /tmp/grep_output.txt
 *          external command with both redirections
 */
int	setup_redir_external_both(void **state)
{
	t_ast		*cmd;
	t_ast		*redir_in;
	t_ast		*redir_out;
	t_ast		*nodes[3];
	int			fd;
	const char	*content = "apple\nbanana\napricot\ncherry\n";

	unlink("/tmp/grep_output.txt");
	fd = open("/tmp/grep_input.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, content, strlen(content));
	close(fd);
	cmd = create_cmd_ast("grep", (char *[]){"ap", NULL}, 1);
	redir_in = create_io_file_node(LESS, "/tmp/grep_input.txt");
	redir_out = create_io_file_node(GREAT, "/tmp/grep_output.txt");
	if (!cmd || !redir_in || !redir_out)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir_in;
	nodes[2] = redir_out;
	*state = create_redirection_ast(nodes, 3);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_external_both(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				buffer[256];
	int					fd;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/grep_output.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_non_null(strstr(buffer, "apple"));
	assert_non_null(strstr(buffer, "apricot"));
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/grep_input.txt");
	unlink("/tmp/grep_output.txt");
}

/*
 *   setup: echo -n test >> /tmp/append_no_newline.txt
 *          append with echo -n
 */
int	setup_redir_append_no_newline(void **state)
{
	t_ast		*cmd;
	t_ast		*redir;
	t_ast		*nodes[2];
	int			fd;
	const char	*first = "first";

	fd = open("/tmp/append_no_newline.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (-1);
	write(fd, first, strlen(first));
	close(fd);
	cmd = create_cmd_ast("echo", (char *[]){"-n", "second", NULL}, 2);
	redir = create_io_file_node(DGREAT, "/tmp/append_no_newline.txt");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_redir_append_no_newline(void **state)
{
	t_ast *ast;
	t_shell_response *res;
	t_shell_env *env;
	char buffer[256];
	int fd;
	extern char **environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	fd = open("/tmp/append_no_newline.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "firstsecond");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/append_no_newline.txt");
}