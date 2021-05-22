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
    {
        controller->read_command();
    }
}

void sigint_handler(int sig_number) // Prevent the shell from ending when the Control + C keys are pressed
{
    if (sig_number == SIGINT)
        return;
}

void sigtstp_handler(int sig_number) // Take the function in the foreground and set it to sleep
{
    if (sig_number == SIGTSTP) // SIGTSTP == CTRL + Z
    {
        raise(SIGSTOP);
        //  teste
        return;
    }
}

void sigchld_handler(int sig_number) // Avoid creating zombie processes
{
    if (sig_number == SIGCHLD) // Terminated or stopped child
    {
        auto pid = wait(nullptr);
        auto active_shell = controller->get_active_shell();
        active_shell->remove_child(pid);
        return;
    }
}