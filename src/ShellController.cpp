#include "ShellController.h"
#include <iostream>
#include <string.h>

using namespace std;

void ShellController::readCommand()
{
    char *full_command, *cmd_input, *cmd_output, **argv;

    std::cout << "tecii$ ";
    cin.get(cmd_input, 100, ' ');
    strcpy(full_command, cmd_input);

    int i = 0;
    while (cin.get(argv[i], 100, ' '))
    {
        strcat(full_command, strcat(" ", argv[i]));
        i++;
    }
    argv[i] = nullptr;

    bool status = this->evaluateCommand(cmd_input, i, argv);
    active_shell->push_history(full_command);
}

bool ShellController::evaluateCommand(char *cmd_input, int argc, char **argv)
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
    if (strcmp(cmd_input, "history") == 0)
    {
        active_shell->command_history(); // Implemented - Needs testing
    }
    else if (strcmp(cmd_input, "exit") == 0)
    {
        active_shell->command_exit(); // Semi implemented
    }
    else if (strcmp(cmd_input, "kill") == 0)
    {
        // TODO: validate argument
        active_shell->command_kill(argv[0]);
    }
    else if (strcmp(cmd_input, "jobs") == 0)
    {
        active_shell->command_jobs();
    }
    else if (strcmp(cmd_input, "export") == 0)
    {
        // TODO: validate argument
        active_shell->command_export(argv[0], argv[1]); // Implemented - Needs testing
    }
    else if (strcmp(cmd_input, "cd") == 0)
    {
        // TODO: validate argument
        active_shell->command_cd(argv[0]);
    }
    else if (strcmp(cmd_input, "echo") == 0)
    {
        // TODO: validate argument
        active_shell->command_echo(argv[0]); // Implemented - Needs testing
    }
    else if (strcmp(cmd_input, "fg") == 0)
    {
        // TODO: validate argument
        active_shell->command_fg(argv[0]);
    }
    else if (strcmp(cmd_input, "bg") == 0)
    {
        // TODO: validate argument
        active_shell->command_bg(argv[0]);
    }
    else if (strcmp(cmd_input, "set") == 0)
    {
        active_shell->command_set(); // Implemented - Needs testing
    }
    else
    {
        return false;
    }
    return true;
}