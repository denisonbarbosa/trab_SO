#ifndef SHELL_CONTROLER_H
#define SHELL_CONTROLER_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Shell.h"

class ShellController
{
private:
    Shell *active_shell = new Shell();

public:
    /**
     * @brief Construct a new Shell Controller object
     * 
     */
    ShellController();

    /**
     * @brief Destroy the Shell Controller object
     * 
     */
    ~ShellController();

    /**
     * @brief Get the active shell object
     * 
     * @return Shell* 
     */
    Shell* get_active_shell();

    /**
     * @brief 
     * 
     */
    void start_shell_loop();
    
    /**
     * @brief Reads the user input and treats it
     * 
     */
    void read_command();

    /**
     * @brief Evaluates the command and calls the correct Shell method
     * 
     * @param command Command to execute
     * @param argc Number of arguments
     * @param argv Vector of arguments
     */
    void evaluate_command(std::string command, int argc, std::vector<std::string> argv);
};

#endif // SHELL_CONTROLER_H