#include "Shell.h"
#include <string>
#include <iostream>

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

void command_history()
{
    // TODO
}

void command_exit()
{
    // TODO
}

void command_kill(char* argv)
{
    // TODO
}

void command_jobs()
{
    // TODO
}

void command_export(char* argv)
{
    // TODO
}

void command_cd(char* argv)
{
    // TODO
}

void command_echo(char* argv)
{
    // TODO
}

void command_fg(char* argv)
{
    // TODO
}

void command_bg(char* argv)
{
    // TODO
}

void command_set()
{
    // TODO
}