/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:37:48 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:01:57 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"

static char	*append_word(char *output, const char *word)
{
	char	*tmp;
	char	*result;

	if (!output)
	{
		if (!word)
			return (ft_strdup(""));
		return (ft_strdup(word));
	}
	tmp = ft_strjoin(output, " ");
	free(output);
	result = ft_strjoin(tmp, word);
	free(tmp);
	return (result);
}

static char	*build_output(t_ast *node, int *newline_flag)
{
	char	*output;

	output = NULL;
	while (node)
	{
		if (ft_strcmp(node->u_ast.s_cmd_suffix.word, "-n") == 0)
			*newline_flag = 0;
		else
			output = append_word(output, node->u_ast.s_cmd_suffix.word);
		node = node->u_ast.s_cmd_suffix.cmd_suffix;
	}
	if (!output)
		return (ft_strdup(""));
	return (output);
}

t_cmd_response	*func_built_in_echo(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;
	char			*output;
	char			*tmp;
	int				newline_flag;

	(void)cmd_str;
	(void)env;
	if (!shell_ast)
		return (NULL);
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	newline_flag = 1;
	output = build_output(shell_ast->u_ast.s_simple_cmd.cmd_suffix,
			&newline_flag);
	if (newline_flag)
	{
		tmp = ft_strjoin(output, "\n");
		free(output);
		output = tmp;
	}
	res->output = output;
	res->exit_code = 0;
	return (res);
}
