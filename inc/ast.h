/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:00:21 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/01 06:13:27 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H
# include "token.h"

typedef enum e_ast_type
{
	LIST,
	AND_OR,
	PIPE_SEQ,
	SUBSHELL,
	SIMPLE_CMD,
	CMD_PREFIX,
	CMD_SUFFIX,
	IO_FILE,
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
	};
}	t_ast;
#endif
