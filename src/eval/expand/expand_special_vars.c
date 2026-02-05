/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 16:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"

char	*expand_exit_status(t_expand_ctx *ctx)
{
	if (!ctx)
		return (ft_strdup("0"));
	return (ft_itoa(ctx->last_exit_code));
}

char	*expand_shell_pid(t_expand_ctx *ctx)
{
	if (!ctx)
		return (ft_strdup("0"));
	return (ft_itoa(ctx->shell_pid));
}

char	*expand_shell_name(t_expand_ctx *ctx)
{
	(void)ctx;
	return (ft_strdup("minishell"));
}

char	*handle_special_var(char c, t_expand_ctx *ctx)
{
	if (c == '?')
		return (expand_exit_status(ctx));
	if (c == '$')
		return (expand_shell_pid(ctx));
	if (c == '0')
		return (expand_shell_name(ctx));
	return (NULL);
}
