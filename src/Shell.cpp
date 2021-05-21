#include "Shell.h"

Shell::Shell()
{
    std::pair<std::string, std::string> path_var = std::make_pair("MYPATH", std::getenv("PATH"));
    std::pair<std::string, std::string> user_var = std::make_pair("MYPS1", "tecii");
    std::pair<std::string, std::string> pwd_var = std::make_pair("PWD", std::getenv("PWD"));
    this->env_vars.insert(path_var);
    this->env_vars.insert(user_var);
    this->env_vars.insert(pwd_var);
}

Shell::~Shell()
{
}

void Shell::push_history(std::string cmd)
{
    if (this->historySize == 50) // If the list has 50
    {
        this->pop_history(); // Removes the "oldest"
    }
    history.push_back(cmd); // Adds to the end of the list
    historySize++;          // Updates the counter
}

void Shell::pop_history()
{
    if (this->historySize == 0) // If there's no command, returns
    {
        return;
    }
    history.pop_front(); // Removes the first element
    historySize--;       // Updates the counter
}

void Shell::command_history()
{
    for (auto const entry : this->history) // For each command in the data structure
    {
        std::cout << entry << std::endl; // Print the entire command
    }
}

void Shell::command_exit()
{
    // TODO: CTRL+D -> EOF, not a signal
    exit(0);
}

void Shell::command_kill(std::string arg)
{
    // TODO
    pid_t pid = std::stoi(arg);
    this->processes.erase(pid);
    kill(pid, SIGKILL);
}

void Shell::command_jobs() // lists all jobs on the background
{

    for (auto const process : this->processes)
    {
        if (!process.second)
            std::cout << process.first << std::endl;
    }
}

void Shell::command_export(std::string env_var, std::string content)
{
    auto search = this->env_vars.find(env_var); // Searches the data structure for the requested environment variable

    if (search == this->env_vars.end()) // If didn't find
    {
        this->env_vars.insert(std::make_pair(env_var, content)); // Create new pair and insert into the data structure
        return;
    }
    search->second.append(content); // Append to the previous content
}

void Shell::command_cd(std::string arg)
{
    auto path = this->env_vars.find("PWD")->second;
    path.append(arg);
    chdir(path.c_str());
    this->command_export("PWD", path);
}

void Shell::command_echo(std::string arg)
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

void Shell::command_fg(std::string arg)
{
    this->processes.find(std::stoi(arg))->second = true; // puts process ${arg} on foreground
}

void Shell::command_bg(std::string arg)
{
    this->processes.find(std::stoi(arg))->second = false; // puts process ${arg} on background
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
        char** argv = new char*[args.size()];
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
        if (dp != nullptr) {
            while (entry = readdir(dp))
            {
                if(program_name.compare(entry->d_name))
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