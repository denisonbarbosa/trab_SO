#include "Shell.h"
#include <string>
#include <iostream>
using namespace std;

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

void Shell::command_history()
{
    for (auto const &entry : this->history)
    {
        printf("%s\n", entry);
    }
}

void Shell::command_exit()
{
    exit(0);
}

void Shell::command_kill(char* argv)
{
    // TODO
}

void Shell::command_jobs()
{
    // TODO
}

void Shell::command_export(char* argv)
{
    // TODO
}

void Shell::command_cd(char* argv)
{
    // TODO
}

void Shell::command_echo(char* argv)
{
    auto env_var = this->env_vars.find(argv);
    
    char* answer;
    if (env_var == this->env_vars.end()) // If env_var not found, print string argv or empty
    {
        answer = argv;
        if (argv[0] == '$')
            answer = "\n";
        printf("%s\n", answer);
        return;
    }    
    printf("%s\n", env_var->second);
}

void Shell::command_fg(char* argv)
{
    // TODO
}

void Shell::command_bg(char* argv)
{
    // TODO
}

void Shell::command_set()
{
    // TODO
}