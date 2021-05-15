#include "Shell.h";

Shell::~Shell()
{
}

void Shell::push_history(char *cmd)
{
    history.push_back(cmd);
    historySize++;
}

void Shell::pop_history()
{
    history.pop_front();
    historySize--;
}

void Shell::set_cmd(char *new_cmd)
{
    *cmd = *new_cmd;
}

void Shell::set_cmd2(char *new_cmd2)
{
    *cmd2 = *new_cmd2;
}

void Shell::set_argv(char **new_argv)
{
    *argv = *new_argv;
}