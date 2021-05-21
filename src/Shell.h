#ifndef SHELL_H
#define SHELL_H

#include <experimental/filesystem>
#include <filesystem>
#include <string>
#include <list>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

const int MAX = 100;

class Shell
{
private:
    int historySize = 0;                                   // Counter of the number of history entries
    std::list<std::string> history;                        // Data structure that stores the last 50 commands
    std::unordered_map<std::string, std::string> env_vars; // Data structure that stores the environment variables
    std::unordered_map<pid_t, bool> processes;             // Map that stores active processes | true: foreground false: background
    bool waiting = false;

public:
    Shell();
    ~Shell();
    const std::string get_var_content(std::string);
    void push_history(std::string);                                             // adds ${arg} to the end of history list
    void pop_history();                                                         // removes first command in history
    void command_history();                                                     // Show last 50 commands
    void command_exit();                                                        // close the shell
    void command_kill(std::string);                                             // kills the ${arg} process
    void command_jobs();                                                        // lists all jobs on the background
    void command_export(std::string env_var, std::string content);              // (re)defines environment variable
    void command_cd(std::string arg);                                           // directory navigation | update PDW
    void command_echo(std::string arg);                                         // Prints ${arg} content on screen
    void command_fg(std::string arg);                                           // puts process ${arg} on foreground
    void command_bg(std::string arg);                                           // puts process ${arg} on background
    void command_set();                                                         // show all environment variables
    pid_t exec_program(std::string programName, std::vector<std::string> args); // executes a program
    std::string search_program(std::string program_name);
    std::vector<std::string> break_env_var(std::string var_name);               // turns env_var string into std::vector
    inline auto is_waiting() const;
    void not_waiting();
};

inline auto Shell::is_waiting() const
{
    return waiting;
}

#endif // SHELL_H
