/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ctx.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:15:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 16:15:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

static pid_t	get_shell_pid(void)
{
	int		fd;
	char	buf[32];
	int		i;
	pid_t	pid;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (0);
	i = read(fd, buf, 31);
	close(fd);
	if (i <= 0)
		return (0);
	buf[i] = '\0';
	pid = 0;
	i = 0;
	while (buf[i] >= '0' && buf[i] <= '9')
	{
		pid = pid * 10 + (buf[i] - '0');
		i++;
	}
	return (pid);
}

t_expand_ctx	*create_expand_ctx(t_shell_env *env)
{
	t_expand_ctx	*ctx;

	ctx = ft_calloc(1, sizeof(t_expand_ctx));
	if (!ctx)
		return (NULL);
	ctx->env = env;
	ctx->quote_state = QUOTE_NONE;
	if (env)
		ctx->last_exit_code = env->last_exit_code;
	else
		ctx->last_exit_code = 0;
	ctx->shell_pid = get_shell_pid();
	return (ctx);
}

void	destroy_expand_ctx(t_expand_ctx *ctx)
{
	if (ctx)
		free(ctx);
}
