/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:47:43 by mring             #+#    #+#             */
/*   Updated: 2025/09/19 19:50:33 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sigint_received;

static void	check_fd(int fd)
{
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}
}

static void	hd_loop(int hd_fd, char *delimiter)
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line || g_sigint_received)
		{
			free(line);
			setup_signal_handlers();
			if (g_sigint_received)
				return ;
			break ;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(hd_fd, line, ft_strlen(line));
		write(hd_fd, "\n", 1);
		free(line);
	}
	setup_signal_handlers();
}

static int	create_heredoc_tempfile(char *delimiter)
{
	char		*filename;
	int			hd_fd;
	char		*conv_c;
	static int	hd_counter = 0;

	conv_c = ft_itoa(hd_counter++);
	filename = ft_strjoin("/tmp/heredoc_", conv_c);
	hd_fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0600);
	check_fd(hd_fd);
	hd_loop(hd_fd, delimiter);
	if (g_sigint_received)
	{
		close(hd_fd);
		unlink(filename);
		free(filename);
		free(conv_c);
		return (-1);
	}
	close(hd_fd);
	hd_fd = open(filename, O_RDONLY);
	check_fd(hd_fd);
	unlink(filename);
	return (free(filename), free(conv_c), hd_fd);
}

void	preprocess_heredocs(t_cmd_list *cmd_list)
{
	t_cmd_node	*curr;
	t_file_node	*file;

	curr = cmd_list->head;
	while (curr)
	{
		if (curr->files && curr->files->head)
		{
			file = curr->files->head;
			while (file)
			{
				if (file->redir_type == TOKEN_HEREDOC)
				{
					file->heredoc_fd = create_heredoc_tempfile(file->filename);
					if (file->heredoc_fd == -1)
						return ;
				}
				else
					file->heredoc_fd = -1;
				file = file->next;
			}
		}
		curr = curr->next;
	}
}

void	handle_heredoc(t_file_node *file)
{
	if (file->heredoc_index == file->heredocs_total)
		dup2(file->heredoc_fd, STDIN_FILENO);
	close(file->heredoc_fd);
}
