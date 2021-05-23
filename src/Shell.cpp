#include "Shell.h"

Shell::Shell()
{
    // Initialize the standard env_vars (MYPATH, MYPS1, PWD)
    this->env_vars.emplace("MYPATH", std::getenv("PATH"));
    this->env_vars.emplace("MYHOME", std::getenv("PWD"));
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

Shell::~Shell()
{
}

void Shell::remove_child(pid_t child_pid)
{
    this->children.erase(child_pid);
}

const std::string Shell::get_var_content(std::string var_name)
{
    auto var = this->env_vars.find(var_name);

    if (var == this->env_vars.end())
        return nullptr;

    return var->second;
}

void Shell::push_history(std::string cmd)
{
    if (this->historySize == 50)
    {
        this->pop_history();
    }
    history.push_back(cmd);
    historySize++;
}

void Shell::pop_history()
{
    if (this->historySize == 0)
    {
        return;
    }
    history.pop_front();
    historySize--;
}

void Shell::command_history()
{
    for (auto const entry : this->history)
    {
        std::cout << entry << std::endl;
    }
}

void Shell::command_exit()
{
    exit(0);
}

void Shell::command_kill(pid_t process_pid, std::string signal)
{
    auto sig_iter = this->SIGNALS.find(signal);
    if (sig_iter == this->SIGNALS.end())
    {
        std::cout << "Signal " << signal << " not registered in signal map" << std::endl;
        return;
    }

    kill(process_pid, sig_iter->second);
}

void Shell::command_jobs()
{

    for (auto const child : this->children)
    {
        std::cout << child.first << " " << child.second << std::endl;
    }
}

void Shell::command_export(std::string entry)
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

void Shell::command_cd()
{
    chdir(this->get_var_content("MYHOME").c_str());
    this->command_export("PWD=" + this->get_var_content("MYHOME"));
}

void Shell::command_cd(std::string arg)
{
    std::string aux, path;
    std::vector<std::string> pieces;

    if (arg[0] == '/')
    {
        path.assign(arg);
    }
    else
    {
        std::istringstream pwd_stream(this->get_var_content("PWD"));
        while (std::getline(pwd_stream, aux, '/'))
        {
            pieces.push_back(aux);
        }

        std::istringstream path_stream(arg);
        while (std::getline(path_stream, aux, '/'))
        {
            if (aux.compare("..") == 0)
                pieces.pop_back();
            else
                pieces.push_back(aux);
        }

        for (auto piece : pieces)
        {
            path.append(piece + "/");
        }
    }

    if (chdir(path.c_str()) != 0)
    {
        std::cout << "chdir() failed " << std::endl;
        return;
    }

    this->command_export("PWD=" + path);
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
    auto child_pid = this->children.find(std::stoi(arg))->first;
    waitpid(child_pid, nullptr, 0);
}

void Shell::command_bg(std::string arg)
{
    auto child_pid = this->children.find(std::stoi(arg))->first;
    this->command_kill(child_pid, "SIGCONT");
}

void Shell::command_set()
{
    for (auto const var : this->env_vars)
    {
        std::cout << "$" << var.first << " = " << var.second << std::endl;
    }
}

void Shell::exec_program(std::string command, std::vector<std::string> argv, bool wait)
{
    auto program = this->search_program(command);

    int child_pid = fork();
    int child_status;
    if (child_pid == 0)
    {
        char **args = (char **)malloc(argv.size() + 2);
        int fd_in = 0, fd_out = 0, fd_err = 0;
        /*
            fd_pipe[0] - read
            fd_pipe[1] - write
        */
        int fd_pipe[2][2];
        int pipe_level = 0;
        
        if (program.empty())
            this->command_exit();

        for (int c = 0; c < argv.size() + 2; c++)
        {
            args[c] = (char *)malloc(50 * sizeof(char));
        }

        command.copy(args[0], command.length());

        int i = 0;
        int c_arg = 1;
        for (; i < argv.size(); i++)
        {
            if (argv[i].compare("<") == 0) // entrada
            {
                if (i + 1 < argv.size())
                {
                    fd_in = open(argv[++i].c_str(),
                                 O_RDONLY,
                                 S_IRWXU | S_ISUID | S_IRWXG | S_IROTH | S_IXOTH); // Sets the ownership and gives 775 permission
                }
            }
            else if (argv[i].compare(">") == 0) // saida
            {
                if (i + 1 < argv.size())
                {
                    fd_out = open(argv[++i].c_str(),
                                  O_TRUNC | O_WRONLY | O_CREAT,
                                  S_IRWXU | S_ISUID | S_IRWXG | S_IROTH | S_IXOTH); // Sets the ownership and gives 775 permission
                }
            }
            else if (argv[i].compare("2>") == 0) // erro
            {
                if (i + 1 < argv.size())
                {
                    fd_err = open(argv[++i].c_str(),
                                  O_WRONLY,
                                  S_IRWXU | S_ISUID | S_IRWXG | S_IROTH | S_IXOTH); // Sets the ownership and gives 775 permission
                }
            }
            else if (argv[i].compare("|") == 0) // pipe
            {
                if (i+1 < argv.size() && pipe_level < 2)
                {
                    if(pipe(fd_pipe[pipe_level++]) < 0)
                        exit(1);
                    child_pid = fork();
                    

                    if (child_pid > 0)
                    {
                        if (pipe_level > 0)
                        {
                            close(fd_pipe[pipe_level-1][1]);
                            dup2(fd_pipe[pipe_level-1][0], STDIN_FILENO);
                            close(fd_pipe[pipe_level-1][0]);
                            waitpid(child_pid, nullptr, 0);
                        }
                        break;
                    }
                    else if (child_pid == 0)
                    {
                        program = search_program(argv[i+1]);
                        args = (char **)malloc(argv.size() + 1 - i);
                        
                        if (program.empty())
                            this->command_exit();

                        for (int c = 0; c < argv.size() + 1 - i; c++)
                        {
                            args[c] = (char *)malloc(50 * sizeof(char));
                        }
                        c_arg = 0;

                        close(fd_pipe[pipe_level-1][0]);
                        dup2(fd_pipe[pipe_level-1][1], STDOUT_FILENO);
                        close(fd_pipe[pipe_level-1][1]);
                    }
                }
            }
            else
            {
                argv[i].copy(args[c_arg++], argv[i].length());
                args[c_arg-1][argv[i].length()] = '\0'; 
            }
        }
        args[c_arg] = nullptr;

        if (fd_in > 0)
        {
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (fd_out > 0)
        {
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        if (fd_err > 0)
        {
            dup2(fd_err, STDERR_FILENO);
            close(fd_err);
        }

        execv(program.c_str(), args);
        exit(0);
    }
    else if (child_pid > 0)
    {
        if (wait)
            waitpid(child_pid, &child_status, 0);
        else
            this->children.emplace(child_pid, command);
    }
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
                if (program_name.compare(entry->d_name) == 0)
                {
                    ret = (path + "/").append(entry->d_name);
                    closedir(dp);
                    return ret;
                }
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
        content.erase(0, position + 1);
    }
    return pieces;
}