/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:19:27 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/29 13:15:40 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include "ast.h"

typedef struct s_parser
{
	t_lexer	*lexer;
	t_token	*cur_token;
	t_token	*peek_token;
	char	*error_msg;
}	t_parser;

t_ast		*read_input(char *input);
t_parser	*create_parser(t_lexer *lexer);
#endif
