#include "minishell.h"

static int	create_heredoc_tempfile(char *delimiter)
{
	int			temp_fd;
	char		*line;
	char		*temp_filename;
	static int	counter = 0;

	temp_filename = ft_strjoin("/tmp/heredoc_", ft_itoa(counter++));
	temp_fd = open(temp_filename, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (temp_fd == -1)
	{
		perror("open");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	lseek(temp_fd, 0, SEEK_SET);
	unlink(temp_filename);
	free(temp_filename);
	return (temp_fd);
}

void	handle_heredoc(t_file_node *file_head)
{
	static t_file_node	*last_processed = NULL;
	t_file_node			*file;
	int					first_heredoc_fd;
	bool				found_first;
	int					temp_fd;

	// Skip if we already processed this file chain
	if (last_processed == file_head)
		return ;
	last_processed = file_head;
	found_first = false;
	first_heredoc_fd = -1;
	file = file_head;
	while (file)
	{
		if (file->redir_type == TOKEN_HEREDOC)
		{
			temp_fd = create_heredoc_tempfile(file->filename);
			if (!found_first)
			{
				first_heredoc_fd = temp_fd;
				found_first = true;
			}
			else
				close(temp_fd);
		}
		file = file->next;
	}
	if (first_heredoc_fd != -1)
	{
		dup2(first_heredoc_fd, STDIN_FILENO);
		close(first_heredoc_fd);
	}
}
