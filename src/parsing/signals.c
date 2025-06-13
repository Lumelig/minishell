#include "minishell.h"

/*
 * global variable set for signal handling
 */
volatile sig_atomic_t	g_sigint_received = 0;

void	set_siginit(int signum)
{
	(void)signum;
	g_sigint_received = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	disable_ctrlc_print(void)
{
	struct termios	new_setting;

	tcgetattr(STDIN_FILENO, &new_setting);
	new_setting.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_setting);
}

//Set up the sigbnal for Ctr=*
void	setup_signal_handlers(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	// ✅ Initialize both structs to zero
	memset(&sig_int, 0, sizeof(sig_int));
	memset(&sig_quit, 0, sizeof(sig_quit));

	// ✅ Setup SIGQUIT to be ignored
	sig_quit.sa_handler = SIG_IGN;

	// ✅ Setup SIGINT to use your handler
	sig_int.sa_handler = set_siginit;
	sigemptyset(&sig_int.sa_mask);
	sigaddset(&sig_int.sa_mask, SIGINT);
	sig_int.sa_flags = 0;

	// ✅ Register both signal handlers
	sigaction(SIGQUIT, &sig_quit, NULL);
	sigaction(SIGINT, &sig_int, NULL);

	disable_ctrlc_print();  // optional extra behavior
}
