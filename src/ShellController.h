#ifndef __ShellControler_h
#define __ShellControler_h

#include "Shell.h"

class ShellController
{
private:
    Shell *active_shell;

public:
    ShellController() : active_shell{new Shell()}
    {
        // do nothing
    }
    ~ShellController();
    void readCommand();
    bool evaluateCommand(char *command, int argc, char **argv);
};

ShellController::~ShellController()
{
    // Limpar os ponteiros
}

#endif