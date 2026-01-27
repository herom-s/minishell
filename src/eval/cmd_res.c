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
	return (res);
}

void	*destroy_cmd_res(t_cmd_response *cmd_res)
{
	if (cmd_res->curr_dir)
	{
		free(cmd_res->curr_dir);
		cmd_res->curr_dir = NULL;
	}
	free(cmd_res);
	cmd_res = NULL;
	return (cmd_res);
}
