#ifndef __Shell_h
#define __Shell_h

#include <list>
#include <unordered_map>

const int MAX = 100;

using namespace std;

class Shell
{
private:
    char cmd[MAX];
    char cmd2[MAX];
    char *argv[MAX];
    int historySize = 0;
    list<char *> history;
    unordered_map<char *, char *> env_vars; // TODO read & change content

public:
    Shell();
    ~Shell();
    void push_history(char *command); // Adds to the end
    void pop_history();               // Removes from the beginning
    auto get_cmd() const;
    auto get_cmd2() const;
    auto get_argv() const;
    void set_cmd(char *new_cmd);
    void set_cmd2(char *new_cmd2);
    void set_argv(char **new_argv);
    void command_history(); // Show last 50 commands
    void command_exit(); // close the process
    void command_kill(char* argv); // kills specified process
    void command_jobs(); // lists all jobs in execution
    void command_export(char* argv); // (re)defines environment variable
    void command_cd(char* argv); // directory navigation | update PDW
    void command_echo(char* argv); // prints string on screen
    void command_fg(char* argv); // show background processes
    void command_bg(char* argv); // show foreground processes
    void command_set(); // show all environment variables
};

auto Shell::get_cmd() const
{
    return this->cmd;
}
auto Shell::get_cmd2() const
{
    return this->cmd2;
}

auto Shell::get_argv() const
{
    return this->argv;
}
#endif __Shell_h
