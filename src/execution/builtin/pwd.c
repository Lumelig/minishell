#include "minishell.h"

void	pwd_builtin(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		printf("getcwd failed\n");
	printf("%s\n", cwd);
	free(cwd);
}
