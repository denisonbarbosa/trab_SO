#include "ShellController.h"
#include <iostream>
using namespace std;

void ShellController::readCommand()
{
    char *cmd_input, *cmd_output, **argv;
    cin.get(cmd_input, 100, ' ');
    int i = 0;
    while (cin.get(argv[i++], 100, ' '));
    argv[i] = nullptr;

    this->evaluateCommand(cmd_input, i, argv);
}

void ShellController::evaluateCommand(char *cmd_input, int argc, char **argv)
{
    /*
    COMMAND LIST:
    - history: Show last 50 commands
    - exit: close the program
    - kill: kills specified process
    - jobs: lists all jobs in execution
    - export: (re)defines environment variable
    - cd: directory navigation | starts at $MYPATH
    - echo: prints string on screen
    - fg: show background processes
    - bg: show foreground processes
    - set: view all environment variables
    */
}