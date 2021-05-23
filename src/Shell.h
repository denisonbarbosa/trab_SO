#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <list>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <dirent.h>
#include <fcntl.h>

const int MAX = 100;

class Shell
{
private:
    int historySize = 0;                                   // Counter of the number of history entries
    std::list<std::string> history;                        // Map that stores the last 50 commands
    std::unordered_map<std::string, std::string> env_vars; // Map that stores the environment variables | name -> value
    std::unordered_map<pid_t, std::string> children;       // Map that stores active processes | pid -> program_name
    std::unordered_map<std::string, int> SIGNALS;          // Map that stores all the possible signals | sig_name -> sig_number

public:
    /**
    * @brief Construct a new Shell:: Shell object
    * 
    */
    Shell();

    /**
     * @brief Destroy the Shell object
     * 
     */
    ~Shell();

    /**
     * @brief Removes the specified child from the children list
     * 
     * @param child_pid pid of the child to be removed
     */
    void remove_child(pid_t child_pid);

    /**
     * @brief Returns the content of the specified variable
     * 
     * @param var_name Name of the variable
     * @return const std::string 
     */
    const std::string get_var_content(std::string var_name);

    /**
     * @brief Adds a command to the list of commands
     * 
     * @param cmd string containing the entire command
     */
    void push_history(std::string cmd);

    /**
     * @brief Removes the first ("oldest") command of the list
     * 
     */
    void pop_history();

    /**
     * @brief Shows the last 50 commands used
     * 
     */
    void command_history();

    /**
     * @brief Exit the application
     * 
     */
    void command_exit();

    /**
     * @brief Sends a signal to the specified process
     * 
     * @param process_pid PID of the process that will receive the signal
     * @param signal Name of the signal to send
     */
    void command_kill(pid_t process_pid, std::string signal);

    /**
     * @brief Lists all jobs in the background
     * 
     */
    void command_jobs();

    /**
     * @brief (Re)defines the specified environment variable
     * 
     * @param entry String containing the new content
     */
    void command_export(std::string entry);

    /**
     * @brief Navigates to home directory
     * 
     */
    void command_cd();
    
    /**
     * @brief Navigates to the specified directory
     * 
     * @param arg Relative path to the directory
     */
    void command_cd(std::string arg);

    /**
     * @brief Prints the content of the specified env_var or the string passed 
     * 
     * @param arg Value to be printed
     */
    void command_echo(std::string arg);

    /**
     * @brief Puts the specified process on foreground
     * 
     * @param arg Pid of the process
     */
    void command_fg(std::string arg);

    /**
     * @brief Puts the specified process on background
     * 
     * @param arg Pid of the process
     */
    void command_bg(std::string arg);

    /**
     * @brief Prints all the environment variables
     * 
     */
    void command_set();

    /**
     * @brief Executes the specified program on a child process
     * 
     * @param program_name Name of the program to execute
     * @param argv Vector of arguments
     * @param wait Specifies wheter the parent needs to wait for the child to finish
     * @return id of the new process created
     */
    void exec_program(std::string programName,std::vector<std::string> argv, bool wait);

    /**
     * @brief Searches for the directory containing the specified program
     * 
     * @param program_name Name of the program to search
     * @return Path to the directory
     */
    std::string search_program(std::string program_name);

    /**
     * @brief Breaks the string value of the specified var into a vector
     * 
     * @param var_name Name of the variable
     * @return std::vector<std::string> 
     */
    std::vector<std::string> break_env_var(std::string var_name);
};
#endif // SHELL_H
