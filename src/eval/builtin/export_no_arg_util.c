/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_arg_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:18:44 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/16 14:24:36 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "eval.h"
#include "hashtable.h"

char	*build_env_line(char *key, char *value)
{
	char	*line;
	char	*tmp;

	line = ft_strjoin("declare -x ", key);
	tmp = line;
	line = ft_strjoin(tmp, "=\"");
	free(tmp);
	tmp = line;
	line = ft_strjoin(tmp, value);
	free(tmp);
	tmp = line;
	line = ft_strjoin(tmp, "\"\n");
	free(tmp);
	return (line);
}

void	append_env_line(char **output, char *key, t_shell_env *env)
{
	char	*value;
	char	*line;
	char	*tmp;

	value = hashtable_get(env->vars, key);
	line = build_env_line(key, value);
	tmp = *output;
	*output = ft_strjoin(tmp, line);
	free(tmp);
	free(line);
}

