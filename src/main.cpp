#include "ShellController.h"

void sig_handler(int sig_number);

ShellController* const controller = new ShellController();

int main()
{
    // signal(SIGINT, sig_handler);
    // signal(SIGTSTP, sig_handler);
    signal(SIGCHLD, sig_handler);
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
        auto pid = wait(nullptr);
        controller->get_active_shell()->remove_child(pid);
        return;
    }
}