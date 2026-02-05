/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exit_eval.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:22:50 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 16:28:43 by hermarti         ###   ########.fr       */
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
#include <syscall.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <cmocka.h>

typedef struct s_exit_test_ctx
{
	t_ast		*ast;
	t_shell_env	*env;
}				t_exit_test_ctx;

static t_exit_test_ctx	ctx = {0};

static char	*eval_and_catch_exit(t_ast *ast, t_shell_env *env,
		t_shell_response **res, int *caught_status)
{
	int					outpipe[2];
	int					errpipe[2];
	pid_t				pid;
	char				*buf;
	size_t				total;
	const size_t		CHUNK = 4096;
	char				tmp[CHUNK];
	ssize_t				r;
	char				*nb;
	int					status;
	int					code;
	t_shell_response	*child_res;

	if (caught_status)
		*caught_status = -1;
	if (res)
		*res = NULL;
	if (pipe(outpipe) == -1)
		return (NULL);
	if (pipe(errpipe) == -1)
	{
		close(outpipe[0]);
		close(outpipe[1]);
		return (NULL);
	}
	pid = fork();
	if (pid == -1)
	{
		close(outpipe[0]);
		close(outpipe[1]);
		close(errpipe[0]);
		close(errpipe[1]);
		return (NULL);
	}
	if (pid == 0)
	{
		/* Child: redirect stdout and stderr to pipes */
		close(outpipe[0]);
		close(errpipe[0]);
		if (dup2(outpipe[1], STDOUT_FILENO) == -1)
			_Exit(127);
		if (dup2(errpipe[1], STDERR_FILENO) == -1)
			_Exit(127);
		close(outpipe[1]);
		close(errpipe[1]);
		
		/* Mark this as a non-interactive shell to avoid "exit" message */
		if (env)
			env->interactive_owner = 0;
		
		/* Run the evaluator - if exit builtin is called, child_exit() will be called */
		child_res = eval_ast(ast, env);
		
		/* If we reach here, exit was NOT called (e.g., "exit" with too many args) */
		code = 1;
		if (child_res)
		{
			code = child_res->exit_code;
			free(child_res);
		}
		
		/* Normal exit - exit builtin was not triggered or returned an error */
		_Exit(code);
	}
	
	/* parent */
	close(outpipe[1]);
	close(errpipe[1]);
	
	/* Read stderr (discard it for now, but we need to drain the pipe) */
	while ((r = read(errpipe[0], tmp, CHUNK)) > 0)
		;
	close(errpipe[0]);
	
	/* Read stdout */
	buf = NULL;
	total = 0;
	for (;;)
	{
		r = read(outpipe[0], tmp, CHUNK);
		if (r <= 0)
			break ;
		nb = realloc(buf, total + r + 1);
		if (!nb)
		{
			free(buf);
			buf = NULL;
			break ;
		}
		buf = nb;
		memcpy(buf + total, tmp, r);
		total += r;
		buf[total] = '\0';
	}
	close(outpipe[0]);
	
	status = 0;
	if (waitpid(pid, &status, 0) == -1)
	{
		if (caught_status)
			*caught_status = -1;
		free(buf);
		return (NULL);
	}
	
	if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
		if (caught_status)
			*caught_status = code;
		if (code == 0 && total == 0)
		{
			free(buf);
			return (NULL);
		}
		if (code != 0)
		{
			free(buf);
			return (NULL);
		}
		return (buf);
	}
	else if (WIFSIGNALED(status))
	{
		if (caught_status)
			*caught_status = 128 + WTERMSIG(status);
		free(buf);
		return (NULL);
	}
	
	if (caught_status)
		*caught_status = -1;
	return (buf);
}

/*
 * setup: exit (no args)
 */
int	setup_built_in_exit_no_args_ast(void **state)
{
	extern char		**environ;

	ctx.ast = create_cmd_ast("exit", NULL, 0);
	if (!ctx.ast)
		return (-1);
	ctx.env = create_shell_env(environ);
	if (!ctx.env)
	{
		free_cmd_ast(ctx.ast);
		return (-1);
	}
	ctx.env->interactive_owner = 0;
	ctx.env->last_exit_code = 0;
	*state = &ctx;
	return (0);
}

/* Test: 'exit' with no args
	-> child should exit with status 0 and produce no stdout */
void	test_eval_built_in_exit_no_args_caught(void **state)
{
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	t_exit_test_ctx		*ctx;
	int					caught_status;
	char				*out;

	ctx = (t_exit_test_ctx *)(*state);
	ast = ctx->ast;
	res = NULL;
	caught_status = -1;
	assert_non_null(ast);
	env = ctx->env;
	out = eval_and_catch_exit(ast, env, &res, &caught_status);
	/* evaluator should have triggered exit(0) and not returned normal output */
	assert_null(out);
	assert_int_equal(caught_status, 0);
	if (out)
		free(out);
}

/*
 * setup: exit 42 (numeric arg)
 */
int	setup_built_in_exit_numeric_arg_ast(void **state)
{
	char			*args[] = {"42"};
	extern char		**environ;

	ctx.ast = create_cmd_ast("exit", args, 1);
	if (!ctx.ast)
	{
		return (-1);
	}
	ctx.env = create_shell_env(environ);
	if (!ctx.env)
	{
		free_cmd_ast(ctx.ast);
		return (-1);
	}
	ctx.env->interactive_owner = 0;
	ctx.env->last_exit_code = 0;
	*state = &ctx;
	return (0);
}

/* Test: 'exit 42' -> child should exit with status 42 and produce no stdout */
void	test_eval_built_in_exit_numeric_arg_caught(void **state)
{
	t_exit_test_ctx		*ctx;
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	int					caught_status;
	char				*out;

	ctx = (t_exit_test_ctx *)(*state);
	ast = ctx->ast;
	res = NULL;
	caught_status = -1;
	assert_non_null(ast);
	env = ctx->env;
	out = eval_and_catch_exit(ast, env, &res, &caught_status);
	assert_null(out);
	assert_int_equal(caught_status, 42);
	if (out)
		free(out);
}

/*
 * setup: exit notanumber (invalid arg)
 */
int	setup_built_in_exit_invalid_arg_ast(void **state)
{
	char			*args[] = {"notanumber"};
	extern char		**environ;

	ctx.ast = create_cmd_ast("exit", args, 1);
	if (!ctx.ast)
		return (-1);
	ctx.env = create_shell_env(environ);
	if (!ctx.env)
	{
		free_cmd_ast(ctx.ast);
		return (-1);
	}
	ctx.env->interactive_owner = 0;
	ctx.env->last_exit_code = 0;
	*state = &ctx;
	return (0);
}

/* Test: 'exit notanumber'
	-> child should exit with a non-zero status and produce no stdout */
void	test_eval_built_in_exit_invalid_arg_caught(void **state)
{
	t_exit_test_ctx		*ctx;
	t_ast				*ast;
	t_shell_response	*res;
	t_shell_env			*env;
	int					caught_status;
	char				*out;

	ctx = (t_exit_test_ctx *)(*state);
	ast = ctx->ast;
	res = NULL;
	caught_status = -1;
	assert_non_null(ast);
	env = ctx->env;
	out = eval_and_catch_exit(ast, env, &res, &caught_status);
	assert_null(out);
	assert_int_equal(caught_status, 2);
	if (out)
		free(out);
}

/* teardown: free ctx (ast + env) */
int	teardown_free_exit_ctx(void **state)
{
	t_exit_test_ctx	*ctx;

	if (!state || !*state)
		return (0);
	ctx = (t_exit_test_ctx *)(*state);
	if (ctx->env)
		destroy_shell_env(ctx->env);
	if (ctx->ast)
		free_ast(ctx->ast);
	memset(ctx, 0, sizeof(t_exit_test_ctx));
	*state = NULL;
	return (0);
}
