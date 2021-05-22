#include "ShellController.h"

void sigint_handler(int sig_number);
void sigtstp_handler(int sig_number);
void sigchld_handler(int sig_number);

const auto controller = new ShellController();

int main()
{
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    signal(SIGCHLD, sigchld_handler);
    while (true)
        controller->read_command();
}

/**
 * @brief Signal handler for SIGINT
 * 
 * @param sig_number Id of the signal
 */
void sigint_handler(int sig_number)
{
    if (sig_number == SIGINT)
        return;
}

/**
 * @brief Signal handler for SIGTSTP
 * 
 * @param sig_number Id of the signal
 */
void sigtstp_handler(int sig_number)
{
    if (sig_number == SIGTSTP) // SIGTSTP == CTRL + Z
    {
        raise(SIGSTOP);
        return;
    }
}

/**
 * @brief Signal handler for SIGCHLD
 * 
 * @param sig_number Id of the signal
 */
void sigchld_handler(int sig_number)
{
    if (sig_number == SIGCHLD)
    {
        auto pid = wait(nullptr);
        auto active_shell = controller->get_active_shell();
        active_shell->remove_child(pid);
        return;
    }
}