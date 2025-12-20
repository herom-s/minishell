/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:00:21 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/20 11:14:18 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H
# include "token.h"
# include "parser.h"

typedef enum e_ast_type
{
	AST_LIST,
	AST_AND_OR,
	AST_PIPE_SEQ,
	AST_SUBSHELL,
	AST_SIMPLE_CMD,
	AST_CMD_PREFIX,
	AST_CMD_SUFFIX,
	AST_IO_FILE,
}	t_ast_type;

typedef struct s_ast
{
	t_ast_type	type;
	union
	{
		struct
		{
			struct s_ast	*left;
			struct s_ast	*right;
		}	s_list;
		struct
		{
			struct s_ast	*left;
			t_token			*op;
			struct s_ast	*right;
		}	s_and_or;
		struct
		{
			struct s_ast	*left;
			struct s_ast	*right;
		}	s_pipe_seq;
		struct
		{
			struct s_ast	*and_or;
		}	s_subshell;
		struct
		{
			struct s_ast	*cmd_prefix;
			char			*cmd_name;
			struct s_ast	*cmd_suffix;
		}	s_simple_cmd;
		struct
		{
			struct s_ast	*cmd_prefix;
			struct s_ast	*io_file;
		}	s_cmd_prefix;
		struct
		{
			struct s_ast	*io_file;
			const char		*word;
			struct s_ast	*cmd_suffix;
		}	s_cmd_suffix;
		struct
		{
			t_token		*op;
			const char	*filename;
		}	s_io_file;
	} u_ast;
}	t_ast;

t_ast	*create_ast(t_ast node);
t_ast	*init_ast(t_parser *parser);
t_ast	*parsing(t_parser *parser, char *envp[]);
void	print_ast(t_ast *node, int depth);

// Parse functions
t_ast	*parse_and_or(t_parser *parser);
t_ast	*parse_pipe_sequence(t_parser *parser);
t_ast	*parse_io_redirect(t_parser *parser);
t_ast	*parse_simple_cmd(t_parser *parser);
t_ast	*parse_cmd_prefix(t_parser *parser);
t_ast	*parse_cmd_suffix(t_parser *parser);
t_ast	*parse_subshell(t_parser *parser);

// Free Memory
void	free_ast(t_ast *node);
void	free_left_right_node(t_ast *node);
void	free_subshell(t_ast *node);
void	free_simple_cmd(t_ast *node);
void	free_cmd_prefix(t_ast *node);
void	free_cmd_suffix(t_ast *node);
#endif
