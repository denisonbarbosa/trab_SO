#include "ShellController.h"

ShellController::ShellController()
{
    this->active_shell = new Shell();
}

ShellController::~ShellController()
{
    // Limpar os ponteiros
}

void ShellController::read_command()
{
    std::string buffer, command;
    std::vector<std::string> argv; // argv[0..n] = arguments
    int argc = 0;

    std::cout << "tecii$ ";

    std::cin.exceptions(std::cin.failbit | std::cin.eofbit);
    try
    {
        std::getline(std::cin, buffer);
    }
    catch (const std::iostream::failure &e)
    {
        if (std::cin.fail() || std::cin.eof())
            std::cout << "Error: " << e.what() << std::endl;
        this->active_shell->command_exit();
    }

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
    for (int i = 0; i < argv.size(); i++)
    {
        std::cout << argv[i] << std::endl;
    }

    bool status = this->evaluate_command(command, argc, argv);
}

bool ShellController::evaluate_command(std::string command, int argc, std::vector<std::string> argv)
{
    if (command.compare("history") == 0)
    {
        active_shell->command_history(); // 9 - Implemented - Needs testing
    }
    else if (command.compare("exit") == 0)
    {
        active_shell->command_exit(); // 11 - Implemented - Needs testing
    }
    else if (command.compare("kill") == 0)
    {
        // TODO: validate argument
        active_shell->command_kill(argv[0]); // 14 - Implemented - Needs testing
    }
    else if (command.compare("jobs") == 0)
    {
        active_shell->command_jobs(); // 15 - Semi Implemented
    }
    else if (command.compare("export") == 0)
    {
        // TODO: validate argument
        active_shell->command_export(argv[0], argv[1]); // 7 - Implemented - Needs testing
    }
    else if (command.compare("cd") == 0)
    {
        // TODO: validate argument
        active_shell->command_cd(argv[0]); // 8 - Implemented - Needs testing
    }
    else if (command.compare("echo") == 0)
    {
        // TODO: validate argument
        active_shell->command_echo(argv[0]); // 19 - Implemented - Needs testing
    }
    else if (command.compare("fg") == 0)
    {
        // TODO: validate argument
        active_shell->command_fg(argv[0]); // 16 - Semi Implemented
    }
    else if (command.compare("bg") == 0)
    {
        // TODO: validate argument
        active_shell->command_bg(argv[0]); // 17 - Semi Implemented
    }
    else if (command.compare("set") == 0)
    {
        active_shell->command_set(); // 20 - Implemented - Needs testing
    }
    else
    {
        auto child_pid = active_shell->exec_program(command, argv);
        if (child_pid > 0)
        {
            if (active_shell->is_waiting())
                waitpid(child_pid, nullptr, 0);
        }
        active_shell->not_waiting();
    }
    return true;
}