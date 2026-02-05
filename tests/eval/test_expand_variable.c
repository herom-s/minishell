/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expand_variable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_expand.h"
#include "expand.h"
#include "hashtable.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_test_ctx
{
	t_shell_env		*env;
	t_expand_ctx	*ctx;
}	t_test_ctx;

int	setup_expand_test(void **state)
{
	t_test_ctx	*test_ctx;
	char		*envp[] = {NULL};

	test_ctx = calloc(1, sizeof(t_test_ctx));
	if (!test_ctx)
		return (-1);
	test_ctx->env = create_shell_env(envp);
	if (!test_ctx->env)
	{
		free(test_ctx);
		return (-1);
	}
	hashtable_set(test_ctx->env->vars, "HOME", ft_strdup("/home/user"));
	hashtable_set(test_ctx->env->vars, "USER", ft_strdup("testuser"));
	hashtable_set(test_ctx->env->vars, "PATH", ft_strdup("/usr/bin:/bin"));
	test_ctx->env->last_exit_code = 42;
	test_ctx->ctx = create_expand_ctx(test_ctx->env);
	*state = test_ctx;
	return (0);
}

int	teardown_expand_test(void **state)
{
	t_test_ctx	*test_ctx;

	test_ctx = (t_test_ctx *)*state;
	if (test_ctx)
	{
		if (test_ctx->ctx)
			destroy_expand_ctx(test_ctx->ctx);
		if (test_ctx->env)
			destroy_shell_env(test_ctx->env);
		free(test_ctx);
	}
	*state = NULL;
	return (0);
}

void	test_expand_simple_variable(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("$HOME", test_ctx->ctx);
	assert_string_equal(result, "/home/user");
	free(result);
}

void	test_expand_undefined_variable(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("$UNDEFINED_VAR", test_ctx->ctx);
	assert_string_equal(result, "");
	free(result);
}

void	test_expand_exit_status(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("$?", test_ctx->ctx);
	assert_string_equal(result, "42");
	free(result);
}

void	test_expand_shell_pid(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;
	pid_t		pid;

	test_ctx = (t_test_ctx *)*state;
	pid = getpid();
	result = expand_variables("$$", test_ctx->ctx);
	assert_non_null(result);
	assert_int_equal(atoi(result), pid);
	free(result);
}

void	test_expand_shell_name(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("$0", test_ctx->ctx);
	assert_string_equal(result, "minishell");
	free(result);
}

void	test_expand_dollar_at_end(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("test$", test_ctx->ctx);
	assert_string_equal(result, "test$");
	free(result);
}

void	test_expand_dollar_with_invalid_char(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("$@invalid", test_ctx->ctx);
	assert_string_equal(result, "$@invalid");
	free(result);
}

void	test_expand_adjacent_variables(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("$USER$HOME", test_ctx->ctx);
	assert_string_equal(result, "testuser/home/user");
	free(result);
}

void	test_expand_variable_in_double_quotes(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("\"$HOME\"", test_ctx->ctx);
	assert_string_equal(result, "\"/home/user\"");
	free(result);
}

void	test_expand_variable_in_single_quotes(void **state)
{
	t_test_ctx	*test_ctx;
	char		*result;

	test_ctx = (t_test_ctx *)*state;
	result = expand_variables("'$HOME'", test_ctx->ctx);
	assert_string_equal(result, "'$HOME'");
	free(result);
}
