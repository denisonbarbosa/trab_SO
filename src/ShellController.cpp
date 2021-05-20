#include "ShellController.h"

ShellController::ShellController()
{
    this->active_shell = new Shell();
}

ShellController::~ShellController()
{
    // Limpar os ponteiros
}

void ShellController::readCommand()
{
    std::string buffer, command;
    std::vector<std::string> argv; // argv[0..n] = arguments 
    int argc = 0;

    std::cout << "tecii$ ";

    std::getline(std::cin, buffer);
    std::cout << buffer << std::endl;
    active_shell->push_history(buffer);
    
    size_t position = 0;
    while ((position = buffer.find(" ")) != std::string::npos)
    {
        argv.push_back(buffer.substr(0, position));
        buffer.erase(position + 1);
        argc++;
    }

    command = argv[0];
    argv.erase(argv.begin());

    // DEBUG
    std::cout << "comando: " << command << " argc: " << argc << std::endl;
    for (int i = 0; i < argv.size(); i++){
        std::cout << argv[i] << std::endl;
    }
    
    bool status = this->evaluateCommand(command, argc, argv);
    
}

bool ShellController::evaluateCommand(std::string command, int argc, std::vector<std::string> argv)
{
    if (command.compare("history") == 0)
    {
        active_shell->command_history(); // Implemented - Needs testing
    }
    else if (command.compare("exit") == 0)
    {
        active_shell->command_exit(); // Semi implemented
    }
    else if (command.compare("kill") == 0) // TODO - Needs implementation
    {
        // TODO: validate argument
        active_shell->command_kill(argv[0]);
    }
    else if (command.compare("jobs") == 0)
    {
        active_shell->command_jobs(); // TODO - Needs implementation
    }
    else if (command.compare("export") == 0)
    {
        // TODO: validate argument
        active_shell->command_export(argv[0], argv[1]); // Implemented - Needs testing
    }
    else if (command.compare("cd") == 0)
    {
        // TODO: validate argument
        active_shell->command_cd(argv[0]); // TODO - Needs implementation
    }
    else if (command.compare("echo") == 0)
    {
        // TODO: validate argument
        active_shell->command_echo(argv[0]); // Implemented - Needs testing
    }
    else if (command.compare("fg") == 0)
    {
        // TODO: validate argument
        active_shell->command_fg(argv[0]); // TODO - Needs implementation
    }
    else if (command.compare("bg") == 0)
    {
        // TODO: validate argument
        active_shell->command_bg(argv[0]); // TODO - Needs implementation
    }
    else if (command.compare("set") == 0)
    {
        active_shell->command_set(); // Implemented - Needs testing
    }
    else
    {
        return false;
    }
    return true;
}