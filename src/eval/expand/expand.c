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
#include <fcntl.h>
#include <unistd.h>

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_charjoin(char *s, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (ft_strjoin_free(s, tmp));
}

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

static char	*expand_vars_only(const char *str, t_expand_ctx *ctx)
{
	if (!str)
		return (NULL);
	return (expand_variables(str, ctx));
}

static int	count_args(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

static int	add_expanded_args(char **result, int pos, char **expanded)
{
	int	i;

	i = 0;
	while (expanded && expanded[i])
	{
		result[pos++] = ft_strdup(expanded[i]);
		i++;
	}
	return (pos);
}

static void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static int	expand_and_add_wildcards(char **result, int pos, char *expanded,
	int do_wildcard)
{
	char	**wildcard_matches;

	if (do_wildcard && has_wildcard(expanded))
	{
		wildcard_matches = expand_wildcards(expanded);
		if (wildcard_matches)
		{
			pos = add_expanded_args(result, pos, wildcard_matches);
			free_str_array(wildcard_matches);
			free(expanded);
			return (pos);
		}
	}
	result[pos++] = expanded;
	return (pos);
}

static int	should_skip_empty(const char *original, const char *expanded)
{
	if (!expanded || expanded[0] == '\0')
	{
		if (original && original[0] == '$')
			return (1);
	}
	return (0);
}

char	**expand_args(char **args, t_shell_env *env)
{
	t_expand_ctx	*ctx;
	char			**result;
	char			*var_expanded;
	char			*expanded;
	int				i;
	int				pos;
	int				do_wildcard;

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
		var_expanded = expand_vars_only(args[i], ctx);
		do_wildcard = has_wildcard(var_expanded);
		expanded = remove_quotes(var_expanded);
		free(var_expanded);
		if (should_skip_empty(args[i], expanded))
			free(expanded);
		else if (expanded)
			pos = expand_and_add_wildcards(result, pos, expanded, do_wildcard);
		i++;
	}
	result[pos] = NULL;
	destroy_expand_ctx(ctx);
	return (result);
}
