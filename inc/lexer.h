/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thawan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 07:31:53 by thawan            #+#    #+#             */
/*   Updated: 2025/11/19 07:31:54 by thawan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

// |  &  ;  <  >  (  )  $  `  \  "  '  <space>  <tab>  <newline>
// = * ?
typedef enum	e_token_type
{
	T_WORD,
	T_PIPE,
	T_AMPERSAND,
	T_SQUOTE,
	T_DQUOTE,
	T_LBRACE,
	T_RBRACE,
	T_LPAREN,
	T_RPAREN,
	T_TAB,
	T_NEWLINE,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_APPEND,
	T_HEREDOC,
	T_EQUAL,
	T_ASTERISK,
	T_QUESTION,
	T_DOLLAR,
	T_EOF,
	T_ILLEGAL,
} t_token_type;

typedef struct	s_token
{
	t_token_type	type;
	char	*type_str;
	char	*literal;
}	t_token;

typedef struct	s_lexer
{
	char	*input;
	int		position;
	int		read_position;
	char	ch;
}	t_lexer;

#endif 
