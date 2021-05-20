#ifndef SHELL_CONTROLER_H
#define SHELL_CONTROLER_H

#include <string>
#include <vector>
#include <iostream>
#include "Shell.h"

class ShellController
{
private:
    Shell *active_shell;

public:
    ShellController();
    ~ShellController();
    void readCommand();
    bool evaluateCommand(std::string command, int argc, std::vector<std::string> argv);
};

#endif // SHELL_CONTROLER_H