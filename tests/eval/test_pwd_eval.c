/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pwd_eval.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/03 17:01:09 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
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
 *   Helper struct to pass test parameters
 */
typedef struct s_test_pwd_params
{
	char		*cmd_name;
	char		**args;
	int			arg_count;
	char		*expected_output;
	int			expected_exit_code;
}				t_test_pwd_params;

/*
 *   create: creates pwd AST from parameters
 */
static t_ast	*create_pwd_ast(char *cmd_name, char **args, int arg_count)
{
	t_ast	*simple;
	t_ast	*suffix;
	t_ast	*prev_suffix;
	int		i;

	simple = calloc(1, sizeof(t_ast));
	if (!simple)
		return (NULL);
	simple->type = SIMPLE_CMD;
	simple->s_simple_cmd.cmd_prefix = NULL;
	simple->s_simple_cmd.cmd_name = strdup(cmd_name);
	simple->s_simple_cmd.cmd_suffix = NULL;
	prev_suffix = NULL;
	i = 0;
	while (i < arg_count)
	{
		suffix = calloc(1, sizeof(t_ast));
		if (!suffix)
			return (NULL);
		suffix->type = CMD_SUFFIX;
		suffix->s_cmd_suffix.io_file = NULL;
		suffix->s_cmd_suffix.word = strdup(args[i]);
		suffix->s_cmd_suffix.cmd_suffix = NULL;
		if (prev_suffix)
			prev_suffix->s_cmd_suffix.cmd_suffix = suffix;
		else
			simple->s_simple_cmd.cmd_suffix = suffix;
		prev_suffix = suffix;
		i++;
	}
	return (simple);
}

/*
 *  teardown: free AST with chained suffixes
 */
int	teardown_free_pwd_ast(void **state)
{
	t_ast	*ast;
	t_ast	*suf;
	t_ast	*next;

	if (!state || !*state)
		return (0);
	ast = (t_ast *)(*state);
	if (ast->type == SIMPLE_CMD)
	{
		if (ast->s_simple_cmd.cmd_name)
			free(ast->s_simple_cmd.cmd_name);
		suf = ast->s_simple_cmd.cmd_suffix;
		while (suf)
		{
			next = suf->s_cmd_suffix.cmd_suffix;
			if (suf->s_cmd_suffix.word)
				free((void *)suf->s_cmd_suffix.word);
			free(suf);
			suf = next;
		}
	}
	free(ast);
	*state = NULL;
	return (0);
}

/*
 *   setup: pwd basic
 */
int	setup_built_in_pwd_basic_ast(void **state)
{
	char	*args[] = {0};

	*state = create_pwd_ast("pwd", args, 0);
	if (!*state)
		return (-1);
	return (0);
}

/*
 *   test: pwd should print the current dir
 */
void	test_eval_built_in_pwd_basic(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	char				*buffer;
	char				*curr_dir;
	char				*expected;
	extern char			**environ;

	ast = (t_ast *)(*state);
	assert_non_null(ast);
	res = eval_ast(ast, environ);
	assert_non_null(res);
	buffer = calloc(1, PATH_MAX + 1);
	if (!buffer)
		return ;
	curr_dir = getcwd(buffer, PATH_MAX);
	expected = calloc(1, strlen(curr_dir) + 2);
	if (!expected)
	{
		free(buffer);
		return ;
	}
	strcpy(expected, curr_dir);
	strcat(expected, "\n");
	assert_string_equal(res->output, expected);
	assert_int_equal(res->exit_code, 0);
	free(res->output);
	free(res->erro_msg);
	free(res);
	free(buffer);
	free(expected);
}
