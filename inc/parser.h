/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:19:27 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/07 09:08:14 by thaperei         ###   ########.fr       */
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
	int		has_error;
}	t_parser;

t_parser	*create_parser(t_lexer *lexer);
t_ast		*parsing(t_parser *parser);
void		free_parser(t_parser *parser);
void		next_token(t_parser *parser);

// Parse functions
t_ast	*parse_and_or(t_parser *parser);
t_ast	*parse_pipe_sequence(t_parser *parser);
t_ast	*parse_io_redirect(t_parser *parser);
t_ast	*parse_simple_cmd(t_parser *parser);
t_ast	*parse_cmd_prefix(t_parser *parser);
t_ast	*parse_cmd_suffix(t_parser *parser);
t_ast	*parse_subshell(t_parser *parser);

// Parser helpers
int		cur_token_is(t_token *token, t_token_type expected);
int		peek_token_is(t_parser *parser, t_token_type expected);
void	parser_error(t_parser *parser);
#endif
