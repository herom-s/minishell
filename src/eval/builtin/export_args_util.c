/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_args_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:23:57 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/16 14:24:49 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"

static void	append_error(char **erro_msg, char *arg)
{
	char	*new_err;
	char	*tmp;

	new_err = ft_strjoin("minishell: `", arg);
	tmp = new_err;
	new_err = ft_strjoin(tmp, "': not a valid identifier\n");
	free(tmp);
	if (*erro_msg == NULL)
		*erro_msg = new_err;
	else
	{
		tmp = *erro_msg;
		*erro_msg = ft_strjoin(tmp, new_err);
		free(tmp);
		free(new_err);
	}
}

void	export_var(t_shell_env *env, char *var, char *equal_pos)
{
	if (!hashtable_get(env->vars, var))
		ft_lstadd_back(&env->order, ft_lstnew(ft_strdup(var)));
	if (equal_pos)
		hashtable_set(env->vars, var, ft_strdup(equal_pos + 1));
	else
		hashtable_set(env->vars, var, NULL);
}

int	check_var_name(char *var_name)
{
	size_t	i;

	if (!var_name || !*var_name)
		return (0);
	if (ft_isdigit(var_name[0]))
		return (0);
	i = 0;
	while (var_name[i])
	{
		if (!ft_isalnum(var_name[i]) && var_name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_equal_export(char *arg, t_shell_env *env, char **error)
{
	char	*var;
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	var = ft_substr(arg, 0, equal_pos - arg);
	if (check_var_name(var))
		export_var(env, var, equal_pos);
	else
	{
		append_error(error, arg);
		free(var);
		return (1);
	}
	free(var);
	return (0);
}

int	process_export_arg(char *arg, t_shell_env *env, char **error)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		return (handle_equal_export(arg, env, error));
	if (check_var_name(arg))
	{
		if (!hashtable_get(env->vars, arg))
			export_var(env, arg, NULL);
	}
	else
	{
		append_error(error, arg);
		return (1);
	}
	return (0);
}
