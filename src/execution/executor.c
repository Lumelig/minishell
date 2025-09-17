#include "minishell.h"

// void		debug_print_cmd_list(t_cmd_list *cmd_list, const char *location);

static void	forking(t_cmd_node *curr, t_cmd_node *prev, t_env *ms_env,
		pid_t *pid, ssize_t *child_count, t_cmd_list *cmd_list)
{
	pid[*child_count] = fork();
	if (pid[*child_count] == 0)
		handle_child(curr, prev, ms_env, cmd_list);
	else if (pid[*child_count] > 0)
	{
		handle_parent(curr, prev);
		(*child_count)++;
	}
	else
	{
		perror("fork");
		*exit_code() = 1;
	}
}

static void	create_pipes(t_cmd_node *curr)
{
	if (curr->next != NULL)
	{
		if (pipe(curr->p_fd) == -1)
		{
			perror("pipe failed");
			*exit_code() = 1;
			return ; // TODO: proper exit, returns but continues exec
		}
	}
}

static int	cmd_check(t_cmd_list *cmd_list)
{
	if (!cmd_list)
	{
		*exit_code() = 1;
		printf("NULL cmd_list\n");
		return (true);
	}
	if (!cmd_list->head)
	{
		*exit_code() = 1;
		printf("no cmd_list\n");
		return (true);
	}
	if (!cmd_list->head->cmd)
	{
		*exit_code() = 1;
		printf("NULL cmd\n");
		return (true);
	}
	if (!cmd_list->head->cmd[0])
	{
		*exit_code() = 1;
		printf("empty cmd\n");
		return (true);
	}
	return (false);
}

// could add pid to cmd_node struct and adjust forking & wait_children.
// same with child_count
// or cmd_list into cmd_node for reference (cleanup and freeing in exit)
void	executor(t_cmd_list *cmd_list, t_env *ms_env)
{
	t_cmd_node	*curr;
	t_cmd_node	*prev;
	pid_t		pid[cmd_list->size];
	ssize_t		child_count;

	if (cmd_check(cmd_list))
		return ;
	// debug_print_cmd_list(cmd_list, "executor");
	curr = cmd_list->head;
	child_count = 0;
	prev = NULL;
	while (curr)
	{
		create_pipes(curr);
		if (cmd_list->size == 1 && check_builtin(curr))
			handle_single_builtin(curr, ms_env, cmd_list);
		else
			forking(curr, prev, ms_env, pid, &child_count, cmd_list);
		prev = curr;
		curr = curr->next;
	}
	wait_children(pid, child_count);
	return ;
}

// debug

// void	debug_print_cmd_list(t_cmd_list *cmd_list, const char *location)
// {
// 	t_cmd_node *cmd;
// 	t_file_node *file;
// 	int cmd_index = 0;
// 	int file_index;

// 	printf("\n=== DEBUG: %s ===\n", location ? location : "UNKNOWN_LOCATION");

// 	if (!cmd_list)
// 	{
// 		printf("ERROR: cmd_list is NULL\n=== END DEBUG ===\n\n");
// 		return ;
// 	}

// 	printf("cmd_list->size: %ld\n", cmd_list->size);

// 	if (!cmd_list->head)
// 	{
// 		printf("ERROR: cmd_list->head is NULL\n=== END DEBUG ===\n\n");
// 		return ;
// 	}

// 	cmd = cmd_list->head;
// 	while (cmd)
// 	{
// 		printf("Command %d:\n", cmd_index);

// 		if (!cmd)
// 		{
// 			printf("ERROR: cmd node is NULL at index %d\n", cmd_index);
// 			break ;
// 		}

// 		if (cmd->cmd)
// 		{
// 			printf("  cmd: [");
// 			for (int i = 0; cmd->cmd[i]; i++)
// 			{
// 				if (cmd->cmd[i])
// 				{
// 					printf("\"%s\"", cmd->cmd[i]);
// 					if (cmd->cmd[i + 1])
// 						printf(", ");
// 				}
// 				else
// 				{
// 					printf("NULL_ARG");
// 				}
// 			}
// 			printf("]\n");
// 		}
// 		else
// 		{
// 			printf("  cmd: NULL\n");
// 		}

// 		printf("  builtin_type: %d\n", cmd->builtin_type);

// 		if (cmd->files)
// 		{
// 			printf("  files->size: %ld\n", cmd->files->size);

// 			if (cmd->files->head)
// 			{
// 				file = cmd->files->head;
// 				file_index = 0;
// 				while (file)
// 				{
// 					if (!file)
// 					{
// 						printf("ERROR: file node is NULL at index %d\n",
// 							file_index);
// 						break ;
// 					}
// 					printf("    file[%d]: \"%s\" type=%d heredoc_idx=%d heredoc_total=%d\n",
// 						file_index,
// 						file->filename ? file->filename : "NULL_FILENAME",
// 						file->redir_type, file->heredoc_index,
// 						file->heredocs_total);
// 					file = file->next;
// 					file_index++;

// 					if (file_index > 100)
// 					{
// 						printf("ERROR: Infinite loop detected in file list\n");
// 						break ;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				printf("  files->head: NULL\n");
// 			}
// 		}
// 		else
// 		{
// 			printf("  files: NULL\n");
// 		}

// 		printf("\n");
// 		cmd = cmd->next;
// 		cmd_index++;

// 		if (cmd_index > 100)
// 		{
// 			printf("ERROR: Infinite loop detected in cmd list\n");
// 			break ;
// 		}
// 	}
// 	printf("=== END DEBUG ===\n\n");
// }