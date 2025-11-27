
#ifndef AST_H
# define AST_H

typedef enum e_ast_type {
	AST_AND_OR,
	AST_PIPE_SEQUENCE,
	AST_SIMPLE_CMD,
}	t_ast_type;

typedef struct s_ast
{
	t_ast_type	type;
	union {
		struct {
			struct t_ast	*left;
			struct t_ast	*right;
		}	list;
		// Fill more structs based on t_ast_type
	};
}	t_ast;

#endif
