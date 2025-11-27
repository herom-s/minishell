/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:19:27 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/20 14:55:54 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include "ast.h"

typedef struct	s_parser
{
	t_lexer	*lexer;
	t_token	*cur_token;
	t_token	*peek_token;
}	t_parser;

t_ast	*read_input(char *input);
#endif
