#include "minishell.h"

/*
 * global variable set for signal handling
 */
volatile sig_atomic_t	g_sigint_received = 0;

// rl_on_new_line();       Tell readline we're on new line
// rl_replace_line("", 0); Clear current input line
// rl_redisplay();         Refresh prompt display
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
	struct sigaction	sig_tstp;

	sigemptyset(&sig_int.sa_mask);
	sigemptyset(&sig_quit.sa_mask);
	sigemptyset(&sig_tstp.sa_mask);

	// Ignore SIGQUIT (Ctrl+\)
	sig_quit.sa_handler = SIG_IGN;
	sig_quit.sa_flags = SA_RESTART;

	// Ignore SIGTSTP (Ctrl+Z)
	sig_tstp.sa_handler = SIG_IGN;
	sig_tstp.sa_flags = SA_RESTART;

	// Handle SIGINT (Ctrl+C)
	sig_int.sa_handler = set_siginit;
	sig_int.sa_flags = SA_RESTART;

	// Register signal handlers
	sigaction(SIGQUIT, &sig_quit, NULL);
	sigaction(SIGINT, &sig_int, NULL);
	sigaction(SIGTSTP, &sig_tstp, NULL);  // ✅ Use SIGTSTP, not SIGSTOP

	disable_ctrlc_print();
}

