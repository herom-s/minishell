/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 15:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 15:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "expand.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int	get_fd_for_op(const char *filename, t_token_type op)
{
	if (op == GREAT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (op == LESS)
		return (open(filename, O_RDONLY));
	if (op == DGREAT)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

static void	free_matches(char **matches)
{
	int	i;

	if (matches)
	{
		i = 0;
		while (matches[i])
			free(matches[i++]);
		free(matches);
	}
}

static char	*handle_wildcard_redir(char *var_expanded)
{
	char	*expanded;
	char	**matches;
	char	*res;

	expanded = remove_quotes(var_expanded);
	free(var_expanded);
	matches = expand_wildcards(expanded);
	free(expanded);
	if (!matches || !matches[0] || matches[1])
	{
		ft_dprintf(STDERR_FILENO, "minishell: ambiguous redirect\n");
		free_matches(matches);
		return (NULL);
	}
	res = ft_strdup(matches[0]);
	free_matches(matches);
	return (res);
}

char	*expand_redir_target(const char *file, t_shell_env *env)
{
	t_expand_ctx	*ctx;
	char			*var_expanded;
	char			*res;

	ctx = create_expand_ctx(env);
	var_expanded = expand_variables(file, ctx);
	destroy_expand_ctx(ctx);
	if (!var_expanded)
		return (NULL);
	if (has_wildcard(var_expanded))
		return (handle_wildcard_redir(var_expanded));
	res = remove_quotes(var_expanded);
	free(var_expanded);
	return (res);
}
