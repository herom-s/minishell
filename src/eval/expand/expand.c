/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:58:02 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 15:00:53 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"

char	*expand_string(const char *str, t_expand_ctx *ctx)
{
	char	*var_expanded;
	char	*result;

	if (!str)
		return (NULL);
	var_expanded = expand_variables(str, ctx);
	if (!var_expanded)
		return (NULL);
	result = remove_quotes(var_expanded);
	free(var_expanded);
	return (result);
}

static int	process_arg(char *arg, char **result, int pos, t_expand_ctx *ctx)
{
	char	*var_expanded;
	char	*expanded;
	int		do_wildcard;

	var_expanded = expand_variables(arg, ctx);
	do_wildcard = has_wildcard(var_expanded);
	expanded = remove_quotes(var_expanded);
	free(var_expanded);
	if (should_skip_empty(arg, expanded))
	{
		free(expanded);
		return (pos);
	}
	if (expanded)
		pos = expand_and_add_wildcards(result, pos, expanded, do_wildcard);
	return (pos);
}

char	**expand_args(char **args, t_shell_env *env)
{
	t_expand_ctx	*ctx;
	char			**result;
	int				i;
	int				pos;

	if (!args)
		return (NULL);
	ctx = create_expand_ctx(env);
	if (!ctx)
		return (NULL);
	result = ft_calloc(count_args(args) * 100 + 1, sizeof(char *));
	if (!result)
		return (destroy_expand_ctx(ctx), NULL);
	i = 0;
	pos = 0;
	while (args[i])
	{
		pos = process_arg(args[i], result, pos, ctx);
		i++;
	}
	result[pos] = NULL;
	destroy_expand_ctx(ctx);
	return (result);
}
