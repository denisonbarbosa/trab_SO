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
    Shell *active_shell;

public:
    ShellController();
    ~ShellController();
    void read_command();
    bool evaluate_command(std::string command, int argc, std::vector<std::string> argv);
};

#endif // SHELL_CONTROLER_H