/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:42:59 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:03:46 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "hashtable.h"
#include "libft.h"

static void	remove_from_order(t_list **order, const char *key)
{
	t_list	*node;
	t_list	*prev;

	prev = NULL;
	node = *order;
	while (node)
	{
		if (ft_strcmp(key, (char *)node->content) == 0)
		{
			if (prev == NULL)
				*order = node->next;
			else
				prev->next = node->next;
			ft_lstdelone(node, free);
			return ;
		}
		prev = node;
		node = node->next;
	}
}

char	*unset_args(char **cmd_str, t_shell_env *env)
{
	size_t	i;
	char	*res;
	size_t	num_args;

	i = 0;
	res = ft_strdup("");
	num_args = num_arguments(cmd_str);
	while (i < num_args)
	{
		if (hashtable_get(env->vars, cmd_str[i + 1]))
		{
			hashtable_delete(env->vars, cmd_str[i + 1]);
			remove_from_order(&env->order, cmd_str[i + 1]);
		}
		i++;
	}
	return (res);
}

t_cmd_response	*func_built_in_unset(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;

	(void)shell_ast;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	if (num_arguments(cmd_str) == 0)
	{
		res->output = ft_strdup("");
		res->exit_code = 0;
		return (res);
	}
	else
	{
		res->output = unset_args(cmd_str, env);
		res->exit_code = 0;
		return (res);
	}
	return (res);
}
