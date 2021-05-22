#include "Shell.h"

// Shell starter
Shell::Shell()
{
    // Initialize the standard env_vars (MYPATH, MYPS1, PWD)
    this->env_vars.emplace("MYPATH", std::getenv("PATH"));
    this->env_vars.emplace("MYPS1", "tecii");
    this->env_vars.emplace("PWD", std::getenv("PWD"));

    // Maps all the possible signals
    this->SIGNALS.emplace("SIGHUP", 1);
    this->SIGNALS.emplace("SIGINT", 2);
    this->SIGNALS.emplace("SIGQUIT", 3);
    this->SIGNALS.emplace("SIGILL", 4);
    this->SIGNALS.emplace("SIGTRAP", 5);
    this->SIGNALS.emplace("SIGABRT", 6);
    this->SIGNALS.emplace("SIGIOT", 6);
    this->SIGNALS.emplace("SIGBUS", 7);
    this->SIGNALS.emplace("SIGKILL", 9);
    this->SIGNALS.emplace("SIGSEGV", 11);
    this->SIGNALS.emplace("SIGTERM", 15);
    this->SIGNALS.emplace("SIGSTKFLT", 16);
    this->SIGNALS.emplace("SIGCHLD", 17);
    this->SIGNALS.emplace("SIGCONT", 18);
    this->SIGNALS.emplace("SIGSTOP", 19);
    this->SIGNALS.emplace("SIGTSTP", 20);
    this->SIGNALS.emplace("SIGTTIN", 21);
    this->SIGNALS.emplace("SIGTTOU", 22);
    this->SIGNALS.emplace("SIGXCPU", 24);
    this->SIGNALS.emplace("SIGIO", 29);
    this->SIGNALS.emplace("SIGSYS", 31);
}

// Shell destroyer
Shell::~Shell()
{
}

const std::string Shell::get_var_content(std::string var_name)
{
    auto var = this->env_vars.find(var_name);

    if (var == this->env_vars.end())
        return nullptr;

    return var->second;
}

void Shell::push_history(std::string cmd) // Update the list of used commands
{
    if (this->historySize == 50) // If the list has 50
    {
        this->pop_history(); // Removes the "oldest"
    }
    history.push_back(cmd); // Adds to the end of the list
    historySize++;          // Updates the counter
}

void Shell::pop_history() // Remove element from the list of used commands
{
    if (this->historySize == 0) // If there's no command, returns
    {
        return;
    }
    history.pop_front(); // Removes the first element
    historySize--;       // Updates the counter
}

void Shell::command_history() // Show last 50 commands
{
    for (auto const entry : this->history) // For each command in the data structure
    {
        std::cout << entry << std::endl; // Print the entire command
    }
}

void Shell::command_exit() // For the exit command and CTRL + D
{
    exit(0); // Closes the Shell
}

void Shell::command_kill(pid_t process_pid, std::string signal) // Kills the ${arg} process
{
    auto sig_iter = this->SIGNALS.find(signal); // Searches the map for the signal number
    if (sig_iter == this->SIGNALS.end())
        return;

    kill(process_pid, sig_iter->second); // Send the signal to the process
}

void Shell::command_jobs() // Lists all jobs on the background
{

    for (auto const child : this->children) // // For each jobs on the background
    {
        std::cout << child.first << " " << child.second << std::endl; // Print the job
    }
}

void Shell::command_export(std::string entry) // (Re)defines environment variable
{
    std::string dest_var_name, source_var_name, aux, content;
    std::vector<std::string> vars;
    int i = 0;
    while (entry[i] != '=')
    {
        dest_var_name.push_back(entry[i++]);
    }
    i++;
    if (entry[i] == '$')
    {
        i++;
        while (entry[i] != ':')
        {
            source_var_name.push_back(entry[i++]);
        }
    }

    while (i < entry.length())
    {
        aux.push_back(entry[i++]);
    }

    if (!source_var_name.empty())
        content.append(this->get_var_content(source_var_name).c_str());

    content.append(aux);

    auto env_var = this->env_vars.find(dest_var_name);
    if (env_var == this->env_vars.end())
    {
        this->env_vars.insert(std::make_pair(dest_var_name, content));
        return;
    }
    env_var->second = content;
}

void Shell::command_cd(std::string arg) // Directory navigation | update PDW
{
    auto path = this->env_vars.find("PWD")->second;
    path.append(arg);
    chdir(path.c_str());

    std::string content;
    content.assign("PWD=");
    content.append(path);
    this->command_export(content);
}

void Shell::command_echo(std::string arg) // Prints ${arg} content on screen
{
    if (arg[0] == '$')
    {
        arg.erase(arg.begin());
        auto env_var = this->env_vars.find(arg);
        if (env_var == this->env_vars.end())
            std::cout << std::endl;
        else
            std::cout << env_var->second << std::endl;
    }
    else
    {
        std::cout << arg << std::endl;
    }
}

void Shell::command_fg(std::string arg) // puts process ${arg} on foreground
{
    auto child_pid = this->children.find(std::stoi(arg))->first;
    waitpid(child_pid, nullptr, 0);
}

void Shell::command_bg(std::string arg) // puts process ${arg} on background
{
    auto child_pid = this->children.find(std::stoi(arg))->first;
    this->command_kill(child_pid, "SIGCONT");
}

void Shell::command_set()
{
    for (auto const var : this->env_vars) // For each environment variable in the data structure
    {
        std::cout << "$" << var.first << " = " << var.second << std::endl; // Print "name = content"
    }
}

pid_t Shell::exec_program(std::string program_name, std::vector<std::string> args)
{
    auto program = search_program(program_name);
    if (!program.empty())
    {
        char **argv = new char *[args.size()];
        int argc = 0;
        for (auto const arg : args)
            arg.copy(argv[argc++], arg.length(), 0);

        auto pid = fork();
        if (pid == 0)
        {
            auto err = execv(program.c_str(), argv);
            exit(1);
        }

        this->children.emplace(pid, program_name);
        return pid;
    }
    return 0;
}

std::string Shell::search_program(std::string program_name)
{
    struct dirent *entry = nullptr;
    DIR *dp = nullptr;
    auto paths = break_env_var("MYPATH");
    auto ret = std::string();

    for (auto path : paths)
    {
        dp = opendir(path.c_str());
        if (dp != nullptr)
        {
            while (entry = readdir(dp))
            {
                if (program_name.compare(entry->d_name))
                    ret = path.append(entry->d_name);
            }
        }

        closedir(dp);
    }
    return ret;
}

std::vector<std::string> Shell::break_env_var(std::string var_name)
{
    std::string content;
    std::vector<std::string> pieces;
    try
    {
        content.assign(this->env_vars.find(var_name)->second);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    size_t position = 0;
    while ((position = content.find(':')) != std::string::npos)
    {
        pieces.push_back(content.substr(0, position));
        content.erase(position + 1);
    }
    return pieces;
}

void Shell::set_waiting(bool status)
{
    this->waiting = status;
}