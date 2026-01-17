/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cd_eval.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 12:05:08 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 16:23:01 by hermarti         ###   ########.fr       */
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
 *   setup: cd /tmp
 */
int	setup_built_in_cd_path_ast(void **state)
{
	char	*args[] = {"/tmp"};
	*state = create_cmd_ast("cd", args, 1);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: cd should be in the dir named /tmp
 */
void	test_eval_built_in_cd_path(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				*buffer;
	char				*old_dir;
	char				*curr_dir;
	char				*expected_dir = "/tmp";
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	mkdir(expected_dir, 0755);
	buffer = calloc(1, PATH_MAX + 1);
	if (!buffer)
		return ;
	old_dir = getcwd(buffer, PATH_MAX);
	chdir(expected_dir);
	curr_dir = getcwd(buffer, PATH_MAX);
	assert_string_equal(res->curr_dir, curr_dir);
	assert_int_equal(res->exit_code, 0);
	chdir(old_dir);
	rmdir(expected_dir);
	free(res->curr_dir);
	free(res);
	free(buffer);
    destroy_shell_env(env);
}

/*
 *   setup: cd /tmp /~ /
 */
int	setup_built_in_cd_to_many_args_ast(void **state)
{
	char	*args[] = {"/tmp", "~/", "/"};
	*state = create_cmd_ast("cd", args, 3);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: cd should give error to many args
 */
void	test_eval_built_in_cd_to_many_args(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	char				*expected_msg = "too many arguments\n";
	extern char			**environ;

	(void)expected_msg;
	ast = (t_ast *)(*state);
	assert_non_null(ast);
	env = create_shell_env(environ);
	assert_non_null(env);
	res = eval_ast(ast, env);
	assert_non_null(res);
	assert_int_equal(res->exit_code, 1);
	free(res);
    destroy_shell_env(env);
}
