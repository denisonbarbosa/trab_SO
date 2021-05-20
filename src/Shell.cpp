#include "Shell.h"

Shell::Shell()
{
}

Shell::~Shell()
{
}

void Shell::push_history(std::string cmd)
{
    if (this->historySize == 50) // If the list has 50
    {
        this->pop_history(); // Removes the "oldest"
    }
    history.push_back(cmd); // Adds to the end of the list
    historySize++;          // Updates the counter
}

void Shell::pop_history()
{
    if (this->historySize == 0) // If there's no command, returns
    {
        return;
    }
    history.pop_front(); // Removes the first element
    historySize--;       // Updates the counter
}

void Shell::command_history()
{
    for (auto const entry : this->history) // For each command in the data structure
    {
        std::cout << entry.c_str() << std::endl; // Print the entire command
    }
}

void Shell::command_exit()
{
    exit(0);
}

void Shell::command_kill(std::string arg)
{
    // TODO
}

void Shell::command_jobs()
{
    // TODO
}

void Shell::command_export(std::string env_var, std::string content)
{
    auto search = this->env_vars.find(env_var); // Searches the data structure for the requested environment variable

    if (search == this->env_vars.end()) // If didn't find
    {
        this->env_vars.insert(std::make_pair(env_var, content)); // Create new pair and insert into the data structure
        return;
    }
    search->second.append(content); // Append to the previous content
}

void Shell::command_cd(std::string arg)
{
    // TODO
}

void Shell::command_echo(std::string arg)
{
    auto env_var = this->env_vars.find(arg); // Searches the data structure for the requested environment variable

    if (env_var == this->env_vars.end()) // If env_var not found, print string argv or empty line
    {
        if (arg[0] == '$')
            std::cout << arg << std::endl;
        else
            std::cout << std::endl;

        return;
    }
    std::cout << env_var->second << std::endl; // Prints content of the variable
}

void Shell::command_fg(std::string arg)
{
    // TODO
}

void Shell::command_bg(std::string arg)
{
    // TODO
}

void Shell::command_set()
{
    for (auto const var : this->env_vars) // For each environment variable in the data structure
    {
        std::cout << "$" << var.first << " = " << var.second << std::endl; // Print "name = content"
    }
}