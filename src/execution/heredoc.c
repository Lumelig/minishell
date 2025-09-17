#include "minishell.h"

static int	create_heredoc_tempfile(char *delimiter)
{
	char	*filename;
	int		hd_fd;
	char	*line;
	char	*conv_c;

	// itoa not freed after use
	conv_c = ft_itoa(getpid());
	filename = ft_strjoin("/tmp/heredoc_", conv_c);
	hd_fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (hd_fd == -1)
	{
		perror("open");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(hd_fd, line, ft_strlen(line));
		write(hd_fd, "\n", 1);
		free(line);
	}
	close(hd_fd); // reopen file to reset cursor to beginning
	hd_fd = open(filename, O_RDONLY);
	if (hd_fd == -1)
	{
		perror("open");
		exit(1);
	}
	unlink(filename); // Delete file, but fd stays open
	free(filename);
	free(conv_c);
	return (hd_fd);
}

void	handle_heredoc(t_file_node *file)
{
	int	hd_fd;

	hd_fd = create_heredoc_tempfile(file->filename);
	if (file->heredoc_index == file->heredocs_total)
		dup2(hd_fd, STDIN_FILENO);
	close(hd_fd);
}
