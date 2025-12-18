/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:46:29 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/11 14:46:30 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "hashtable.h"
#include <stdlib.h>

t_shell_env	*create_shell_env(char *envp[])
{
	char		*tmp_var;
	char		*after_equal;
	t_shell_env	*env;
	size_t		i;

	env = ft_calloc(1, sizeof(t_shell_env));
	if (!env)
		return (NULL);
	env->vars = hashtable_create();
	if (!env->vars)
		return (free(env), NULL);
	env->order = NULL;
	i = 0;
	while (envp[i])
	{
		tmp_var = ft_strdup(envp[i]);
		after_equal = ft_strchr(tmp_var, '=');
		*after_equal = '\0';
		hashtable_set(env->vars, tmp_var, ft_strdup(after_equal + 1));
		ft_lstadd_back(&env->order, ft_lstnew(ft_strdup(tmp_var)));
		free(tmp_var);
		i++;
	}
	return (env);
}

void	*destroy_shell_env(t_shell_env *env)
{
	if (!env)
		return (NULL);
	hashtable_destroy(env->vars);
	ft_lstclear(&env->order, free);
	free(env);
	return (NULL);
}
