/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_res.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:01:26 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 16:07:18 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"
#include <stdlib.h>

t_cmd_response	*create_cmd_res(void)
{
	t_cmd_response	*res;

	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	res->output = ft_strdup("");
	if (!res->output)
	{
		free(res);
		return (NULL);
	}
	res->erro_msg = ft_strdup("");
	if (!res->erro_msg)
	{
		free(res->output);
		free(res);
		return (NULL);
	}
	return (res);
}

void	*destroy_cmd_res(t_cmd_response *cmd_res)
{
	if (cmd_res->output)
	{
		free(cmd_res->output);
		cmd_res->output = NULL;
	}
	if (cmd_res->erro_msg)
	{
		free(cmd_res->erro_msg);
		cmd_res->erro_msg = NULL;
	}
	if (cmd_res->curr_dir)
	{
		free(cmd_res->curr_dir);
		cmd_res->erro_msg = NULL;
	}
	free(cmd_res);
	cmd_res = NULL;
	return (cmd_res);
}
