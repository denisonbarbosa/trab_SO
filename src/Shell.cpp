#include "Shell.h";

Shell::Shell()
{
}

Shell::~Shell()
{
}

void Shell::set_cmd(char* new_cmd)
{
    *cmd = *new_cmd;
}

void Shell::set_cmd2(char* new_cmd2)
{
    *cmd2 = *new_cmd2;
}

void Shell::set_argv(char **new_argv)
{
    **argv = **new_argv;
}