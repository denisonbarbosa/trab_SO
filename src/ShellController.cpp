#include "ShellController.h"

Shell* ShellController::get_active_shell()
{
    return this->active_shell;
}

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
    std::string buffer, command, aux, myps1_content;
    std::vector<std::string> argv; // argv[0..n] = arguments
    int argc = 0;

    myps1_content = active_shell->get_var_content("MYPS1");
    std::cout << myps1_content << "$ ";

    std::cin.exceptions(std::cin.failbit | std::cin.eofbit);
    try
    {
        std::getline(std::cin, buffer);
    }
    catch (const std::iostream::failure &e)
    {
        std::cout << std::endl;
        this->active_shell->command_exit();
    }

    if (buffer.empty())
        return;

    active_shell->push_history(buffer);

    // Iterates through buffer and breaks it into the args
    std::istringstream buffer_stream(buffer);
    while (std::getline(buffer_stream, aux, ' '))
    {
        argv.push_back(aux);
        argc++;
    }

    command.assign(argv[0].c_str());
    argv.erase(argv.begin());

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
        active_shell->command_kill(std::stoi(argv[0]), argv[1]); // 14 - Implemented - Needs testing
    }
    else if (command.compare("jobs") == 0)
    {
        active_shell->command_jobs(); // 15 - Semi Implemented
    }
    else if (command.compare("export") == 0)
    {
        active_shell->command_export(argv[0]); // 7 - Implemented - Working properly
    }
    else if (command.compare("cd") == 0)
    {
        // TODO: validate argument
        active_shell->command_cd(argv[0]); // 8 - Implemented - NOT WORKING AS IT SHOULD
    }
    else if (command.compare("echo") == 0)
    {
        active_shell->command_echo(argv[0]); // 19 - Implemented - Working properly
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
        active_shell->command_set(); // 20 - Implemented - Working properly
    }
    else
    {
        this->active_shell->set_waiting(true);
        if (argv[argc-1] == "&")
        {
            this->active_shell->set_waiting(false);
        }

        argv.pop_back();        
        auto child_pid = active_shell->exec_program(command, argv);

        if (child_pid > 0)
        {
            if (active_shell->is_waiting())
            {
                waitpid(child_pid, nullptr, 0);
                active_shell->remove_child(child_pid);
            }
        }
    }
    return true;
}