/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/19 17:34:48 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

t_cmd_func_call	*check_cmd(t_ast *shell_ast, t_shell_env *env,
		char *envp[])
{
	t_cmd_func_call	*call;

	call = ft_calloc(1, sizeof(t_cmd_func_call));
	if (!call)
		return (NULL);
	call->env = env;
	call->cmd_str = get_cmd_str(shell_ast->s_simple_cmd.cmd_name,
			shell_ast->s_simple_cmd.cmd_suffix, envp);
	call->envp = envp;
	call->cmd_func = get_cmd_func(shell_ast->s_simple_cmd.cmd_name);
	call->is_builtin = check_builtin(shell_ast->s_simple_cmd.cmd_name);
	return (call);
}

t_cmd_response	*eval_external_cmd(t_ast *shell_ast, char *envp[], t_cmd_func_call *call)
{
	t_cmd_response	*cmd_res;
	int				status;
	char                *line;
    char                *tmp;
	pid_t			cmd_pid;
	int				cmd_res_pipe_fd[2];

	if (pipe(cmd_res_pipe_fd) == -1)
		return (NULL);
	cmd_pid = fork();
    if (cmd_pid == -1)
    {
        close(cmd_res_pipe_fd[INPUT_END]);
        close(cmd_res_pipe_fd[OUTPUT_END]);
        return (NULL);
    }
	if (cmd_pid == 0)
	{
		close(cmd_res_pipe_fd[INPUT_END]);
		dup2(cmd_res_pipe_fd[OUTPUT_END], STDOUT_FILENO);
		close(cmd_res_pipe_fd[OUTPUT_END]);
        cmd_res = call->cmd_func(shell_ast, call->cmd_str, call->env, envp);
        if (cmd_res)
            exit(cmd_res->exit_code);
        else
            exit(1);
	}
	else
    {
        close(cmd_res_pipe_fd[OUTPUT_END]);
        cmd_res = ft_calloc(1, sizeof(t_cmd_response));
        if (!cmd_res)
        {
            close(cmd_res_pipe_fd[INPUT_END]);
            waitpid(cmd_pid, &status, 0);
            return (NULL);
        }
        cmd_res->output = ft_strdup("");
        if (!cmd_res->output)
        {
            close(cmd_res_pipe_fd[INPUT_END]);
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
                break;
            cmd_res->output = tmp;
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

t_cmd_response *eval_cmd(t_ast *shell_ast, t_shell_env *env, char *envp[])
{
    t_cmd_response      *cmd_res;
    t_cmd_func_call     *call;
    
    cmd_res = NULL;
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

t_shell_response	*eval_ast(t_ast *shell_ast, t_shell_env *env, char *envp[])
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;

	res = ft_calloc(1, sizeof(t_shell_response));
	if (!res)
		return (NULL);
	cmd_res = eval_cmd(shell_ast, env, envp);
	res->output = cmd_res->output;
	res->exit_code = cmd_res->exit_code;
	res->erro_msg = cmd_res->erro_msg;
	res->curr_dir = cmd_res->curr_dir;
	free(cmd_res);
	return (res);
}
