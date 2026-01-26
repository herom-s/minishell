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
#include <fcntl.h>
#include <errno.h>

/* Safety net: if a saved duplicate ever remains open at process exit,
 * close it to avoid FD reports in valgrind CI. Registered once via atexit. */
static int __capture_leak_saved_stdout = -1;
static int __capture_leak_saved_stderr = -1;
static int __capture_cleanup_registered = 0;
static int __capture_leak_pipe_fd = -1;

static void __capture_cleanup(void)
{
    if (__capture_leak_saved_stdout != -1)
    {
        close(__capture_leak_saved_stdout);
        __capture_leak_saved_stdout = -1;
    }
    if (__capture_leak_saved_stderr != -1)
    {
        close(__capture_leak_saved_stderr);
        __capture_leak_saved_stderr = -1;
    }
    if (__capture_leak_pipe_fd != -1)
    {
        close(__capture_leak_pipe_fd);
        __capture_leak_pipe_fd = -1;
    }
}

int	capture_start(int *saved_stdout, int *saved_stderr, int *pipe_read_fd)
{
    int	pipefd[2];

    if (pipe(pipefd) == -1)
        return (-1);
    /* ensure pipe fds are not inherited by exec'd children */
    {
        int flags;
        flags = fcntl(pipefd[0], F_GETFD);
        if (flags != -1)
            fcntl(pipefd[0], F_SETFD, flags | FD_CLOEXEC);
        flags = fcntl(pipefd[1], F_GETFD);
        if (flags != -1)
            fcntl(pipefd[1], F_SETFD, flags | FD_CLOEXEC);
    }
    /* Save originals */
    /* duplicate starting at fd 3 to avoid reusing low fds */
    *saved_stdout = fcntl(STDOUT_FILENO, F_DUPFD_CLOEXEC, 3);
    /* don't save/redirect stderr when we only want stdout captured */
    *saved_stderr = -1;
    if (*saved_stdout == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    /* record saved fd and register cleanup only on success */
    __capture_leak_saved_stdout = *saved_stdout;
    if (!__capture_cleanup_registered)
    {
        atexit(__capture_cleanup);
        __capture_cleanup_registered = 1;
    }
    /* ensure saved fd is not inherited by exec'd children */
    /* `F_DUPFD_CLOEXEC` already set close-on-exec on the duplicate */
    /* Redirect stdout to pipe write end */
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        if (*saved_stdout != -1)
            close(*saved_stdout);
        __capture_leak_saved_stdout = -1;
        return (-1);
    }
    /* Close write end (fds duplicated) and return read end */
    close(pipefd[1]);
    *pipe_read_fd = pipefd[0];
    __capture_leak_pipe_fd = *pipe_read_fd;
    return (0);
}

int	capture_start_ex(int *saved_stdout, int *saved_stderr, int *pipe_read_fd,
        int capture_stderr)
{
    int	pipefd[2];

    if (pipe(pipefd) == -1)
        return (-1);
    /* ensure pipe fds are not inherited by exec'd children */
    {
        int flags;
        flags = fcntl(pipefd[0], F_GETFD);
        if (flags != -1)
            fcntl(pipefd[0], F_SETFD, flags | FD_CLOEXEC);
        flags = fcntl(pipefd[1], F_GETFD);
        if (flags != -1)
            fcntl(pipefd[1], F_SETFD, flags | FD_CLOEXEC);
    }
    /* duplicate starting at fd 3 to avoid reusing low fds */
    *saved_stdout = fcntl(STDOUT_FILENO, F_DUPFD_CLOEXEC, 3);
    if (*saved_stdout == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    /* record saved fd and register cleanup only on success */
    __capture_leak_saved_stdout = *saved_stdout;
    if (!__capture_cleanup_registered)
    {
        atexit(__capture_cleanup);
        __capture_cleanup_registered = 1;
    }
    /* `F_DUPFD_CLOEXEC` already set close-on-exec on the duplicate */
    if (capture_stderr)
    {
        *saved_stderr = fcntl(STDERR_FILENO, F_DUPFD_CLOEXEC, 3);
        if (*saved_stderr == -1)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            if (*saved_stdout != -1)
                close(*saved_stdout);
            return (-1);
        }
        __capture_leak_saved_stderr = *saved_stderr;
        if (!__capture_cleanup_registered)
        {
            atexit(__capture_cleanup);
            __capture_cleanup_registered = 1;
        }
        /* `F_DUPFD_CLOEXEC` already set close-on-exec on the duplicate */
    }
    else
        *saved_stderr = -1;
    /* Redirect stdout (and optionally stderr) to pipe write end */
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
    {
        if (capture_stderr && *saved_stderr != -1)
        {
            close(*saved_stderr);
            __capture_leak_saved_stderr = -1;
        }
        if (*saved_stdout != -1)
        {
            close(*saved_stdout);
            __capture_leak_saved_stdout = -1;
        }
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    if (capture_stderr)
    {
        if (dup2(pipefd[1], STDERR_FILENO) == -1)
        {
            if (*saved_stderr != -1)
            {
                close(*saved_stderr);
                __capture_leak_saved_stderr = -1;
            }
            if (*saved_stdout != -1)
            {
                close(*saved_stdout);
                __capture_leak_saved_stdout = -1;
            }
            close(pipefd[0]);
            close(pipefd[1]);
            return (-1);
        }
    }
    close(pipefd[1]);
    *pipe_read_fd = pipefd[0];
    __capture_leak_pipe_fd = *pipe_read_fd;
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
    fflush(stdin);

    /* Restore original stdout */
    if (dup2(saved_stdout, STDOUT_FILENO) == -1)
    {
        /* cleanup on failure */
        close(saved_stdout);
        __capture_leak_saved_stdout = -1;
        if (saved_stderr != -1)
            close(saved_stderr);
        close(pipe_read_fd);
        __capture_leak_pipe_fd = -1;
        return (NULL);
    }
    close(saved_stdout);
    /* clear safety tracking: we've closed the saved fd */
    __capture_leak_saved_stdout = -1;
    /* restore stderr only if it was saved */
    if (saved_stderr != -1)
    {
        if (dup2(saved_stderr, STDERR_FILENO) == -1)
        {
            /* cleanup on failure */
            close(saved_stderr);
            __capture_leak_saved_stderr = -1;
            close(pipe_read_fd);
            __capture_leak_pipe_fd = -1;
            return (NULL);
        }
        close(saved_stderr);
        __capture_leak_saved_stderr = -1;
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
    __capture_leak_pipe_fd = -1;
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

    // Start capturing stdout
    if (capture_start(&saved_out, &saved_err, &pipe_fd) != 0)
        return (NULL);

    // Evaluate the AST (heredoc simulation happens here)
    if (res_out)
        *res_out = eval_ast(ast, env);
    else
        eval_ast(ast, env);

    // Capture the command's output
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
