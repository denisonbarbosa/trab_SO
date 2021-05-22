#include "ShellController.h"

void sigint_handler(int sig_number);
void sigtstp_handler(int sig_number);
void sigchld_handler(int sig_number);



int main()
{
    auto controller = new ShellController();
    // signal(SIGINT, sig_handler);
    // signal(SIGTSTP, sig_handler);
    // signal(SIGCHLD, sig_handler);
    controller->start_shell_loop();
}

/**
 * @brief 
 * 
 * @param sig_number 
 */
void sig_handler(int sig_number)
{
    if (sig_number == SIGCHLD)
    {
        /*
        auto pid = wait(nullptr);
        auto active_shell = controller->get_active_shell();
        active_shell->remove_child(pid);
        signal(SIGCHLD,SIG_IGN)
        */

        return;
    }
}