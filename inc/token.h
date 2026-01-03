/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 14:46:31 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/20 10:39:00 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

typedef enum s_token_type
{
	WORD,
	ASSIGN_WORD,
	PIPE,
	OR_IF,
	AMPERSAND,
	AND_IF,
	GREAT,
	DGREAT,
	LESS,
	DLESS,
	LPAREN,
	RPAREN,
	EQUAL,
	END,
	ILLEGAL,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*literal;
	int				len;
}	t_token;

t_token	*create_token(t_token token);
void	free_token(void *node);
#endif
