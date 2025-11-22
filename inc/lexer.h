/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 15:09:25 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/22 15:55:33 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# define CHAR_SIZE 1
# define DUP_SIZE 2

typedef enum s_token_type
{
	WORD,
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

typedef struct s_lexer
{
	char	*input;
	int		position;
	int		read_position;
	char	ch;
	t_token	*tokens;
}	t_lexer;

typedef struct s_hash_item
{
	char	*key;
	int		value;
}	t_hash_item;

t_token	*get_next_token(t_lexer *lexer);
t_lexer	*create_lexer(char *input);
t_token	*create_token(t_token token);
void	read_word(t_lexer *lexer, t_token *token);
void	check_duplicate_operators(t_lexer *lexer, t_token *token);
void	check_operators(t_lexer *lexer, t_token *token);

// Utils
char	peek_char(t_lexer *lexer);
void	next_char(t_lexer *lexer);
void	skip_whitespace(t_lexer *lexer);
#endif
