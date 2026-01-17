/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@example.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/01/16 00:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int	capture_start(int *saved_stdout, int *saved_stderr, int *pipe_read_fd)
{
    int	pipefd[2];

    if (pipe(pipefd) == -1)
        return (-1);
    /* Save originals */
    *saved_stdout = dup(STDOUT_FILENO);
    /* don't save/redirect stderr when we only want stdout captured */
    *saved_stderr = -1;
    if (*saved_stdout == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    /* Redirect stdout to pipe write end */
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        return (-1);
    /* Close write end (fds duplicated) and return read end */
    close(pipefd[1]);
    *pipe_read_fd = pipefd[0];
    return (0);
}

int	capture_start_ex(int *saved_stdout, int *saved_stderr, int *pipe_read_fd,
        int capture_stderr)
{
    int	pipefd[2];

    if (pipe(pipefd) == -1)
        return (-1);
    *saved_stdout = dup(STDOUT_FILENO);
    if (*saved_stdout == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    if (capture_stderr)
    {
        *saved_stderr = dup(STDERR_FILENO);
        if (*saved_stderr == -1)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            close(*saved_stdout);
            return (-1);
        }
    }
    else
        *saved_stderr = -1;
    /* Redirect stdout (and optionally stderr) to pipe write end */
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        return (-1);
    if (capture_stderr)
    {
        if (dup2(pipefd[1], STDERR_FILENO) == -1)
            return (-1);
    }
    close(pipefd[1]);
    *pipe_read_fd = pipefd[0];
    return (0);
}

char	*capture_end(int saved_stdout, int saved_stderr, int pipe_read_fd)
{
    char	*buf = NULL;
    size_t	sz = 0;
    ssize_t	r;
    char	tmp[1024];

    fflush(stdout);
    fflush(stderr);
    /* Restore original stdout */
    if (dup2(saved_stdout, STDOUT_FILENO) == -1)
        return (NULL);
    close(saved_stdout);
    /* restore stderr only if it was saved */
    if (saved_stderr != -1)
    {
        if (dup2(saved_stderr, STDERR_FILENO) == -1)
            return (NULL);
        close(saved_stderr);
    }
    /* Read all data from pipe */
    while ((r = read(pipe_read_fd, tmp, sizeof(tmp))) > 0)
    {
        char *newb = realloc(buf, sz + r + 1);
        if (!newb)
        {
            free(buf);
            close(pipe_read_fd);
            return (NULL);
        }
        buf = newb;
        memcpy(buf + sz, tmp, r);
        sz += r;
        buf[sz] = '\0';
    }
    close(pipe_read_fd);
    if (!buf)
    {
        buf = malloc(1);
        if (buf)
            buf[0] = '\0';
    }
    return (buf);
}

char	*eval_and_capture(t_ast *ast, t_shell_env *env, t_shell_response **res_out)
{
    int saved_out, saved_err, pipe_fd;
    char *captured = NULL;

    if (capture_start(&saved_out, &saved_err, &pipe_fd) != 0)
        return (NULL);
    if (res_out)
        *res_out = eval_ast(ast, env);
    else
        eval_ast(ast, env);
    captured = capture_end(saved_out, saved_err, pipe_fd);
    return (captured);
}

char	*eval_and_capture_ex(t_ast *ast, t_shell_env *env, t_shell_response **res_out,
        int capture_stderr)
{
    int saved_out, saved_err, pipe_fd;
    char *captured = NULL;

    if (capture_start_ex(&saved_out, &saved_err, &pipe_fd, capture_stderr) != 0)
        return (NULL);
    if (res_out)
        *res_out = eval_ast(ast, env);
    else
        eval_ast(ast, env);
    captured = capture_end(saved_out, saved_err, pipe_fd);
    return (captured);
}
