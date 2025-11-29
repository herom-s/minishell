/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:00:21 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/28 22:02:19 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

typedef enum e_ast_type {
	AST_AND_OR,
	AST_PIPE_SEQUENCE,
	AST_HEREDOC,
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
		struct {
			char	*limiter;
		}	here_doc;
		struct {
			t_token	*op;
			char	*filename;
		}	io_file;
		// Fill more structs based on t_ast_type
	};
}	t_ast;

#endif
