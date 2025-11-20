/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 15:09:25 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/20 15:09:28 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef enum
{
	IDENT,
	KEYWORD,
	PIPE,
	AMPERSAND,
	SQUOTE,
	DQUOTE,
	LBRACE,
	RBRACE,
	LPAREN,
	RPAREN,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	EQUAL,
	ASTERISK,
	QUESTION,
	DOLLAR,
	END,
	ILLEGAL,
}	t_token_type;

typedef struct	s_token
{
	t_token_type	type;
	char			*literal;
}	t_token;

typedef struct	s_lexer
{
	char	*input;
	int		position;
	int		read_position;
	char	ch;
}	t_lexer;

t_token	*get_next_token(t_lexer *lexer);
t_lexer	*create_lexer(char *input);

// Utils
void	read_char(t_lexer *lexer);
void	skip_whitespace(t_lexer *lexer);
#endif
