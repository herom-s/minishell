/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pipe_redir_eval.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 16:51:51 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/05 16:51:53 by hermarti         ###   ########.fr       */
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
	fd = open("/tmp/pipe_redir_out.txt", O_RDONLY);
	assert_true(fd >= 0);
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	assert_string_equal(buffer, "hello\n");
	free(res->output);
	free(res->erro_msg);
	free(res);
	destroy_shell_env(env);
	unlink("/tmp/pipe_redir_out.txt");
}

/*
 * setup: cat < /tmp/pipe_redir_in_out_src.txt | cat > /tmp/pipe_redir_in_out_dest.txt
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

    fd = open("/tmp/pipe_redir_in_out_src.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
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

    ast = (t_ast *)(*state);
    assert_non_null(ast);
    env = create_shell_env(environ);
    assert_non_null(env);
    res = eval_ast(ast, env);
    assert_non_null(res);
    assert_int_equal(res->exit_code, 0);

    fd = open("/tmp/pipe_redir_in_out_dest.txt", O_RDONLY);
    assert_true(fd >= 0);
    memset(buffer, 0, sizeof(buffer));
    read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    assert_string_equal(buffer, "transfer this\n");

    free(res->output);
    free(res->erro_msg);
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

    ast = (t_ast *)(*state);
    assert_non_null(ast);
    env = create_shell_env(environ);
    assert_non_null(env);
    res = eval_ast(ast, env);
    assert_non_null(res);
    assert_int_equal(res->exit_code, 0);

    fd = open("/tmp/pipe_redir_append.txt", O_RDONLY);
    assert_true(fd >= 0);
    memset(buffer, 0, sizeof(buffer));
    read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    assert_string_equal(buffer, "hello\nworld\n");

    free(res->output);
    free(res->erro_msg);
    free(res);
    destroy_shell_env(env);
    unlink("/tmp/tmp/pipe_redir_append.txt");
}