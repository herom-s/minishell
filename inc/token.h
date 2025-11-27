
#ifndef TOKEN_H
# define TOKEN_H

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

t_token	*create_token(t_token token);
#endif
