#include "minishell.h"

void	handle_redirections(t_cmd_node *curr)
{
	t_file_node	*file;

	if (curr->files && curr->files->head)
	{
		file = curr->files->head;
		while (file)
		{
			if (file->redir_type == TOKEN_REDIR_IN)
				handle_input(file->filename);
			else if (file->redir_type == TOKEN_HEREDOC)
				handle_heredoc(file->filename);
			else if (file->redir_type == TOKEN_REDIR_OUT)
				handle_output(file->filename, false);
			else if (file->redir_type == TOKEN_REDIR_APPEND)
				handle_output(file->filename, true);
			file = file->next;
		}
	}
}

void	handle_input(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		perror(infile);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	handle_output(char *outfile, bool append)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(outfile, flags, 0644);
	if (fd == -1)
	{
		perror(outfile);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
