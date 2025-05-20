#include <stdlib.h>      // getenv

#include <stdio.h>       // printf, perror
#include <stdlib.h>      // malloc, free, exit
#include <string.h>      // strerror
#include <unistd.h>      // write, access, fork, getcwd, chdir, dup, dup2, pipe, isatty, ttyname, ttyslot

#include <fcntl.h>       // open
#include <sys/types.h>   // types for stat, wait
#include <sys/stat.h>    // stat, lstat, fstat
#include <dirent.h>      // opendir, readdir, closedir

#include <sys/wait.h>    // wait, waitpid, wait3, wait4
#include <sys/time.h>    // struct timeval (used in wait3/wait4)
#include <sys/resource.h> // struct rusage (used in wait3/wait4)

#include <signal.h>      // signal, sigaction, sigemptyset, sigaddset, kill

#include <termios.h>     // tcgetattr, tcsetattr
#include <sys/ioctl.h>   // ioctl

#include <term.h>        // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

#include <readline/readline.h>      // readline, rl_on_new_line, rl_replace_line, rl_redisplay
#include <readline/history.h>       // add_history, rl_clear_history
