/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/23 18:37:40 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

t_cmd_func_call	*check_cmd(t_ast *shell_ast, t_shell_env *env, char *envp[])
{
	t_cmd_func_call	*call;

	call = ft_calloc(1, sizeof(t_cmd_func_call));
	if (!call)
		return (NULL);
	call->env = env;
	call->cmd_str = get_cmd_str(shell_ast->u_ast.s_simple_cmd.cmd_name,
			shell_ast->u_ast.s_simple_cmd.cmd_suffix, envp);
	call->envp = envp;
	call->cmd_func = get_cmd_func(shell_ast->u_ast.s_simple_cmd.cmd_name);
	call->is_builtin = check_builtin(shell_ast->u_ast.s_simple_cmd.cmd_name);
	return (call);
}

t_cmd_response	*eval_external_cmd(t_ast *shell_ast, char *envp[],
		t_cmd_func_call *call)
{
	t_cmd_response	*cmd_res;
	int				status;
	int				exit_code;
	char			*line;
	char			*tmp;
	pid_t			cmd_pid;
	int				cmd_error_pipe_fd[2];
	int				cmd_res_pipe_fd[2];

	if (pipe(cmd_res_pipe_fd) == -1)
		return (NULL);
	if (pipe(cmd_error_pipe_fd) == -1)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_res_pipe_fd[OUTPUT_END]);
		return (NULL);
	}
	cmd_pid = fork();
	if (cmd_pid == -1)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_res_pipe_fd[OUTPUT_END]);
		close(cmd_error_pipe_fd[INPUT_END]);
		close(cmd_error_pipe_fd[OUTPUT_END]);
		return (NULL);
	}
	if (cmd_pid == 0)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_error_pipe_fd[INPUT_END]);
		dup2(cmd_res_pipe_fd[OUTPUT_END], STDOUT_FILENO);
		dup2(cmd_error_pipe_fd[OUTPUT_END], STDERR_FILENO);
		close(cmd_res_pipe_fd[OUTPUT_END]);
		close(cmd_error_pipe_fd[OUTPUT_END]);
		cmd_res = call->cmd_func(shell_ast, call->cmd_str, call->env, envp);
		if (cmd_res)
		{
			exit_code = cmd_res->exit_code;
			if (cmd_res->output)
				free(cmd_res->output);
			if (cmd_res->erro_msg)
				free(cmd_res->erro_msg);
			if (cmd_res->curr_dir)
				free(cmd_res->curr_dir);
			free(cmd_res);
		}
		else
			exit_code = 1;
		free_cmd_str(call->cmd_str);
		free_ast(shell_ast);
		destroy_shell_env(call->env);
		free(call);
		exit(exit_code);
	}
	else
	{
		close(cmd_res_pipe_fd[OUTPUT_END]);
		close(cmd_error_pipe_fd[OUTPUT_END]);
		cmd_res = ft_calloc(1, sizeof(t_cmd_response));
		if (!cmd_res)
		{
			close(cmd_res_pipe_fd[INPUT_END]);
			close(cmd_error_pipe_fd[INPUT_END]);
			waitpid(cmd_pid, &status, 0);
			return (NULL);
		}
		cmd_res->output = ft_strdup("");
		if (!cmd_res->output)
		{
			close(cmd_res_pipe_fd[INPUT_END]);
			close(cmd_error_pipe_fd[INPUT_END]);
			waitpid(cmd_pid, &status, 0);
			free(cmd_res);
			return (NULL);
		}
		cmd_res->erro_msg = ft_strdup("");
		if (!cmd_res->erro_msg)
		{
			close(cmd_res_pipe_fd[INPUT_END]);
			close(cmd_error_pipe_fd[INPUT_END]);
			waitpid(cmd_pid, &status, 0);
			free(cmd_res);
			return (NULL);
		}
		while ((line = get_next_line(cmd_res_pipe_fd[INPUT_END])))
		{
			tmp = ft_strjoin(cmd_res->output, line);
			free(cmd_res->output);
			free(line);
			if (!tmp)
				break ;
			cmd_res->output = tmp;
		}
		while ((line = get_next_line(cmd_error_pipe_fd[INPUT_END])))
		{
			tmp = ft_strjoin(cmd_res->erro_msg, line);
			free(cmd_res->erro_msg);
			free(line);
			if (!tmp)
				break ;
			cmd_res->erro_msg = tmp;
		}
		close(cmd_res_pipe_fd[INPUT_END]);
		waitpid(cmd_pid, &status, 0);
		if (WIFEXITED(status))
			cmd_res->exit_code = WEXITSTATUS(status);
		else
			cmd_res->exit_code = 1;
	}
	return (cmd_res);
}

t_cmd_response	*eval_cmd(t_ast *shell_ast, t_shell_env *env, char *envp[])
{
	t_cmd_response	*cmd_res;
	t_cmd_func_call	*call;

	call = check_cmd(shell_ast, env, envp);
	if (!call)
		return (NULL);
	if (call->is_builtin)
		cmd_res = call->cmd_func(shell_ast, call->cmd_str, call->env, envp);
	else
		cmd_res = eval_external_cmd(shell_ast, envp, call);
	free_cmd_str(call->cmd_str);
	free(call);
	return (cmd_res);
}

void	func_exec_cmd_pipe(t_ast *shell_ast, t_shell_env *env, char *envp[])
{
	int				exit_code;
	t_cmd_func_call	*call;
	t_cmd_response	*cmd_res;

	exit_code = 1;
	if (shell_ast->type != AST_SIMPLE_CMD)
		exit(1);
	call = check_cmd(shell_ast, env, envp);
	if (!call)
		exit(1);
	cmd_res = call->cmd_func(shell_ast, call->cmd_str, call->env, envp);
	if (cmd_res)
	{
		exit_code = cmd_res->exit_code;
		if (cmd_res->output)
			free(cmd_res->output);
		if (cmd_res->erro_msg)
			free(cmd_res->erro_msg);
		if (cmd_res->curr_dir)
			free(cmd_res->curr_dir);
		free(cmd_res);
	}
	free_cmd_str(call->cmd_str);
	free_ast(shell_ast);
	destroy_shell_env(call->env);
	free(call);
	exit(exit_code);
}

void	eval_pipe_recursive(t_ast *shell_ast, t_shell_env *env, char *envp[],
		int pipefd[2])
{
	pid_t	left_pid;
	int		new_pipefd[2];

	if (shell_ast->type == AST_SIMPLE_CMD)
	{
		func_exec_cmd_pipe(shell_ast, env, envp);
		exit(1);
	}
	else if (shell_ast->type == AST_PIPE_SEQ)
	{
		if (pipe(new_pipefd) == -1)
			exit(1);
		left_pid = fork();
		if (left_pid == -1)
		{
			close(new_pipefd[INPUT_END]);
			close(new_pipefd[OUTPUT_END]);
			exit(1);
		}
		if (left_pid == 0)
		{
			if (pipefd)
			{
				close(pipefd[INPUT_END]);
				close(pipefd[OUTPUT_END]);
			}
			close(new_pipefd[INPUT_END]);
			dup2(new_pipefd[OUTPUT_END], STDOUT_FILENO);
			close(new_pipefd[OUTPUT_END]);
			eval_pipe_recursive(shell_ast->u_ast.s_pipe_seq.left, env, envp,
				new_pipefd);
			exit(1);
		}
		close(new_pipefd[OUTPUT_END]);
		dup2(new_pipefd[INPUT_END], STDIN_FILENO);
		close(new_pipefd[INPUT_END]);
		eval_pipe_recursive(shell_ast->u_ast.s_pipe_seq.right, env, envp,
			pipefd);
		exit(1);
	}
	exit(1);
}

t_cmd_response	*eval_pipe(t_ast *shell_ast, t_shell_env *env, char *envp[])
{
	t_cmd_response	*cmd_res;
	int				cmd_res_pipe_fd[2];
	int				cmd_error_pipe_fd[2];
	pid_t			root_pid;
	int				status;
	char			*line;
	char			*tmp;

	if (shell_ast->type == AST_SIMPLE_CMD)
		return (eval_cmd(shell_ast, env, envp));
	if (pipe(cmd_res_pipe_fd) == -1)
		return (NULL);
	if (pipe(cmd_error_pipe_fd) == -1)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_res_pipe_fd[OUTPUT_END]);
		return (NULL);
	}
	root_pid = fork();
	if (root_pid == -1)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_res_pipe_fd[OUTPUT_END]);
		close(cmd_error_pipe_fd[INPUT_END]);
		close(cmd_error_pipe_fd[OUTPUT_END]);
		return (NULL);
	}
	if (root_pid == 0)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_error_pipe_fd[INPUT_END]);
		dup2(cmd_res_pipe_fd[OUTPUT_END], STDOUT_FILENO);
		dup2(cmd_error_pipe_fd[OUTPUT_END], STDERR_FILENO);
		eval_pipe_recursive(shell_ast, env, envp, cmd_res_pipe_fd);
	}
	close(cmd_res_pipe_fd[OUTPUT_END]);
	close(cmd_error_pipe_fd[OUTPUT_END]);
	cmd_res = ft_calloc(1, sizeof(t_cmd_response));
	if (!cmd_res)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_error_pipe_fd[INPUT_END]);
		waitpid(root_pid, &status, 0);
		return (NULL);
	}
	cmd_res->output = ft_strdup("");
	if (!cmd_res->output)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_error_pipe_fd[INPUT_END]);
		waitpid(root_pid, &status, 0);
		free(cmd_res);
		return (NULL);
	}
	cmd_res->erro_msg = ft_strdup("");
	if (!cmd_res->erro_msg)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		close(cmd_error_pipe_fd[INPUT_END]);
		waitpid(root_pid, &status, 0);
		free(cmd_res);
		return (NULL);
	}
	while ((line = get_next_line(cmd_res_pipe_fd[INPUT_END])))
	{
		tmp = ft_strjoin(cmd_res->output, line);
		free(cmd_res->output);
		free(line);
		if (!tmp)
			break ;
		cmd_res->output = tmp;
	}
	while ((line = get_next_line(cmd_error_pipe_fd[INPUT_END])))
	{
		tmp = ft_strjoin(cmd_res->erro_msg, line);
		free(cmd_res->erro_msg);
		free(line);
		if (!tmp)
			break ;
		cmd_res->erro_msg = tmp;
	}
	close(cmd_res_pipe_fd[INPUT_END]);
	waitpid(root_pid, &status, 0);
	if (WIFEXITED(status))
		cmd_res->exit_code = WEXITSTATUS(status);
	else
		cmd_res->exit_code = 1;
	return (cmd_res);
}

t_shell_response	*eval_ast(t_ast *shell_ast, t_shell_env *env, char *envp[])
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;

	cmd_res = NULL;
	if (shell_ast->type == AST_SIMPLE_CMD)
		cmd_res = eval_cmd(shell_ast, env, envp);
	else if (shell_ast->type == AST_PIPE_SEQ)
		cmd_res = eval_pipe(shell_ast, env, envp);
	if (!cmd_res)
		return (NULL);
	res = ft_calloc(1, sizeof(t_shell_response));
	if (!res)
	{
		if (cmd_res->output)
			free(cmd_res->output);
		if (cmd_res->erro_msg)
			free(cmd_res->erro_msg);
		if (cmd_res->curr_dir)
			free(cmd_res->curr_dir);
		free(cmd_res);
		return (NULL);
	}
	res->output = cmd_res->output;
	res->exit_code = cmd_res->exit_code;
	res->erro_msg = cmd_res->erro_msg;
	res->curr_dir = cmd_res->curr_dir;
	free(cmd_res);
	return (res);
}
