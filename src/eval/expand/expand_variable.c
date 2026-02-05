/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:58:10 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 14:58:33 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "hashtable.h"
#include "libft.h"

int	is_valid_var_char(char c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

char	*get_var_value(const char *name, t_expand_ctx *ctx)
{
	char	*value;

	if (!name || !ctx || !ctx->env || !ctx->env->vars)
		return (ft_strdup(""));
	value = hashtable_get(ctx->env->vars, name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

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

char	*expand_var_name(const char *str, int *len, t_expand_ctx *ctx)
{
	char	*name;
	char	*value;
	int		i;

	*len = 0;
	if (!str || !*str)
		return (ft_strdup("$"));
	if (*str == '?')
	{
		*len = 1;
		return (expand_exit_status(ctx));
	}
	if (*str == '$')
	{
		*len = 1;
		return (expand_shell_pid(ctx));
	}
	if (*str == '0')
	{
		*len = 1;
		return (expand_shell_name(ctx));
	}
	if (!is_valid_var_char(*str, 1))
		return (ft_strdup("$"));
	i = 0;
	while (str[i] && is_valid_var_char(str[i], i == 0))
		i++;
	*len = i;
	name = ft_substr(str, 0, i);
	value = get_var_value(name, ctx);
	free(name);
	return (value);
}

static char	*handle_dollar_expansion(const char *str, int *i,
	t_expand_ctx *ctx, t_quote_state q_state)
{
	int		var_len;
	char	*expanded;

	if (q_state == QUOTE_SINGLE)
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	(*i)++;
	expanded = expand_var_name(str + *i, &var_len, ctx);
	*i += var_len;
	return (expanded);
}

char	*expand_variables(const char *str, t_expand_ctx *ctx)
{
	char			*result;
	char			*expanded;
	t_quote_state	q_state;
	int				i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	q_state = QUOTE_NONE;
	i = 0;
	while (str[i])
	{
		q_state = update_quote_state(str[i], q_state);
		if (str[i] == '$' && str[i + 1] && q_state != QUOTE_SINGLE)
		{
			expanded = handle_dollar_expansion(str, &i, ctx, q_state);
			result = ft_strjoin_free(result, expanded);
			free(expanded);
		}
		else
		{
			result = ft_charjoin(result, str[i]);
			i++;
		}
	}
	return (result);
}
