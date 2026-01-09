/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heredoc_eval.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 00:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/09 00:00:00 by hermarti         ###   ########.fr       */
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
 * Helper function to simulate heredoc input by redirecting stdin from a pipe
 */
static int	setup_stdin_from_string(const char *input)
{
	int		pipe_fd[2];
	ssize_t	written;
	int		original_stdin;

	if (pipe(pipe_fd) == -1)
		return (-1);
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	written = write(pipe_fd[1], input, strlen(input));
	close(pipe_fd[1]);
	if (written == -1)
	{
		close(pipe_fd[0]);
		close(original_stdin);
		return (-1);
	}
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		close(original_stdin);
		return (-1);
	}
	close(pipe_fd[0]);
	return (original_stdin);
}

static void	restore_stdin(int original_stdin)
{
	if (original_stdin >= 0)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
}

/*
 *   setup: cat << EOF (with actual execution)
 *          Tests heredoc execution with cat command
 */
int	setup_heredoc_exec_cat_basic(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	cmd = create_cmd_ast("cat", NULL, 0);
	redir = create_io_file_node(DLESS, "EOF");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_heredoc_exec_cat_basic(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	// Setup heredoc input: "line1\nline2\nEOF\n"
	original_stdin = setup_stdin_from_string("line1\nline2\nEOF\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	// Restore stdin before assertions
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	assert_string_equal(res->output, "line1\nline2\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat << DELIMITER > /tmp/heredoc_exec_out.txt
 *          Tests heredoc with output redirection (actual execution)
 */
int	setup_heredoc_exec_with_output(void **state)
{
	t_ast	*cmd;
	t_ast	*heredoc;
	t_ast	*output_redir;
	t_ast	*suffix1;
	t_ast	*suffix2;

	unlink("/tmp/heredoc_exec_out.txt");
	
	cmd = create_cmd_ast("cat", NULL, 0);
	heredoc = create_io_file_node(DLESS, "DELIMITER");
	output_redir = create_io_file_node(GREAT, "/tmp/heredoc_exec_out.txt");
	
	if (!cmd || !heredoc || !output_redir)
		return (-1);
	
	suffix1 = calloc(1, sizeof(t_ast));
	if (!suffix1)
		return (-1);
	suffix1->type = AST_CMD_SUFFIX;
	suffix1->u_ast.s_cmd_suffix.io_file = heredoc;
	suffix1->u_ast.s_cmd_suffix.word = NULL;
	suffix1->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	
	suffix2 = calloc(1, sizeof(t_ast));
	if (!suffix2)
	{
		free(suffix1);
		return (-1);
	}
	suffix2->type = AST_CMD_SUFFIX;
	suffix2->u_ast.s_cmd_suffix.io_file = output_redir;
	suffix2->u_ast.s_cmd_suffix.word = NULL;
	suffix2->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	
	suffix1->u_ast.s_cmd_suffix.cmd_suffix = suffix2;
	cmd->u_ast.s_simple_cmd.cmd_suffix = suffix1;
	
	*state = cmd;
	return (0);
}

void	test_eval_heredoc_exec_with_output(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;
	int					fd;
	char				buffer[256];

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	// Setup heredoc input
	original_stdin = setup_stdin_from_string("test line 1\ntest line 2\nDELIMITER\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	
	// Verify output was written to file
	fd = open("/tmp/heredoc_exec_out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "test line 1\ntest line 2\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/heredoc_exec_out.txt");
}

/*
 *   setup: wc -l << END
 *          Tests heredoc with external command (wc) that counts lines
 */
int	setup_heredoc_exec_wc(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	cmd = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	redir = create_io_file_node(DLESS, "END");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_heredoc_exec_wc(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	// Setup heredoc input with 3 lines
	original_stdin = setup_stdin_from_string("line1\nline2\nline3\nEND\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	// wc -l should output "3"
	assert_string_equal(res->output, "3\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: grep "hello" << LIMITER
 *          Tests heredoc with grep filtering
 */
int	setup_heredoc_exec_grep(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	cmd = create_cmd_ast("grep", (char *[]){"hello", NULL}, 1);
	redir = create_io_file_node(DLESS, "LIMITER");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_heredoc_exec_grep(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	// Setup heredoc input with some lines containing "hello"
	original_stdin = setup_stdin_from_string("hello world\ngoodbye\nhello again\nLIMITER\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	// grep should output lines containing "hello"
	assert_string_equal(res->output, "hello world\nhello again\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: << LIMITER cat (prefix redirection with execution)
 */
int	setup_heredoc_exec_prefix(void **state)
{
	t_ast	*cmd;
	t_ast	*prefix;

	cmd = calloc(1, sizeof(t_ast));
	if (!cmd)
		return (-1);
	
	cmd->type = AST_SIMPLE_CMD;
	cmd->u_ast.s_simple_cmd.cmd_name = "cat";
	cmd->u_ast.s_simple_cmd.cmd_suffix = NULL;
	
	prefix = calloc(1, sizeof(t_ast));
	if (!prefix)
	{
		free(cmd);
		return (-1);
	}
	
	prefix->type = AST_CMD_PREFIX;
	prefix->u_ast.s_cmd_prefix.io_file = create_io_file_node(DLESS, "LIMITER");
	prefix->u_ast.s_cmd_prefix.cmd_prefix = NULL;
	
	if (!prefix->u_ast.s_cmd_prefix.io_file)
	{
		free(prefix);
		free(cmd);
		return (-1);
	}
	
	cmd->u_ast.s_simple_cmd.cmd_prefix = prefix;
	*state = cmd;
	return (0);
}

void	test_eval_heredoc_exec_prefix(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	// Setup heredoc input
	original_stdin = setup_stdin_from_string("prefix test\nanother line\nLIMITER\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	assert_string_equal(res->output, "prefix test\nanother line\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat << EOF (empty heredoc)
 *          Tests heredoc with no content (just limiter)
 */
int	setup_heredoc_exec_empty(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	cmd = create_cmd_ast("cat", NULL, 0);
	redir = create_io_file_node(DLESS, "EOF");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_heredoc_exec_empty(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	// Setup heredoc input with immediate limiter (empty heredoc)
	original_stdin = setup_stdin_from_string("EOF\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	// Empty heredoc should produce empty output
	assert_string_equal(res->output, "");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat << EOF | grep "test"
 *          Tests heredoc piped to grep
 */
int	setup_heredoc_pipe_grep(void **state)
{
	t_ast	*cat_cmd;
	t_ast	*grep_cmd;
	t_ast	*heredoc;
	t_ast	*nodes[2];
	t_ast	*left_redir;
	t_ast	*pipe_node;

	cat_cmd = create_cmd_ast("cat", NULL, 0);
	heredoc = create_io_file_node(DLESS, "EOF");
	if (!cat_cmd || !heredoc)
		return (-1);
	
	nodes[0] = cat_cmd;
	nodes[1] = heredoc;
	left_redir = create_redirection_ast(nodes, 2);
	if (!left_redir)
		return (-1);
	
	grep_cmd = create_cmd_ast("grep", (char *[]){"test", NULL}, 1);
	if (!grep_cmd)
		return (-1);
	
	pipe_node = calloc(1, sizeof(t_ast));
	if (!pipe_node)
		return (-1);
	
	pipe_node->type = AST_PIPE_SEQ;
	pipe_node->u_ast.s_pipe_seq.left = left_redir;
	pipe_node->u_ast.s_pipe_seq.right = grep_cmd;
	
	*state = pipe_node;
	return (0);
}

void	test_eval_heredoc_pipe_grep(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	original_stdin = setup_stdin_from_string("test line\nother line\ntest again\nEOF\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	assert_string_equal(res->output, "test line\ntest again\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat << EOF | wc -l | cat
 *          Tests heredoc with multiple pipes
 */
int	setup_heredoc_multi_pipe(void **state)
{
	t_ast	*cat_cmd;
	t_ast	*wc_cmd;
	t_ast	*cat_cmd2;
	t_ast	*heredoc;
	t_ast	*nodes[2];
	t_ast	*left_redir;
	t_ast	*pipe1;
	t_ast	*pipe2;

	cat_cmd = create_cmd_ast("cat", NULL, 0);
	heredoc = create_io_file_node(DLESS, "EOF");
	if (!cat_cmd || !heredoc)
		return (-1);
	
	nodes[0] = cat_cmd;
	nodes[1] = heredoc;
	left_redir = create_redirection_ast(nodes, 2);
	if (!left_redir)
		return (-1);
	
	wc_cmd = create_cmd_ast("wc", (char *[]){"-l", NULL}, 1);
	cat_cmd2 = create_cmd_ast("cat", NULL, 0);
	if (!wc_cmd || !cat_cmd2)
		return (-1);
	
	pipe1 = calloc(1, sizeof(t_ast));
	pipe2 = calloc(1, sizeof(t_ast));
	if (!pipe1 || !pipe2)
		return (-1);
	
	pipe1->type = AST_PIPE_SEQ;
	pipe1->u_ast.s_pipe_seq.left = left_redir;
	pipe1->u_ast.s_pipe_seq.right = wc_cmd;
	
	pipe2->type = AST_PIPE_SEQ;
	pipe2->u_ast.s_pipe_seq.left = pipe1;
	pipe2->u_ast.s_pipe_seq.right = cat_cmd2;
	
	*state = pipe2;
	return (0);
}

void	test_eval_heredoc_multi_pipe(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	original_stdin = setup_stdin_from_string("line1\nline2\nline3\nline4\nEOF\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	assert_string_equal(res->output, "4\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat << EOF > /tmp/out.txt | grep "test"
 *          Tests heredoc with output redirection and pipe
 */
int	setup_heredoc_redir_and_pipe(void **state)
{
	t_ast	*cat_cmd;
	t_ast	*grep_cmd;
	t_ast	*heredoc;
	t_ast	*out_redir;
	t_ast	*suffix1;
	t_ast	*suffix2;
	t_ast	*pipe_node;

	unlink("/tmp/out.txt");
	
	cat_cmd = create_cmd_ast("cat", NULL, 0);
	heredoc = create_io_file_node(DLESS, "EOF");
	out_redir = create_io_file_node(GREAT, "/tmp/out.txt");
	
	if (!cat_cmd || !heredoc || !out_redir)
		return (-1);
	
	suffix1 = calloc(1, sizeof(t_ast));
	if (!suffix1)
		return (-1);
	suffix1->type = AST_CMD_SUFFIX;
	suffix1->u_ast.s_cmd_suffix.io_file = heredoc;
	suffix1->u_ast.s_cmd_suffix.word = NULL;
	suffix1->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	
	suffix2 = calloc(1, sizeof(t_ast));
	if (!suffix2)
	{
		free(suffix1);
		return (-1);
	}
	suffix2->type = AST_CMD_SUFFIX;
	suffix2->u_ast.s_cmd_suffix.io_file = out_redir;
	suffix2->u_ast.s_cmd_suffix.word = NULL;
	suffix2->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	
	suffix1->u_ast.s_cmd_suffix.cmd_suffix = suffix2;
	cat_cmd->u_ast.s_simple_cmd.cmd_suffix = suffix1;
	
	grep_cmd = create_cmd_ast("grep", (char *[]){"test", NULL}, 1);
	if (!grep_cmd)
		return (-1);
	
	pipe_node = calloc(1, sizeof(t_ast));
	if (!pipe_node)
		return (-1);
	
	pipe_node->type = AST_PIPE_SEQ;
	pipe_node->u_ast.s_pipe_seq.left = cat_cmd;
	pipe_node->u_ast.s_pipe_seq.right = grep_cmd;
	
	*state = pipe_node;
	return (0);
}

void	test_eval_heredoc_redir_and_pipe(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;
	int					fd;
	char				buffer[256];

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	original_stdin = setup_stdin_from_string("test line\nother line\ntest again\nEOF\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 1);
	
	// Check file was written
	fd = open("/tmp/out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "test line\nother line\ntest again\n");
	
	// Pipe output should be empty since cat redirected to file
	assert_string_equal(res->output, "");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/out.txt");
}

/*
 *   setup: cat << EOF | sort | uniq
 *          Tests heredoc with multiple external commands in pipeline
 */
int	setup_heredoc_pipe_sort_uniq(void **state)
{
	t_ast	*cat_cmd;
	t_ast	*sort_cmd;
	t_ast	*uniq_cmd;
	t_ast	*heredoc;
	t_ast	*nodes[2];
	t_ast	*left_redir;
	t_ast	*pipe1;
	t_ast	*pipe2;

	cat_cmd = create_cmd_ast("cat", NULL, 0);
	heredoc = create_io_file_node(DLESS, "EOF");
	if (!cat_cmd || !heredoc)
		return (-1);
	
	nodes[0] = cat_cmd;
	nodes[1] = heredoc;
	left_redir = create_redirection_ast(nodes, 2);
	if (!left_redir)
		return (-1);
	
	sort_cmd = create_cmd_ast("sort", NULL, 0);
	uniq_cmd = create_cmd_ast("uniq", NULL, 0);
	if (!sort_cmd || !uniq_cmd)
		return (-1);
	
	pipe1 = calloc(1, sizeof(t_ast));
	pipe2 = calloc(1, sizeof(t_ast));
	if (!pipe1 || !pipe2)
		return (-1);
	
	pipe1->type = AST_PIPE_SEQ;
	pipe1->u_ast.s_pipe_seq.left = left_redir;
	pipe1->u_ast.s_pipe_seq.right = sort_cmd;
	
	pipe2->type = AST_PIPE_SEQ;
	pipe2->u_ast.s_pipe_seq.left = pipe1;
	pipe2->u_ast.s_pipe_seq.right = uniq_cmd;
	
	*state = pipe2;
	return (0);
}

void	test_eval_heredoc_pipe_sort_uniq(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	original_stdin = setup_stdin_from_string("banana\napple\nbanana\napple\ncherry\nEOF\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	// sort | uniq should output unique sorted lines
	assert_string_equal(res->output, "apple\nbanana\ncherry\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat << END | head -n 2
 *          Tests heredoc piped to head command
 */
int	setup_heredoc_pipe_head(void **state)
{
	t_ast	*cat_cmd;
	t_ast	*head_cmd;
	t_ast	*heredoc;
	t_ast	*nodes[2];
	t_ast	*left_redir;
	t_ast	*pipe_node;

	cat_cmd = create_cmd_ast("cat", NULL, 0);
	heredoc = create_io_file_node(DLESS, "END");
	if (!cat_cmd || !heredoc)
		return (-1);
	
	nodes[0] = cat_cmd;
	nodes[1] = heredoc;
	left_redir = create_redirection_ast(nodes, 2);
	if (!left_redir)
		return (-1);
	
	head_cmd = create_cmd_ast("head", (char *[]){"-n", "2", NULL}, 2);
	if (!head_cmd)
		return (-1);
	
	pipe_node = calloc(1, sizeof(t_ast));
	if (!pipe_node)
		return (-1);
	
	pipe_node->type = AST_PIPE_SEQ;
	pipe_node->u_ast.s_pipe_seq.left = left_redir;
	pipe_node->u_ast.s_pipe_seq.right = head_cmd;
	
	*state = pipe_node;
	return (0);
}

void	test_eval_heredoc_pipe_head(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	original_stdin = setup_stdin_from_string("first\nsecond\nthird\nfourth\nEND\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	assert_string_equal(res->output, "first\nsecond\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat << EOF << EOF2 (multiple heredocs - last one should win)
 */
int	setup_heredoc_multiple(void **state)
{
	t_ast	*cmd;
	t_ast	*heredoc1;
	t_ast	*heredoc2;
	t_ast	*suffix1;
	t_ast	*suffix2;

	cmd = create_cmd_ast("cat", NULL, 0);
	heredoc1 = create_io_file_node(DLESS, "EOF");
	heredoc2 = create_io_file_node(DLESS, "EOF2");
	
	if (!cmd || !heredoc1 || !heredoc2)
		return (-1);
	
	suffix1 = calloc(1, sizeof(t_ast));
	if (!suffix1)
		return (-1);
	suffix1->type = AST_CMD_SUFFIX;
	suffix1->u_ast.s_cmd_suffix.io_file = heredoc1;
	suffix1->u_ast.s_cmd_suffix.word = NULL;
	suffix1->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	
	suffix2 = calloc(1, sizeof(t_ast));
	if (!suffix2)
	{
		free(suffix1);
		return (-1);
	}
	suffix2->type = AST_CMD_SUFFIX;
	suffix2->u_ast.s_cmd_suffix.io_file = heredoc2;
	suffix2->u_ast.s_cmd_suffix.word = NULL;
	suffix2->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	
	suffix1->u_ast.s_cmd_suffix.cmd_suffix = suffix2;
	cmd->u_ast.s_simple_cmd.cmd_suffix = suffix1;
	
	*state = cmd;
	return (0);
}

void	test_eval_heredoc_multiple(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	// First heredoc content, then second heredoc content
	original_stdin = setup_stdin_from_string("first heredoc\nEOF\nsecond heredoc\nEOF2\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	// Last heredoc should be used
	assert_string_equal(res->output, "second heredoc\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: cat << EOF < /dev/null (heredoc with input redirection)
 */
int	setup_heredoc_with_input_redir(void **state)
{
	t_ast	*cmd;
	t_ast	*heredoc;
	t_ast	*input_redir;
	t_ast	*suffix1;
	t_ast	*suffix2;

	cmd = create_cmd_ast("cat", NULL, 0);
	heredoc = create_io_file_node(DLESS, "EOF");
	input_redir = create_io_file_node(LESS, "/dev/null");
	
	if (!cmd || !heredoc || !input_redir)
		return (-1);
	
	suffix1 = calloc(1, sizeof(t_ast));
	if (!suffix1)
		return (-1);
	suffix1->type = AST_CMD_SUFFIX;
	suffix1->u_ast.s_cmd_suffix.io_file = heredoc;
	suffix1->u_ast.s_cmd_suffix.word = NULL;
	suffix1->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	
	suffix2 = calloc(1, sizeof(t_ast));
	if (!suffix2)
	{
		free(suffix1);
		return (-1);
	}
	suffix2->type = AST_CMD_SUFFIX;
	suffix2->u_ast.s_cmd_suffix.io_file = input_redir;
	suffix2->u_ast.s_cmd_suffix.word = NULL;
	suffix2->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	
	suffix1->u_ast.s_cmd_suffix.cmd_suffix = suffix2;
	cmd->u_ast.s_simple_cmd.cmd_suffix = suffix1;
	
	*state = cmd;
	return (0);
}

void	test_eval_heredoc_with_input_redir(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	original_stdin = setup_stdin_from_string("heredoc content\nEOF\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	// Last input redirection wins, so /dev/null should be used (empty)
	assert_string_equal(res->output, "");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   setup: tr 'a-z' 'A-Z' << EOF (heredoc with tr command)
 */
int	setup_heredoc_pipe_tr(void **state)
{
	t_ast	*cmd;
	t_ast	*redir;
	t_ast	*nodes[2];

	cmd = create_cmd_ast("tr", (char *[]){"a-z", "A-Z", NULL}, 2);
	redir = create_io_file_node(DLESS, "EOF");
	if (!cmd || !redir)
		return (-1);
	nodes[0] = cmd;
	nodes[1] = redir;
	*state = create_redirection_ast(nodes, 2);
	if (*state == NULL)
		return (-1);
	return (0);
}

void	test_eval_heredoc_pipe_tr(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	extern char			**environ;
	int					original_stdin;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	
	original_stdin = setup_stdin_from_string("hello world\ntest line\nEOF\n");
	assert_true(original_stdin >= 0);
	
	env = create_shell_env(environ);
	assert_non_null(env);
	
	res = eval_ast(ast, env);
	
	restore_stdin(original_stdin);
	
	assert_non_null(res);
	assert_int_equal(res->exit_code, 0);
	assert_non_null(res->output);
	assert_string_equal(res->output, "HELLO WORLD\nTEST LINE\n");
	
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
}

/*
 *   Teardown function for heredoc tests with custom cleanup
 */
int	teardown_free_heredoc_ast(void **state)
{
	t_ast	*ast;
	t_ast	*suffix;
	t_ast	*next;
	t_ast	*prefix;

	if (!state || !*state)
		return (0);
	
	ast = (t_ast *)(*state);
	
	// Handle different AST types
	if (ast->type == AST_SIMPLE_CMD)
	{
		// Free prefix
		prefix = ast->u_ast.s_simple_cmd.cmd_prefix;
		while (prefix)
		{
			t_ast *next_prefix = prefix->u_ast.s_cmd_prefix.cmd_prefix;
			if (prefix->u_ast.s_cmd_prefix.io_file)
			{
				if (prefix->u_ast.s_cmd_prefix.io_file->u_ast.s_io_file.op)
					free_token(prefix->u_ast.s_cmd_prefix.io_file->u_ast.s_io_file.op);
				free(prefix->u_ast.s_cmd_prefix.io_file);
			}
			free(prefix);
			prefix = next_prefix;
		}
		
		// Free suffix
		suffix = ast->u_ast.s_simple_cmd.cmd_suffix;
		while (suffix)
		{
			next = suffix->u_ast.s_cmd_suffix.cmd_suffix;
			if (suffix->u_ast.s_cmd_suffix.io_file)
			{
				if (suffix->u_ast.s_cmd_suffix.io_file->u_ast.s_io_file.op)
					free_token(suffix->u_ast.s_cmd_suffix.io_file->u_ast.s_io_file.op);
				free(suffix->u_ast.s_cmd_suffix.io_file);
			}
			free(suffix);
			suffix = next;
		}
	}
	else if (ast->type == AST_PIPE_SEQ)
	{
		// Free left side
		if (ast->u_ast.s_pipe_seq.left)
		{
			void *left_state = ast->u_ast.s_pipe_seq.left;
			teardown_free_heredoc_ast(&left_state);
		}
		// Free right side
		if (ast->u_ast.s_pipe_seq.right)
		{
			void *right_state = ast->u_ast.s_pipe_seq.right;
			teardown_free_heredoc_ast(&right_state);
		}
	}
	
	free(ast);
	*state = NULL;
	
	// Cleanup any temporary files
	unlink("/tmp/heredoc_output.txt");
	
	return (0);
}
