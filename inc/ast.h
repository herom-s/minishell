/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:00:21 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/05 07:37:00 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H
# include "token.h"

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
		struct s_list
		{
			struct s_ast	*left;
			struct s_ast	*right;
		};
		struct s_and_or
		{
			struct s_ast	*left;
			t_token			*op;
			struct s_ast	*right;
		};
		struct s_pipe_seq
		{
			struct s_ast	*left;
			struct s_ast	*right;
		};
		struct s_subshell
		{
			struct s_ast	*and_or;
		};
		struct s_simple_cmd
		{
			struct s_ast	*cmd_prefix;
			char			*cmd_name;
			struct s_ast	*cmd_suffix;
		};
		struct s_cmd_prefix
		{
			struct s_ast	*cmd_prefix;
			struct s_ast	*io_file;
		};
		struct s_cmd_suffix
		{
			struct s_ast	*io_file;
			const char		*word;
			struct s_ast	*cmd_suffix;
		};
		struct s_io_file
		{
			t_token		*op;
			const char	*filename;
		};
	} u_ast;
}	t_ast;
#endif
