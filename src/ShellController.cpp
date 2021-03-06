#include "ShellController.h"

Shell *ShellController::get_active_shell()
{
    return this->active_shell;
}

ShellController::ShellController()
{
}

ShellController::~ShellController()
{
}

void ShellController::start_shell_loop()
{
    std::cin.exceptions(std::cin.failbit | std::cin.eofbit);
    while (true)        
    {
        this->read_command();
    }
}

void ShellController::read_command()
{
    std::string buffer, command, aux;
    std::vector<std::string> argv; // argv[0..n] = arguments
    int argc = 0;

    auto myps1_content = active_shell->get_var_content("MYPS1");
    std::cout << myps1_content << "$ ";
    
    try
    {
        std::getline(std::cin, buffer);
    }
    catch (const std::iostream::failure &e)
    {
        std::cout << std::endl;
        this->active_shell->command_exit();
    }
    if (buffer.find_first_not_of(" ") == buffer.npos)
        return;

    active_shell->push_history(buffer);

    std::istringstream buffer_stream(buffer);
    while (std::getline(buffer_stream, aux, ' '))
    {
        argv.push_back(aux);
        argc++;
    }
    // argc--; unused

    command.assign(argv[0].c_str());
    argv.erase(argv.begin());
    
    this->evaluate_command(command, argv);
    return;
}

void ShellController::evaluate_command(std::string command, std::vector<std::string> argv)
{
    if (command.compare("history") == 0)
    {
        active_shell->command_history();
    }
    else if (command.compare("exit") == 0)
    {
        active_shell->command_exit();
    }
    else if (command.compare("kill") == 0)
    {
        active_shell->command_kill(std::stoi(argv[0]), argv[1]); // 14 - Implemented - Needs testing
    }
    else if (command.compare("jobs") == 0)
    {
        active_shell->command_jobs(); // 15 - Semi Implemented
    }
    else if (command.compare("export") == 0)
    {
        active_shell->command_export(argv[0]);
    }
    else if (command.compare("cd") == 0)
    {
        if (argv.empty())
            active_shell->command_cd();
        else
            active_shell->command_cd(argv[0]);
    }
    else if (command.compare("echo") == 0)
    {
        active_shell->command_echo(argv[0]);
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
        active_shell->command_set();
    }
    else
    {
        bool wait = true;
        if (!argv.empty())
        {
            if(argv[argv.size() - 1] == "&")
            {
                wait = false;
                argv.pop_back();
            }
        }
        this->active_shell->exec_program(command, argv, wait);
    }
}