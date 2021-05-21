#include "Shell.h"

// Shell starter
Shell::Shell()
{
    // Initialize the standard env_vars (MYPATH, MYPS1, PWD)
    std::pair<std::string, std::string> path_var = std::make_pair("MYPATH", std::getenv("PATH"));
    std::pair<std::string, std::string> user_var = std::make_pair("MYPS1", "tecii");
    std::pair<std::string, std::string> pwd_var = std::make_pair("PWD", std::getenv("PWD"));
    this->env_vars.insert(path_var);
    this->env_vars.insert(user_var);
    this->env_vars.insert(pwd_var);
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

void Shell::command_kill(std::string arg) // Kills the ${arg} process
{
    pid_t pid = std::stoi(arg); // Converts arg (string) to a pid
    this->processes.erase(pid); // Remove the process
    kill(pid, SIGKILL); // Send the signal to kill the process 
}

void Shell::command_jobs() // Lists all jobs on the background
{

    for (auto const process : this->processes) // // For each jobs on the background
    {
        if (!process.second)
            std::cout << process.first << std::endl; // Print the job
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

    for (; i < entry.length(); i++)
    {
        aux.push_back(entry[i]);
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
    this->processes.find(std::stoi(arg))->second = true; 
}

void Shell::command_bg(std::string arg) // puts process ${arg} on background
{
    this->processes.find(std::stoi(arg))->second = false; 
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

        return execv(program.c_str(), argv);
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

void Shell::not_waiting()
{
    waiting = false;
}