#include "Shell.h"
#include <string.h>
#include <iostream>
using namespace std;

Shell::~Shell()
{
}

void Shell::push_history(char *cmd)
{
    if (this->historySize == 50) // If the list has 50
    {
        this->pop_history(); // Removes the "oldest"
    }
    history.push_back(cmd); // Adds to the end of the list
    historySize++; // Updates the counter
}

void Shell::pop_history()
{
    if (this->historySize == 0) // If there's no command, returns
    {
        return;
    }
    history.pop_front(); // Removes the first element
    historySize--; // Updates the counter
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
    for (auto const entry : this->history) // For each command in the data structure
    {
        printf("%s\n", entry); // Print the entire command
    }
}

void Shell::command_exit()
{
    exit(0);
}

void Shell::command_kill(char *argv)
{
    // TODO
}

void Shell::command_jobs()
{
    // TODO
}

void Shell::command_export(char *env_var, char *content)
{
    auto search = this->env_vars.find(env_var); // Searches the data structure for the requested environment variable

    if (search == this->env_vars.end()) // If didn't find
    {
        this->env_vars.insert(std::make_pair(env_var, content)); // Create new pair and insert into the data structure
        return;
    }
    search->second = strcat(search->second, content); // Append to the previous content
}

void Shell::command_cd(char *argv)
{
    // TODO
}

void Shell::command_echo(char *argv)
{
    auto env_var = this->env_vars.find(argv); // Searches the data structure for the requested environment variable

    char *answer;
    if (env_var == this->env_vars.end()) // If env_var not found, print string argv or empty line
    {
        answer = argv;
        if (argv[0] == '$')
            answer = "\n";
        printf("%s\n", answer);
        return;
    }
    printf("%s\n", env_var->second); // Prints content of the variable
}

void Shell::command_fg(char *argv)
{
    // TODO
}

void Shell::command_bg(char *argv)
{
    // TODO
}

void Shell::command_set()
{
    for (auto const var : this->env_vars) // For each environment variable in the data structure
    {
        printf("%s = %s\n", var.first, var.second); // Print "name = content"
    }
}