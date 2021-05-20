#include "ShellController.h"

void sigint_handler(int sig_number);
void sigtstp_handler(int sig_number);
void sigchld_handler(int sig_number);

int main()
{
    auto controller = new ShellController();
    // signal(SIGINT, sigint_handler);
    // signal(SIGTSTP, sigtstp_handler);
    // signal(SIGCHLD, sigchld_handler);
    while (true)
    {
        controller->read_command();
    }
}

void sigint_handler(int sig_number)
{
    if (sig_number == SIGINT)
        return;
}

void sigtstp_handler(int sig_number)
{
    // TODO
}

void sigchld_handler(int sig_number)
{
    // TODO
}
